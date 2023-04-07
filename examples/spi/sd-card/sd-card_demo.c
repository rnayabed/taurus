#include "stdlib.h"
#include "spi.h"
#include "sd.h"

#define DTB_START_SECTOR 2048
#define DTB_SIZE 1024 // 512 KB
#define DTB_END_SECTOR (DTB_START_SECTOR + DTB_SIZE)
#define DTB_ADDR 0x87000000

#define LINUX_START_SECTOR DTB_END_SECTOR
#define LINUX_SIZE 20480 // 10 MB //40960 // 20 MB
#define LINUX_END_SECTOR (LINUX_START_SECTOR + LINUX_SIZE)
#define LINUX_ADDR 0x80000000

unsigned long g_program_entry; //to preserve GPR a0
unsigned long g_dtb_address;


void start_boot(unsigned long entry, unsigned long dtb_address)
{
	g_program_entry = entry; //to preserve GPR a0
	g_dtb_address = dtb_address;
	void (*fuc_ptr)();
	__asm__ __volatile__("csrr 	a0,mhartid");
	//HART ID
	__asm__ __volatile__("la a2, g_dtb_address");
	__asm__ __volatile__("ld a1, 0(a2)");
	fuc_ptr = (void (*)())(g_program_entry);
	__asm__ __volatile__("fence");
	__asm__ __volatile__("fence.i");
	__asm__ __volatile__("nop");
	__asm__ __volatile__("nop");
	//__asm__ __volatile__ ("fence.i");
	//*tft_base = FB_BASE;
	//*tft_cr = 0x1;
	//core_flag = 0;
	__asm__ __volatile__("fence");
	__asm__ __volatile__("fence.i");

	fuc_ptr();
}

void main()
{
	u_int8_t *buf_ptr;

	printf("SD-Card Test !\n");
	if (sdInit(MDP_SPI_0))
	{
		printf("Failed to initialize SD-Card !\n");
		waitTillDie();
	}
	/*
	printf("Copying device tree binary...\n");
	buf_ptr = (u_int8_t *)(DTB_ADDR);
	for (int i = DTB_START_SECTOR; i < DTB_END_SECTOR; i++)
	{
		readBlock(i, buf_ptr);
		buf_ptr += BYTES_512;
	}
	//readMultiBlock(0x0, DTB_BLOCKS, (u_int8_t *) DTB_ADDR);
	buf_ptr = (u_int8_t *)(DTB_ADDR);
	printf("Sample data :  %02x\n", *buf_ptr);
	printf("Copying Linux....\n");
	//readMultiBlock(DTB_BLOCKS, LINUX_BLOCKS, (u_int8_t *) LINUX_ADDR);
	//readMultiBlock(DTB_BLOCKS, DTB_BLOCKS, (u_int8_t *) LINUX_ADDR);
	buf_ptr = (u_int8_t *)(LINUX_ADDR);
	for (int i = LINUX_START_SECTOR; i < LINUX_END_SECTOR; i++)
	{
		readBlock(i, buf_ptr);
		buf_ptr += BYTES_512;
	}
	buf_ptr = (u_int8_t *)(LINUX_ADDR);
	printf("Sample data :  %02x\n", *buf_ptr);
	//readBlock(DTB_BLOCKS, (u_int8_t *) DTB_ADDR);
	printf("Starting Linux...\n");
	*/
	/*	
	u_int32_t *mem = (u_int32_t *)LINUX_ADDR;
	for (int i = 0; i < 1415; i++)
	{
		u_int32_t d1 = *mem;
		mem++;
		u_int32_t d2 = *mem;
		mem++;
		u_int32_t d3 = *mem;
		mem++;
		u_int32_t d4 = *mem;
		mem++;
		printf("%08x%08x%08x%08x\n", d4, d3, d2, d1);
	}
*/
	//start_boot(LINUX_ADDR, DTB_ADDR);
	waitTillDie();
}
