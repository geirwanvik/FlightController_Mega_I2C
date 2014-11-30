#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "Arduino.h"
#define PRIVATE static
#define PUBLIC

PUBLIC void UpdateCRC(uint8_t data);
PUBLIC void ClearCRC(void);
PUBLIC uint16_t GetCRC(void);



#endif // CHECKSUM_H
