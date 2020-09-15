#include <Wire.h>
#include <SoftwareSerial.h>

const int MPU_addr = 0x68; // mpu-6050의 I2C 주소
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; // 가속도, 자이로 원시 값
float gyro_x, gyro_y, gyro_z; //각속도 변수 //각속도 : 단위시간당 회전한 각도
float accel_angle_x, accel_angle_y, accel_angle_z; // 가속도 회전각
float gyro_angle_x, gyro_angle_y, gyro_angle_z; // 자이로 회전각
float filtered_angle_x, filtered_angle_y, filtered_angle_z; // 최종 보정각
float baseAcX, baseAcY, baseAcZ;  // 가속도 평균값 저장 변수
float baseGyX, baseGyY, baseGyZ;  // 자이로 평균값 저장 변수
float dt; // 측정시간 주기
unsigned long t_now;  // 현재 측정 주기 시간
unsigned long t_prev; // 이전 측정 주기 시간

SoftwareSerial BTSerial(8, 9);
String str="M";
char out[30]="";

///////////////////////////// Main - Start ////////////////////////////////////
void setup() {
  initMPU6050(); // MPU-6050 센서에 대한 초기 설정 함수
  BTSerial.begin(9600);
  calibAccelGyro(); // 센서 보정
  initDT(); // 시간 간격에 대한 초기화 -> 현재 시각 저장, 전원이 ON 되면 그떄부터 측정 시작
}

void loop() {
  readAccelGyro(); // 가속도, 자이로 센서 값 읽어드림
  calcDT(); // 측정 주기 시간 계산
  calcAccelYPR(); // 가속도 센서 처리 루틴
  calcGyroYPR(); // 자이로 센서 처리 루틴
  calcFilteredYPR(); // 상보 필터 적용                

  static int cnt;
  cnt++;
  if(cnt%40 == 0)
    makeStr();
}
///////////////////////////// Main - End ///////////////////////////////////////


////////////////////////// Functions - Start ///////////////////////////////////
// MPU-6050 센서에 대한 초기 설정 함수 (셋업)
void initMPU6050(){
  Wire.begin(); //I2C 통신 시작 알림
  Wire.beginTransmission(MPU_addr); //0x68번지 값을 가지는 MPU-6050과 I2C 통신
  Wire.write(0x6B);
  Wire.write(0); // 잠자는 MPU-6050을 깨움
  Wire.endTransmission(true); //I2C 버스 제어권 중단
}

// 가속도 , 자이로 원시값을 읽는 함수
void readAccelGyro(){
  Wire.beginTransmission(MPU_addr); //0 x68번지 값을 가지는 MPU-6050과 I2C 통신 시작
  Wire.write(0x3B); // 0x3B번지에 저장
  Wire.endTransmission(false); // 데이터 전송 후 재시작 메새지 전송(연결은 계속 지속)
  Wire.requestFrom(MPU_addr, 14, true); // 0x68 번지에 0x3B 부터 48까지 총 14바이트 저장
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

// 오차보정을 위한 평균값 구하기 (셋업)
void calibAccelGyro(){
  float sumAcX = 0, sumAcY = 0, sumAcZ = 0;
  float sumGyX = 0, sumGyY = 0, sumGyZ = 0;

  readAccelGyro(); //가속도 자이로 센서 읽어들임

  //평균값 구하기
  for(int i=0; i<10; i++){
    readAccelGyro();
    sumAcX += AcX; sumAcY += AcY; sumAcZ += AcZ;
    sumGyX += GyX; sumGyY += GyY; sumGyZ += GyZ;
    delay(100);
  }
  baseAcX = sumAcX / 10; baseAcY = sumAcY / 10; baseAcZ = sumAcZ / 10;
  baseGyX = sumGyX / 10; baseGyY = sumGyY / 10; baseGyZ = sumGyZ / 10;
}

// 주기를 구하기 위해 최초값 지정 함수 (셋업) 
void initDT(){
  t_prev = millis();
}

// 주기 계산 함수
void calcDT(){
  t_now = millis();
  dt = (t_now - t_prev) / 1000.0;
  t_prev = t_now;
}

// 가속도로부터 회전각 계산 함수
void calcAccelYPR(){
  float accel_x, accel_y, accel_z; //가속도 센서의 보정값
  float accel_xz, accel_yz;
  const float RADIANS_TO_DEGREES = 180/3.14159;

  accel_x = AcX - baseAcX; // 가속도(직선) X축에 대한 현재 값 - 가속도 센서의 평균값
  accel_y = AcY - baseAcY;
  accel_z = AcZ + (16384 - baseAcZ);

  //직석 +X축이 기울어진 각도 구함
  accel_yz = sqrt(pow(accel_y, 2) + pow(accel_z, 2));
  accel_angle_y = atan(-accel_x / accel_yz)*RADIANS_TO_DEGREES;

  accel_xz = sqrt(pow(accel_x, 2) + pow(accel_z, 2));
  accel_angle_x = atan(accel_y / accel_xz)*RADIANS_TO_DEGREES;

  accel_angle_z = 0;
}

// 자이로로부터 회전각 계산 함수
void calcGyroYPR(){
  const float GYROXYZ_TO_DEGREES_PER_SED = 131; // 131 : 1초동안 1도 돌때 자이로 값이 131이다.

  gyro_x = (GyX - baseGyX) / GYROXYZ_TO_DEGREES_PER_SED;
  gyro_y = (GyY - baseGyY) / GYROXYZ_TO_DEGREES_PER_SED;
  gyro_z = (GyZ - baseGyZ) / GYROXYZ_TO_DEGREES_PER_SED;

  gyro_angle_x += gyro_x * dt; //변화된 각 : 각속도 x 측정 주기 시간
  gyro_angle_y += gyro_y * dt;
  gyro_angle_z += gyro_z * dt;
}

// 상보필터 적용 함수
void calcFilteredYPR(){
  const float ALPHA = 0.96;
  float tmp_angle_x, tmp_angle_y, tmp_angle_z;  //이전 필터 각도(prev)

  tmp_angle_x = filtered_angle_x + gyro_x * dt; //자이로 각도 = 각속도 x 센서 입력 주기
                                                //각속도 = 단위시간당 회전한 각도 -> 회전한 각도 / 단위시간
  tmp_angle_y = filtered_angle_y + gyro_y * dt;
  tmp_angle_z = filtered_angle_z + gyro_z * dt;

  filtered_angle_x = ALPHA * tmp_angle_x + (1.0 - ALPHA) * accel_angle_x;
  filtered_angle_y = ALPHA * tmp_angle_y + (1.0 - ALPHA) * accel_angle_y;
  filtered_angle_z = tmp_angle_z; //곡선 +Z 축은 자이로 센서만 이용해서 나타내고 있음(가속도 센서는 안함)
}

// 데이터 출력
void makeStr(){
  str+=String((int)filtered_angle_x);
  str+=String('E');
  str+=String((int)filtered_angle_y);
  str+=String('E');
  str+=String((int)filtered_angle_z);
  str+=String('E');
  str.toCharArray(out, str.length()+1);
  BTSerial.write(out);
  /*BTSerial.print(filtered_angle_x);
  BTSerial.print('\t');
  BTSerial.print(filtered_angle_y);
  BTSerial.print('\t');
  BTSerial.print(filtered_angle_z);
  BTSerial.print('\t');
  BTSerial.print(dt);
  BTSerial.println(" ");*/
  str="M";
  strcpy(out, "");
}
////////////////////////// Functions - End //////////////////////////////////////
