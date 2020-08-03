# Prototype_Arduino_CANSAT
- Prototype of Arduino CANSAT which will be launched on August 2021.
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
Released version 0.0.0 (2020 08 01)   
- Base file upload
Released version 0.0.1 (2020 08 03)   
- Test Serial with Arduino and Ground_Control by using USB port (COM3)
