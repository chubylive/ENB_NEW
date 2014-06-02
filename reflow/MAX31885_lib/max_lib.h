#ifndef __MAX31_h_
#define __MAX31_h_

#include "spi.h"

//#include ""

typedef struct
{
	int16_t celcius;
	int16_t farenheit;
	uint8_t data[4];
}max_info;

typedef enum  { Cel, Far } Unit;

void init_max31();
void dinit_max31();
int16_t read_max31(Unit un);


#endif