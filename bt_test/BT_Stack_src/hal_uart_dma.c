/*uart over dma*/

#include "lpc17xx_uart.h"
#include "hal_uart_dma.h"

// uart configure struct
static UART_CFG_Type UART_CFG_1152;
static UART_FIFO_CFG_Type FIFO_DEFAULT;

void dummy_handler(void){};

// rx state
static uint16_t  bytes_to_read = 0;
static uint8_t * rx_buffer_ptr = 0;

// tx state

static uint8_t * tx_buffer_ptr = 0;

// handlers
static void (*rx_done_handler)(void) = dummy_handler;
static void (*tx_done_handler)(void) = dummy_handler;
//static void (*cts_irq_handler)(void) = dummy_handler;


static void hal_uart_dma_enable_rx(void){
	UART_FullModemForcePinState((LPC_UART1_TypeDef *)LPC_UART1, UART1_MODEM_PIN_RTS, ACTIVE);
}

static void hal_uart_dma_disable_rx(void){
	UART_FullModemForcePinState((LPC_UART1_TypeDef *) LPC_UART1, UART1_MODEM_PIN_RTS, INACTIVE);
}

void hal_uart_dma_init(void){
	//setup uart configure struct
	UART_ConfigStructInit(&UART_CFG_1152, 115200);

	//initialize Uart1
	UART_Init((LPC_UART_TypeDef *)LPC_UART1, &UART_CFG_1152);

	//setup fifo
	UART_FIFOConfigStructInit(&FIFO_DEFAULT);
	UART_FIFOConfig((LPC_UART_TypeDef *) LPC_UART1, &FIFO_DEFAULT);

	NVIC_EnableIRQ(UART1_IRQn);

/*	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RLS, ENABLE);*/
		

}

/* this funtion is not implemented yet because i am not sure if
 * is it safe to change baudrates on the fly
 */
int hal_uart_dma_set_baud(uint32_t baud){
	return 1;
}

void hal_uart_dma_set_block_received( void (*the_block_handler)(void)){
    rx_done_handler = the_block_handler;
}

void hal_uart_dma_set_block_sent( void (*the_block_handler)(void)){
    tx_done_handler = the_block_handler;
}

void hal_uart_dma_shutdown(void){
	UART_DeInit((LPC_UART_TypeDef*) LPC_UART1);
}


void hal_uart_dma_send_block( uint8_t * data, uint16_t len){

	UART_TxCmd((LPC_UART_TypeDef *) LPC_UART1, ENABLE);
	tx_buffer_ptr = data;
	UART_Send((LPC_UART_TypeDef *) LPC_UART1, tx_buffer_ptr, len, BLOCKING);
    (*tx_done_handler)();
}

void hal_uart_dma_receive_block(uint8_t *buffer, uint16_t len){

	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, DISABLE);

	rx_buffer_ptr = buffer;
	bytes_to_read = len;

	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);

	hal_uart_dma_enable_rx();

	//enable rx with rts
}


void UART1_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART1 ->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue <<= 1; /*preserve first bits place*/
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */

	if (IIRValue == UART_IIR_INTID_RLS ) /* Receive Line Status */
	{
		LSRValue = LPC_UART1 ->LSR; /* Receive Line Status */

		/*If the cause of the interrupt was an error exit interrupt routine*/
		if (LSRValue & (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_RXFE | UART_LSR_BI )) {
			/* There are errors or break interrupt */
			/* Read LSR will clear the interrupt */
			//UART1Status = LSRValue;
			Dummy = LPC_UART1 ->RBR; /* Dummy read on RX to clear interrupt, then bail out */
			return;
		}
		if (LSRValue & UART_LSR_RDR ) /* Receive Data Ready */
		{
			/* If no error on RLS, normal ready, save into the data buffer. */
			/* Note: read RBR will clear the interrupt */


			if(bytes_to_read == 0){
				hal_uart_dma_disable_rx();
				UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, DISABLE);
				return;

			}
			*rx_buffer_ptr = UART_ReceiveByte((LPC_UART_TypeDef *) LPC_UART1);
			++rx_buffer_ptr;
			--bytes_to_read;
			if(bytes_to_read > 0){
				return;
			}
			hal_uart_dma_disable_rx();
			UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, DISABLE);

			(*rx_done_handler)();

		}
	} else if (IIRValue == UART_IIR_INTID_RDA ) /* Receive Data Available */
	{
		/* Receive Data Available */

		if(bytes_to_read == 0){
			hal_uart_dma_disable_rx();
			UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, DISABLE);
			return;

		}
		*rx_buffer_ptr = UART_ReceiveByte((LPC_UART_TypeDef *) LPC_UART1);
		++rx_buffer_ptr;
		--bytes_to_read;
		if(bytes_to_read > 0){
			return;
		}
		hal_uart_dma_disable_rx();
		UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, DISABLE);

		(*rx_done_handler)();

	}
}



