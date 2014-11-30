/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega w/ ATmega2560 (Mega 2560), Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
//

#include "C:\Program Files\Arduino\hardware\arduino\avr\variants\mega\pins_arduino.h" 
#include "C:\Program Files\Arduino\hardware\arduino\avr\cores\arduino\arduino.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\FlightController_Mega_I2C.ino"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Checksum.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Checksum.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\CommandLibrary.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\I2C.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\I2C.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\IMU.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\IMU.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Memory.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Memory.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\PID.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\PID.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Protocol.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Protocol.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\RC.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\RC.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Sensors.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Sensors.h"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Telegram.cpp"
#include "C:\Users\GeirW\Dropbox\Stash\Git Projects\FlightController_Mega_I2C\Telegram.h"
