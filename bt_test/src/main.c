#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP;

#include <stdint.h>
#include <stdio.h>
#include "hci.h"
#include "run_loop.h"
#include "config.h"
#include "btstack_memory.h"
#include "bt_control_cc256x.h"
#include "LPC17xx.h"

static void packet_handler(uint8_t packet_type, uint8_t *packet, uint16_t size) {
  static bd_addr_t addr;

  switch (packet_type) {

  case HCI_EVENT_PACKET:
    switch (packet[0]) {

    case BTSTACK_EVENT_STATE:
      // bt stack activated, get started - set local name
      if (packet[2] == HCI_STATE_WORKING) {
        printf("working\n");
        //hci_send_cmd( &hci_read_bd_addr);
        hci_send_cmd( &hci_le_set_advertising_parameters, 0x0800, 0x0800, 2, 0,0, addr, 1,0);
      }
      break;
    case HCI_EVENT_COMMAND_COMPLETE:
      if (COMMAND_COMPLETE_EVENT(packet, hci_read_bd_addr)) {
        bt_flip_addr(addr, &packet[6]);
        printf("BD ADDR: %s\n", bd_addr_to_str(addr));

        break;
      }

      if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertising_parameters)){
            /* code */
        printf("parameters set \n");
         hci_send_cmd( &hci_le_read_supported_features);
            break;
      
      }
      
      if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertising_data)){
            /* code */
            break;
      
      }
      
      if (COMMAND_COMPLETE_EVENT(packet, hci_le_set_advertise_enable)){
            /* we will be checking if it was a command to start or to end
              advertising*/
            break;
      
      }
      if(COMMAND_COMPLETE_EVENT(packet, hci_le_read_supported_features)){

            break; 
      }
    }
  }
}


int main(void){
  volatile int delay;
  LPC_GPIO0->FIODIR |= (1<<22);
  LPC_GPIO0->FIOCLR |= (1<<22);
  for (delay = 0; delay <1000000 ; ++delay )
  {
      LPC_GPIO0->FIOCLR |= (1<<22);
  }
  LPC_GPIO0->FIOSET |= (1<<22); //19

  run_loop_init(RUN_LOOP_EMBEDDED);


  hci_transport_t * transport = hci_transport_h4_dma_instance();
  bt_control_t       * control   = bt_control_cc256x_instance();
  hci_uart_config_t  * config    = hci_uart_config_cc256x_instance();
  hci_init(transport, config, control, NULL);
  hci_power_control(HCI_POWER_ON);
  hci_register_packet_handler(packet_handler);
  run_loop_execute();

  return 0;
}