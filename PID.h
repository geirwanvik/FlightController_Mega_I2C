/* 
* PID.h
*
* Created: 14.11.2014 08:27:27
* Author: GeirW
*/


#ifndef __PID_H__
#define __PID_H__

#include "Arduino.h"
#include "CommandLibrary.h"

#define PUBLIC
#define PRIVATE static

PUBLIC void ComputePID(void);
PUBLIC void InitMotors(void);
PRIVATE void WriteMotors(void);

#endif //__PID_H__
