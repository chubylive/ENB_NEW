#include <stdlib.h>

#include "LPC17xx.h"
#include "lpc_types.h"
#include "spi.h"
#include <stdint.h>

#define _BV(n) (1<<(n))
void spi_init(){
	int i;
	//power on ssp0 (on reset this bit is set to 1)
	LPC_SC->PCONP |= _BV(21);
	//setup clock bits in the pclksel register( no divider)
	LPC_SC->PCLKSEL1 |= _BV(10);
	//setup pins for miso0(PO.17) mosi0(P0.18) Sck0(P0.15) SSel(PO.16)
	LPC_PINCON->PINSEL0 &= ~(0x3UL<<30);
  	LPC_PINCON->PINSEL0 |= (0x2UL<<30);
  	LPC_PINCON->PINSEL1 &= ~((0x3<<0)|(0x3<<2)|(0x3<<4));
  	LPC_PINCON->PINSEL1 |= ((0x2<<0)|(0x2<<2)|(0x2<<4));

	 /* Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15 */
  	LPC_SSP0->CR0 |=  0b0111;
	// drop clock rate from 12mhz to 150khz
	LPC_SSP0->CPSR = 8;
	//master mode and enable ssp0
	LPC_SSP0->CR1 |= _BV(1);


}

void spi_txrx(uint8_t* tx, uint8_t* rx, uint16_t len)
{
	/* Embed: transmit and receive len bytes
	 * Remember:
	 *   SPI transmits and receives at the same time
	 *   If tx == NULL and you are only receiving then transmit all 0xFF
	 *   If rx == NULL and you are only transmitting then dump all recieved bytes
	 */
	 uint8_t dummy, idx ;
	 for( idx = 0; idx < len; idx++){
	 	if(tx == NULL && rx == NULL){
	 		//while not busy
	 		 while ( LPC_SSP0->SR & SSPSR_BSY );
	 		dummy = LPC_SSP0->DR;
	 		 while ( LPC_SSP0->SR & SSPSR_BSY );
	 		 LPC_SSP0->DR = 0xFF;


	 	}
	 	else if(tx != NULL && rx == NULL){
	 		/*according to manual (software can write data to be sent in a future frame to this
			register whenever the TNF bit in the Status register is 1)*/
	 		while ( (LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
	 		LPC_SSP0->DR = *tx;
	 		tx++;
	 		 while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
	 			  /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
	 			  on MISO. Otherwise, when SSP0Receive() is called, previous data byte
	 			  is left in the FIFO. */
	 			  dummy = LPC_SSP0->DR;
	 	}
	 	else if(tx == NULL && rx != NULL){
	 		LPC_SSP0->DR = 0xFF;
	 		/* Wait until the Busy bit is cleared */
			while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
	 		/*according to manual (software can read data from this register whenever the RNE bit
			in the Status register is 1, indicating that the Rx FIFO is not empty.)*/

	 		*rx++ =LPC_SSP0->DR;


	 	}
	 }
}

uint8_t spi_tx(uint8_t tx){
	uint8_t dummy;
	while ( (LPC_SSP0->SR & (SSPSR_TNF|SSPSR_BSY)) != SSPSR_TNF );
		 		LPC_SSP0->DR = tx;
	 while ( (LPC_SSP0->SR & (SSPSR_BSY|SSPSR_RNE)) != SSPSR_RNE );
	 /* Whenever a byte is written, MISO FIFO counter increments, Clear FIFO
		on MISO. Otherwise, when SSP0Receive() is called, previous data byte
		is left in the FIFO. */
	dummy = LPC_SSP0->DR;
	return dummy;

}
