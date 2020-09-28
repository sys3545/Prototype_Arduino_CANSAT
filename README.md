# Prototype_Arduino_CANSAT
- Prototype of CANSAT which will be launched on August 2021.
- Prototype of CANSAT which will be exhibited during Capstone Event on January 2021.

# Caution
'sketchs' must be executed with 'libraries' folder of Arduino.

# Missions

Satellite
- Read Pitch, Roll, Yaw(PRY) value and send it to Ground-Control.

Ground-Control
- Connect with Satellite by using Bluetooth or Wifi.
- Print the PRY value transmitted from Satellite.
- Draw the position of CANSAT using PRY value.
  
# Version
Released version 0.0.0 (2020 08 01) :   
Base file was uploaded      
   
Released version 0.0.1 (2020 08 03) :   
Test Serial Communication with Arduino and Ground_Control by using USB port (COM3)      
   
Released version 0.0.2 (2020 08 08) :    
Function added, Bluetooth Serial Communication! We can do this with wireless (COM5)   
   
Released version 0.0.3 (2020 08 14) :    
MFC Oscilloscope function was added, but it's just a test-version yet.     
   
Released version 0.0.4 (2020 08 19) :    
Complete comm department, it can show the values from Oscilloscope!
   
Released version 0.1.0 (2020 08 28) :   
RealTime postion is printed in OpenGL from oscilloscope value! openGL confirm.
   
Released version 0.1.1 (2020 09 01) :   
3D-Cube, parachute are printed in openGL replecting LDR value.   
   
Released version 0.1.2 (2020 09 09) :   
Position of cansat is printed in openGL replecting pitch, roll, yaw. it's first prototype version!  
   
Released version 0.1.3 (2020 09 15) :   
Renovation of .ino, the position of cansat is printed precisely more than before.   
   
Released version 0.1.4 (2020 09 28) :   
Each of P, Y, R is printed.   
  


