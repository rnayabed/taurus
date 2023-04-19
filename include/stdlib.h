/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Nov-2019
 Filename		: stdlib.h
 Purpose		: Generic library functions
 Description		: Provides generic library functions like 
			  printf/memcpy etc 
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/
/**
 @file stdlib.h
 @brief header file for standard library functions
 @detail 
 */

#ifndef INCLUDE_STDLIB_H_
#define INCLUDE_STDLIB_H_

#include <time.h>


typedef unsigned char  UC;	//1 Byte
typedef unsigned int   UI;	//4 Bytes
typedef unsigned long  UL;	//4 Bytes
typedef unsigned short US;	//2 Bytes

/* End of file character.
   Some things throughout the library rely on this being -1.  */
#ifndef EOF
# define EOF (-1)
#endif

int printf(const char* fmt, ...);
int sprintf(char * buffer,const char *fmt, ...);
int putchar(int ch);
int puts(const char *p);
int delay(unsigned int count);
int udelay(unsigned int count);
//void usleep(unsigned int microsec);
int _usleep(unsigned int microsec);
void usleep(unsigned long delay);
int atoi(char *s);
float atof(char* s);
clock_t get_time();
void *_sbrk(int incr);

#endif /* INCLUDE_STDLIB_H_ */
