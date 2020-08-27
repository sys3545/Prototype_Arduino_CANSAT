# Prototype_Arduino_CANSAT
- Prototype of CANSAT which will be launched on August 2021.
- Prototype of CANSAT which will be exhibited during Capstone Event on January 2021.

# Caution
'sketchs' must be executed with 'libraries' folder of Arduino.

# Missions

Satellite
- Turn on its LED when the spread command comes in.
- Read illumination value and send it to Ground-Control.

Ground-Control
- Connect with Satellite by using Bluetooth or Wifi.
- Command Satellite to spread its wing.
- Command Satellite to rotate its wing.
- Print the illumination value transmitted from Satellite.
- Command Satellite to fold its wing stably.
  
# Version
Released version 0.0.0 (2020 08 01) :   
Base file was uploaded      
Released version 0.0.1 (2020 08 03) :   
Test Serial Communication with Arduino and Ground_Control by using USB port (COM3)      
Released version 0.0.2 (2020 08 08) :    
Function added, Bluetooth Serial Communication! We can do this with wireless (COM5)   
Released version 0.0.3 (2020 08 14) :    
MFC Oscilloscope function was added, but it's just a test-version yet  
Released version 0.0.4 (2020 08 19) :    
Complete comm department, it can show the values from Oscilloscope!
