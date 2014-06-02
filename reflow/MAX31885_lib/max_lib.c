#include "max_lib.h"
#include <stdio.h>
#include <stdint.h>

max_info mf;

void init_max31(){
	//SSP_ConfigStructInit(&max_ssp);
	//SSP_Init((LPC_SSP_TypeDef *) LPC_SSP0, &max_ssp);
	spi_init();

}


int16_t read_max31(Unit un){
	
	//SSP_ReadWrite((LPC_SSP_TypeDef *) LPC_SSP0, &max_data, SSP_TRANSFER_POLLING);
	spi_txrx(NULL, &mf.data,4);
	// parse the data 
	
	//tmp[0] = SSP_ReceiveData(LPC_SSP0);
	//tmp[1] = SSP_ReceiveData(LPC_SSP0);
	//mf.data = tmp;
	//mf.data
	// if (mf.data & 0x7)
	// {
	// 	return 0;
	// }

	// if (un == Cel){
	// 	mf.celcius = (mf.data >> 18)/4;
	// 	return mf.celcius;
	// }else if (un == Far){
	// 	mf.farenheit = (((mf.data >> 18)/4)*9/5) + 32;
	// 	return mf.farenheit;
	// }
	printf("%s\n",mf.data );
	return 0;
}