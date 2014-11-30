/*
* RC.cpp
*
* Created: 14.11.2014 08:28:02
* Author: GeirW
*/


#include "RC.h"

rc_t rc;

static uint8_t rcChannel[NUM_RC_CHANS] = { PITCH,ROLL,THROTTLE,YAW,AUX1,AUX2,AUX3,AUX4 };
volatile uint16_t rcValue[NUM_RC_CHANS] = {1502, 1502, 1502, 1502, 1502, 1502, 1502, 1502}; // interval [1000;2000]
int16_t rcData[NUM_RC_CHANS];    // interval [1000;2000]

void InitRC()
{
	//configure THROTTLE PIN (A8 pin) as input witch pullup and enabled PCINT interrupt
	DDRK &= ~(1<<0); PORTK |= (1<<0);  PCICR |= (1<<2); PCMSK2 |= (1<<0);
}

 void rxInt(void)
 {
	  uint16_t now,diff;
	  static uint16_t last = 0;
	  static uint8_t chan = 0;
	  now = micros();
	  sei();
	  diff = now - last;
	  last = now;
	  if(diff>3000) chan = 0;
	  else 
	  {
		  if(900<diff && diff<2200 && chan<NUM_RC_CHANS )
		  {   
			  rcValue[chan] = diff;
		  }
		  chan++;
	  }
 }
 
 uint16_t readRawRC(uint8_t chan) {
	 uint16_t data;
	 uint8_t oldSREG;
	 oldSREG = SREG; cli(); // Let's disable interrupts
	 data = rcValue[chan]; // Let's copy the data Atomically
	 SREG = oldSREG;        // Let's restore interrupt state
	 return data; // We return the value correctly copied when the IRQ's where disabled
 }

void ComputeRC(void)
{
	static uint16_t rcData4Values[NUM_RC_CHANS][4], rcDataMean[NUM_RC_CHANS];
	static uint8_t rc4ValuesIndex = 0;
	uint8_t chan,a;

	rc4ValuesIndex++;
	if (rc4ValuesIndex == 4) rc4ValuesIndex = 0;
	for (chan = 0; chan < NUM_RC_CHANS; chan++)
	{
		rcData4Values[chan][rc4ValuesIndex] = readRawRC(chan);
		rcDataMean[chan] = 0;
		for (a=0;a<4;a++) rcDataMean[chan] += rcData4Values[chan][a];
		
		rcDataMean[chan]= (rcDataMean[chan]+2)>>2;
		if ( rcDataMean[chan] < (uint16_t)rcData[chan] -3)
		{
			rcData[chan] = rcDataMean[chan]+2;
		}
		if ( rcDataMean[chan] > (uint16_t)rcData[chan] +3)
		{
			rcData[chan] = rcDataMean[chan]-2;
		}
	}
	rc.pitch = rcData[PITCH];
	rc.roll = rcData[ROLL];
	rc.yaw = rcData[THROTTLE];
	rc.throttle = rcData[YAW];
	rc.aux1 = rcData[AUX1];
	rc.aux2 = rcData[AUX2];
	rc.aux3 = rcData[AUX3];
	rc.aux4 = rcData[AUX4];
}

ISR(PCINT2_vect) { if(PINK & (1<<0)) rxInt(); }