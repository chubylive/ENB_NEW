/*
 ===============================================================================
 Name        : main.c
 Author      :
 Version     :
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "max_lib.h"
#include "main.h"
#include "UMDLPC/system/clocking.h"
 #include <stdio.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP;

int main(void) {
  // Select 12MHz crystal oscillator
  //LPC_SC ->CLKSRCSEL = 1;

  // Bypass PLL 0
  //PLL_bypass();

	init_max31();

  while (1) {
	printf("temp is %d degrees Cel\n", read_max31(Cel));
  }
  return 0;
}
