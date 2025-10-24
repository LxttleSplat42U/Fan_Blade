# Fan_Blade

Code used for the Fan blade ESP32 to control the holographic fan LEDs (Please see the source repository for more information https://github.com/LxttleSplat42U/Holo3D_Base?tab=readme-ov-file).

Before flashing the ESP32s please ensure that the FAN_ID global variable in the SRC->HardwareSoftwareConfig.cpp file has a unique ID number with a trailing 1 in the (format X1 where "X" is the fan number, e.g. for the thrid fan FAN_ID = 31)

If other hardware or software configurations are required the SRC->HardwareSoftwareConfig.cpp file can be changed as well
