/**
 @file interrupt.c
 @brief Contains routines for Iinterrupt handler, IRQ enable functions
 @detail Includes software functions to enable, handling functions for intterrupt

 @authors Debayan Sutradhar (@rnayabed), Avra Mitra (@abhra0897)
 */

#include <stdlib.h>
#include <interrupt.h>
#include <timer.h>
#include <config.h>
#include <encoding.h>



#if __riscv_xlen == 64
#define MAXIMUM_INTR_COUNT 64
#else
#define MAXIMUM_INTR_COUNT 32
#endif

#define read_const_csr(reg) ({ unsigned long __tmp; \
  asm ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })


extern volatile unsigned long INTERRUPT_Handler_0;
extern volatile unsigned long trap_entry;

void (*irq_table[MAXIMUM_INTR_COUNT])(void);
void (*sw_irq_function)(void);


/** @fn    enable_irq
  @brief   Enable processor interrupt and Global interrupt in machine mode.
  @details The function "set_csr" will set the external interrupt bit in MIE register and Machine mode interrupt in MSTATUS register.

  @param[in]  No input parameter.
  @param[Out] No output parameter.
*/
void enable_irq(void) {
    set_csr(mie, MIP_MEIP);			// Set MEIE bit in MIE register for Machine External Intr.
    set_csr(mstatus, MSTATUS_MIE);		// Set global machine intr bit (3rd bit) in MSTATUS register.
#if __riscv_xlen == 64
	//For 64 Bit processors		
    write_csr(mtvec,(UL)&INTERRUPT_Handler_0);		// Set MTVEC register with vector address.
#else
    //For 32 Bit processors
    write_csr(mtvec,(UI)&INTERRUPT_Handler_0);		// Set MTVEC register with vector address.
#endif
}

/** @fn    enable_sw_irq

  @brief   Enable processor interrupt and Global interrupt in machine mode.
  @details The function "set_csr" will set the software interrupt bit in MIE register and Machine mode interrupt in MSTATUS register.

  @param[in]  No input parameter.
  @param[Out] No output parameter.
*/
void enable_sw_irq(void) {
#if __riscv_xlen == 64
	//For 64 Bit processors		
    write_csr(mtvec,(UL)&INTERRUPT_Handler_0);		// Set MTVEC register with vector address.
    
#else
	//For32 Bit processors
    write_csr(mtvec,(UI)&INTERRUPT_Handler_0);		// Set MTVEC register with vector address.
    
#endif
    set_csr(mie, MIP_MSIP); // Set MSIE bit in MIE register for Machine SW intr.
    set_csr(mstatus, MSTATUS_MIE); // Set global machine intr bit (3rd bit) in MSTATUS register.
    clear_csr(mideleg, MIP_MSIP);
   
}


/** @fn    disable_sw_irq

  @brief   Disable machine software interrupt in MIE register
  @details 

  @param[in]  No input parameter.
  @param[Out] No output parameter.
*/
void disable_sw_irq(void) {

  clear_csr(mie, MIP_MSIP);	// Clear MSIP bit in MIE register for Machine 
   
}


/** @fn void interrupt_enable(UC intr_number)
  @brief  Enable interrupt in controller.
  @details The interrupt enable register is set with the enable value of peripheral selected.
 
  @param[in] unsigned char intr_number: The number at which the periphral will interrupt
  @param[Out] No output parameter.
*/

void interrupt_enable(UC intr_number)
{
	enable_irq();	// Enable global interrupt and external interrupt of the processor.
	PLIC_INTR_REGS.INTR_EN |= ((UL)1 << intr_number);	// Enable interrupt for peripheral in interrupt controller.
	__asm__ __volatile__ ("fence");	
}

 
/** @fn void irq_register_handler(UC irq_no, void (*irq_handler)())
 @brief  Initialise peripheral interrupt handler to a table.
 @details Enable processor interrupt and Global interrupt in machine mode  and init irq_table.
 @warning 
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void irq_register_handler(UC irq_no, void (*irq_handler)()){
    //*irq_handler is function pointer to user defined intr handler
    irq_table[irq_no] = irq_handler;
}

/** @fn void sw_irq_register_handler(UC irq_no, void (*irq_handler)())
 @brief  Initialise peripheral interrupt handler to a table.
 @details Enable processor interrupt and Global interrupt in machine mode  and init irq_table.
 @warning 
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void sw_irq_register_handler( void (*irq_handler)()){
    //*irq_handler is function pointer to user defined intr handler
    sw_irq_function = irq_handler;
}


/** @fn interrupt_handler
 @brief  Invoke the peripheral interrupt handler.
 @details The interrupt controller's status register is read to identify the interrupted peripheral and then the corresponding
	  handler for the peripheral is invoked.
 @warning 
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/
void interrupt_handler(void){
	void (*func_ptr)();
	int mcause_val = 0, trap_type=0;

#if __riscv_xlen == 64
    trap_type = (read_csr(mcause) >> 63);
#else
    trap_type = (read_csr(mcause) >> 31);
#endif

    if(trap_type)
    { //Interrupt
		mcause_val = ((read_csr(mcause) << 1)>>1);

        if(mcause_val == 3)
        {
            // Machine software interrupt
			#if __riscv_xlen == 64
              PLIC_SW_INTR_EN = 0;      //OFF sw intr
			#else
			  clear_csr(mip, MIP_MSIP);	// Clear MSIP bit in MIP register for Machine 
			#endif  
			
			sw_irq_function(); // Invoke the peripheral handler as function pointer.	
        }
        else
        {
            UL intr_status = PLIC_INTR_REGS.INTR_STATUS;

            for(UL i = 0; i < MAXIMUM_INTR_COUNT ; i++)
			{
                if ((intr_status >> i) & 1){
                    irq_table[i]();// Invoke the peripheral handler as function pointer.
				}
			}
		}
	}
	else
	{ //Exception
		func_ptr = (void (*)()) (&trap_entry); //jump to exception handler
		func_ptr();
	}	
}

/** @fn generate_sw_irq
 @brief  Generates machine mode software interrupt if already enabled
 @details 
 @warning 
 @param[in] No input parameter.
 @param[Out] No output parameter.
*/


void generate_sw_irq(void){

#if __riscv_xlen == 64
  PLIC_SW_INTR_EN = 1; //ON sw intr
#else
  set_csr(mip, MIP_MSIP); // Set MSIP bit in MIP register for Machine SW intr.
#endif  	

}


