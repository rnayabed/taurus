/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Nov-2019
 Filename		: mbl.lds
 Purpose		: Linker descriptor
 Description		: Linker descriptor
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/

SECTIONS
{

  /* . = 0x30000; 
   .INTERRUPT_isr_sect_0 : { *(.INTERRUPT_isr_sect_0) }*/

  /* text: test code section */
  . = 0x200000;/*SOC ROM start address*/
  .text.init : { *(.text.init) }

  .text : 
  { 
  		*(.text)
  }  
  /*.INTERRUPT_isr_sect_0 : { *(.INTERRUPT_isr_sect_0) }*/
  /*. = 0x20000;*//*SOC ROM start address*/
  /* data segment */
  .data : { *(.data) }

  .sdata : {
    __global_pointer$ = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
  *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

  /* bss segment */
  _bss_start = .;
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss : { *(.bss) }
   _bss_end = .;

  /* thread-local data segment */
  .tdata :
  {
    _tls_data = .;
    *(.tdata.begin)
    *(.tdata)
    *(.tdata.end)
  }
  .tbss :
  {
    *(.tbss)
    *(.tbss.end)
  }

  /* End of uninitalized data segement */
  _end = .;
}
