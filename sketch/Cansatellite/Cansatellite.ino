#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

typedef struct { unsigned long t_prev, t_now; double t_period; } dt_t; // 주기 계산용 구조체
typedef struct { double roll, pitch, yaw; } gy_ra_t; // 자이로 각속도 구조체
typedef struct { double roll, pitch, yaw; } gy_angle_t; // 자이로 회전각 구조체
typedef struct { double roll, pitch, yaw; } tmp_angle_t; // 자이로 필터적용 회전각 구조체 (임시)
typedef struct { double roll, pitch, yaw; } fi_angle_t; // 자이로 필터적용 회전각 구조체

SoftwareSerial BTSerial(8, 9); // Create bluetooth object
MPU6050 accelgyro; // Create mpu6050 object

int16_t gx, gy, gz; // Gyro raw value
int16_t gx_of, gy_of, gz_of; //Gyro average value
int16_t gx_ad, gy_ad, gz_ad; //Gyro adjusted value
dt_t dt; // 주기
gy_ra_t gy_ra; //Gyro rate value ( 각속도 )
gy_angle_t gy_angle; //Gyro angle value
tmp_angle_t tmp_angle;
fi_angle_t fi_angle; //Gyro filtered angle value

int16_t ax, ay, az;
int16_t ax_of, ay_of, az_of;
double accel_angle_x, accel_angle_y, accel_angle_z;

String str="M";
char out[30]="";

// Calculate average value of gyro
void calcOffset(){
  int32_t  sumX =0, sumY = 0, sumZ = 0;
  int32_t  aX =0, aY = 0, aZ = 0;
  
  for(int i=0;i<100;i++){
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    sumX += gx;
    sumY += gy;
    sumZ += gz;
    aX += gx;
    aY += gy;
    aZ += gz;
    delay(1);
  }

  gx_of = sumX/100;
  gy_of = sumY/100;
  gz_of = sumZ/100;
  ax_of = aX/100;
  ay_of = aY/100;
  az_of = aZ/100;
}

// Calculate adjusted value of gyro
void calcAdjust(){
  gx_ad = gx - gx_of;
  gy_ad = gy - gy_of;
  gz_ad = gz - gz_of;
}

// Calculate period
void calcDt(){
  dt.t_now = millis();
  dt.t_period = (dt.t_now - dt.t_prev)/1000.0;
  dt.t_prev = dt.t_now;
}

// Calculate rotate speed
void calcRate(){
  gy_ra.roll = gy_ad/131.0;
  gy_ra.pitch = gx_ad/131.0;
  gy_ra.yaw = gz_ad/131.0;
}

// Calculate rotate angle
void calcAngle(){
  gy_angle.roll += gy_ra.roll *dt.t_period;
  gy_angle.pitch += gy_ra.pitch *dt.t_period;
  gy_angle.yaw += gy_ra.yaw *dt.t_period;
}

void calcAccel(){
  double accel_x, accel_y, accel_z;
  double accel_xz, accel_yz;
  const double RAD = 180/3.14159;

  accel_x = ax - ax_of;
  accel_y = ay - ay_of;
  accel_z = az + (16384 - az_of);

  accel_yz = sqrt(pow(accel_y,2) + pow(accel_z, 2));
  accel_angle_x = atan(-accel_x/accel_yz)*RAD;

  accel_xz = sqrt(pow(accel_x,2) + pow(accel_z, 2));
  accel_angle_y = atan(-accel_y/accel_xz)*RAD;

  accel_angle_z = 0;
}

// Calculate filtered angle
void clacFilter(){
  tmp_angle.roll = fi_angle.roll + (gy_ra.roll *dt.t_period);
  tmp_angle.pitch = fi_angle.pitch + (gy_ra.pitch *dt.t_period);
  tmp_angle.yaw = fi_angle.yaw + (gy_ra.yaw *dt.t_period);
  
  fi_angle.roll = 0.96 * tmp_angle.roll + (0.04 * accel_angle_x);
  fi_angle.pitch = 0.96 * tmp_angle.pitch + (0.04 * accel_angle_y);
  fi_angle.yaw = 0.99 * tmp_angle.yaw;
}

void setup() {
  BTSerial.begin(9600);

  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

  accelgyro.initialize();
  accelgyro.testConnection();
  calcOffset(); // 자이로 평균값 계산
  dt.t_prev = millis(); // 주기 계산용 초기화 값
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // 원시 자이로 값
  calcAdjust(); // 자이로 보정값 계산
  calcRate(); // 각속도 계산
  calcDt(); // 주기 계산
  calcAccel(); // 가속도 계산
  calcAngle(); // 회전각 계산
  clacFilter(); // 상보필터 적용
  
  str+=String((int)fi_angle.pitch);
  str+=String('E');
  str+=String((int)fi_angle.roll);
  str+=String('E');
  str+=String((int)fi_angle.yaw);
  str+=String('E');
  str.toCharArray(out, str.length()+1);
  BTSerial.write(out);
  
  str="M";
  strcpy(out, "");
  delay(100);
}
