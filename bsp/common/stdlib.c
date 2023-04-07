/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Nov-2019
 Filename		: stdlib.c
 Purpose		: Generic library functions
 Description		: Provides generic library functions like 
			  printf/memcpy etc 
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/
/**
 @file stdlib.c
 @brief Contains routines for standard library functions
 @detail 
 */

#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>

#include <include/debug_uart.h>
#include <include/encoding.h>

 //volatile unsigned int loop_count=0;

#define HAS_FLOAT 1

#define NUM_COUNTERS 2
static uintptr_t counters[NUM_COUNTERS];
static char* counter_names[NUM_COUNTERS];

void* memset(void* dest, int byte, size_t len);
void* memcpy(void* dest, const void* src, size_t len);
size_t strnlen(const char *s, size_t n);
#define read_const_csr(reg) ({ unsigned long __tmp; \
  asm ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define static_assert(cond) switch(0) { case 0: case !!(long)(cond): ; }

/** @fn handle_trap
 * @brief Trap handler for programs
 * @details 
 * @warning 
 */
 
uintptr_t __attribute__((weak)) handle_trap(uintptr_t cause, uintptr_t epc, uintptr_t regs[32])
{
	printf("\n\rTRAP\n\r");
	printf("EPC : %lx\n\r",epc);
	printf("Cause : %lx\n\r",cause);
	printf("badaddress: %lx\n\r",read_const_csr(mbadaddr));
	for(int i=0;i<32;i++)
		printf("reg %d : %lx\n\r",i,regs[i]);

	while(1);
	//return 0;
}

/** @fn 
 * @brief 
 * @details 
 */
void exit(int code)
{
  printf("\n\rEXIT\n\r");
  while(1);
}
/** @fn 
 * @brief 
 * @details 
 */
void abort()
{
	 printf("\n\rABORT\n\r");
}
/** @fn 
 * @brief 
 * @details 
 */
void printstr(const char* s)
{
	 printf("\n\rEXIT\n\r");
}
/** @fn 
 * @brief 
 * @details 
 */
void __attribute__((weak)) thread_entry(int cid, int nc)
{
  // multi-threaded programs override this function.
  // for the case of single-threaded programs, only let core 0 proceed.
  while (cid != 0);
}
/** @fn 
 * @brief 
 * @details 
 */
int __attribute__((weak)) main(int argc, char** argv)
{
  // single-threaded programs override this function.
  printstr("Implement main(), foo!\n");
  return -1;
}
/** @fn 
 * @brief 
 * @details 
 */
static void init_tls()
{
  register void* thread_pointer asm("tp");
  extern char _tls_data;
  extern __thread char _tdata_begin, _tdata_end, _tbss_end;
  size_t tdata_size = &_tdata_end - &_tdata_begin;
  memcpy(thread_pointer, &_tls_data, tdata_size);
  size_t tbss_size = &_tbss_end - &_tdata_end;
  memset(thread_pointer + tdata_size, 0, tbss_size);
}
/** @fn 
 * @brief 
 * @details 
 */
void _init(int cid, int nc)
{
  init_tls();
  thread_entry(cid, nc);

  // only single-threaded programs should ever get here.
  int ret = main(0, 0);
    
   exit(0);
  
}
#undef putchar
/** @fn 
 * @brief 
 * @details 
 */
int putchar(int ch)
{
  tx_uart(ch);

if(ch=='\n')
 tx_uart('\r');
  return 0;
}
/** @fn 
 * @brief 
 * @details 
 */
void printhex(UL x)
{
  char str[17];
  int i;
  for (i = 0; i < 16; i++)
  {
    str[15-i] = (x & 0xF) + ((x & 0xF) < 10 ? '0' : 'a'-10);
    x >>= 4;
  }
  str[16] = 0;

  printstr(str);
}
/** @fn 
 * @brief 
 * @details 
 */
static inline void printnum(void (*putch)(int, void**), void **putdat,
                    unsigned long long num, unsigned base, int width, int padc)
{
  unsigned digs[sizeof(num)*CHAR_BIT];
  int pos = 0;

  while (1)
  {
    digs[pos++] = num % base;
    if (num < base)
      break;
    num /= base;
  }

  while (width-- > pos)
    putch(padc, putdat);

  while (pos-- > 0)
    putch(digs[pos] + (digs[pos] >= 10 ? 'a' - 10 : '0'), putdat);
}
/** @fn 
 * @brief 
 * @details 
 */
static unsigned long long getuint(va_list *ap, int lflag)
{
  if (lflag >= 2)
    return va_arg(*ap, unsigned long long);
  else if (lflag)
    return va_arg(*ap, unsigned long);
  else
    return va_arg(*ap, unsigned int);
}
/** @fn 
 * @brief 
 * @details 
 */
static long long getint(va_list *ap, int lflag)
{
  if (lflag >= 2)
    return va_arg(*ap, long long);
  else if (lflag)
    return va_arg(*ap, long);
  else
    return va_arg(*ap, int);
}

#define ZEROPAD  	(1<<0)	/* Pad with zero */
#define SIGN    	(1<<1)	/* Unsigned/signed long */
#define PLUS    	(1<<2)	/* Show plus */
#define SPACE   	(1<<3)	/* Spacer */
#define LEFT    	(1<<4)	/* Left justified */
#define HEX_PREP 	(1<<5)	/* 0x */
#define UPPERCASE   (1<<6)	/* 'ABCDEF' */

#define is_digit(c) ((c) >= '0' && (c) <= '9')

static char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static char *upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//static size_t strnlen(const char *s, size_t count);



/*static size_t strnlen(const char *s, size_t count)
{
  const char *sc;
  for (sc = s; *sc != '\0' && count--; ++sc);
  return sc - s;
}*/

static int skip_atoi(const char **s)
{
  int i = 0;
  while (is_digit(**s)) i = i*10 + *((*s)++) - '0';
  return i;
}

int atoi(char* str) 
{ 
    // Initialize result 
    int res = 0; 
  
    // Iterate through all characters 
    // of input string and update result 
    for (int i = 0; str[i] 
                    != '\0'; 
         ++i) 
        res = res * 10 + str[i] - '0'; 
  
    // return result. 
    return res; 
} 

float atof(char* s){
  float rez = 0, fact = 1;
  if (*s == '-'){
    s++;
    fact = -1;
  };
  for (int point_seen = 0; *s; s++){
    if (*s == '.'){
      point_seen = 1; 
      continue;
    };
    int d = *s - '0';
    if (d >= 0 && d <= 9){
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (float)d;
    };
  };
  return rez * fact;
};



/**
@fn int accurate_udelay(unsigned int microsec)
@brief Microsecond accurate delay for ARIES board
@param[in] micro seconds
@return int
*/
// unsigned int loop_count;
volatile unsigned int loop_count;
__attribute__ ((noinline))
int _usleep(unsigned int microsec)

{
	
        loop_count=((SYSTEM_FREQUENCY/3)*microsec);      
    

        asm volatile("addi sp,sp,-8\n\t"
                 "sw a0,0(sp)\n\t"
                     "sw a1,4(sp)\n\t"
                 "la a0, loop_count\n\t"
                 "lw a1,(a0)\n\t"
                 "loop_us: \n\t"
                       "addi a1,a1,-1\n\t"
                   "bnez a1,loop_us\n\t"

                 "lw a0,0(sp)\n\t"
                 "lw a1,4(sp)\n\t"
                 "addi sp,sp,8\n\t"); 
}
/**
@fn int udelay(unsigned int count)
@brief Millisecond accurate delay for Thejas32 board
@param[in] int count
@return 0
*/
int udelay(unsigned int count)
{
	for(int i=0;i<(count*35);i++); //delay millisecond calculation with board mhz
}
/*
int udelay(unsigned int count)
{
	clock_t clock_count,target_count;	

	clock_count = rdcycle(); //delay millisecond calculation with board mhz

	target_count = clock_count + (count*35);

	while(clock_count < target_count) 
	{
		clock_count = rdcycle();
	}

}*/

__attribute__ ((noinline))
void usleep(unsigned long delay){

asm volatile("mv a5,%0\n\t"
	"li a6,1 \n\t"
	"beq a5,a6,final\n\t"
	"li a6,3 \n\t"
	"bge a5,a6,cycle_loop\n\t"
	"loop: addi a5,a5,-1\n\t"
	"nop\n\t"
	"nop\n\t"
	"bnez a5,loop\n\t"
	"j final\n\t"
	"cycle_loop: li a4,33\n\t"
	"mul a4,a5,a4\n\t"
	//"csrr a6,mcycle\n\t"
	"loop1: addi a4,a4,-1\n\t"	
	"bnez a4,loop1\n\t"
	//"addi a5,a5,-1\n\t"
	//"bnez a5,cycle_loop\n\t"
	"final:\n\t"
	: : "r"(delay) : "a5" );
} 

clock_t get_time()
{
	clock_t clock_count;	

	clock_count = read_csr(mcycle); 
	
	return clock_count;
}



static char *number(char *str, long num, int base, int size, int precision, int type)
{
  char c, sign, tmp[66];
  char *dig = digits;
  int i;

  if (type & UPPERCASE)  dig = upper_digits;
  if (type & LEFT) type &= ~ZEROPAD;
  if (base < 2 || base > 36) return 0;
  
  c = (type & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (type & SIGN)
  {
    if (num < 0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if (type & PLUS)
    {
      sign = '+';
      size--;
    }
    else if (type & SPACE)
    {
      sign = ' ';
      size--;
    }
  }

  if (type & HEX_PREP)
  {
    if (base == 16)
      size -= 2;
    else if (base == 8)
      size--;
  }

  i = 0;

  if (num == 0)
    tmp[i++] = '0';
  else
  {
    while (num != 0)
    {
      tmp[i++] = dig[((unsigned long) num) % (unsigned) base];
      num = ((unsigned long) num) / (unsigned) base;
    }
  }

  if (i > precision) precision = i;
  size -= precision;
  if (!(type & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
  if (sign) *str++ = sign;
  
  if (type & HEX_PREP)
  {
    if (base == 8)
      *str++ = '0';
    else if (base == 16)
    {
      *str++ = '0';
      *str++ = digits[33];
    }
  }

  if (!(type & LEFT)) while (size-- > 0) *str++ = c;
  while (i < precision--) *str++ = '0';
  while (i-- > 0) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';

  return str;
}

static char *eaddr(char *str, unsigned char *addr, int size, int precision, int type)
{
  char tmp[24];
  char *dig = digits;
  int i, len;

  if (type & UPPERCASE)  dig = upper_digits;
  len = 0;
  for (i = 0; i < 6; i++)
  {
    if (i != 0) tmp[len++] = ':';
    tmp[len++] = dig[addr[i] >> 4];
    tmp[len++] = dig[addr[i] & 0x0F];
  }

  if (!(type & LEFT)) while (len < size--) *str++ = ' ';
  for (i = 0; i < len; ++i) *str++ = tmp[i];
  while (len < size--) *str++ = ' ';

  return str;
}

static char *iaddr(char *str, unsigned char *addr, int size, int precision, int type)
{
  char tmp[24];
  int i, n, len;

  len = 0;
  for (i = 0; i < 4; i++)
  {
    if (i != 0) tmp[len++] = '.';
    n = addr[i];
    
    if (n == 0)
      tmp[len++] = digits[0];
    else
    {
      if (n >= 100) 
      {
        tmp[len++] = digits[n / 100];
        n = n % 100;
        tmp[len++] = digits[n / 10];
        n = n % 10;
      }
      else if (n >= 10) 
      {
        tmp[len++] = digits[n / 10];
        n = n % 10;
      }

      tmp[len++] = digits[n];
    }
  }

  if (!(type & LEFT)) while (len < size--) *str++ = ' ';
  for (i = 0; i < len; ++i) *str++ = tmp[i];
  while (len < size--) *str++ = ' ';

  return str;
}

#ifdef HAS_FLOAT

char *ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
char *fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf);
static void ee_bufcpy(char *d, char *s, int count); 
 
void ee_bufcpy(char *pd, char *ps, int count) {
	char *pe=ps+count;
	while (ps!=pe)
		*pd++=*ps++;
}

static void parse_float(double value, char *buffer, char fmt, int precision)
{
  int decpt, sign, exp, pos;
  char *digits = NULL;
  char cvtbuf[80];
  int capexp = 0;
  int magnitude;

  if (fmt == 'G' || fmt == 'E')
  {
    capexp = 1;
    fmt += 'a' - 'A';
  }

  if (fmt == 'g')
  {
    digits = ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
    magnitude = decpt - 1;
    if (magnitude < -4  ||  magnitude > precision - 1)
    {
      fmt = 'e';
      precision -= 1;
    }
    else
    {
      fmt = 'f';
      precision -= decpt;
    }
  }

  if (fmt == 'e')
  {
    digits = ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);

    if (sign) *buffer++ = '-';
    *buffer++ = *digits;
    if (precision > 0) *buffer++ = '.';
    ee_bufcpy(buffer, digits + 1, precision);
    buffer += precision;
    *buffer++ = capexp ? 'E' : 'e';

    if (decpt == 0)
    {
      if (value == 0.0)
        exp = 0;
      else
        exp = -1;
    }
    else
      exp = decpt - 1;

    if (exp < 0)
    {
      *buffer++ = '-';
      exp = -exp;
    }
    else
      *buffer++ = '+';

    buffer[2] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[1] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[0] = (exp % 10) + '0';
    buffer += 3;
  }
  else if (fmt == 'f')
  {
    digits = fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
    if (sign) *buffer++ = '-';
    if (*digits)
    {
      if (decpt <= 0)
      {
        *buffer++ = '0';
        *buffer++ = '.';
        for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
        while (*digits) *buffer++ = *digits++;
      }
      else
      {
        pos = 0;
        while (*digits)
        {
          if (pos++ == decpt) *buffer++ = '.';
          *buffer++ = *digits++;
        }
      }
    }
    else
    {
      *buffer++ = '0';
      if (precision > 0)
      {
        *buffer++ = '.';
        for (pos = 0; pos < precision; pos++) *buffer++ = '0';
      }
    }
  }

  *buffer = '\0';
}

static void decimal_point(char *buffer)
{
  while (*buffer)
  {
    if (*buffer == '.') return;
    if (*buffer == 'e' || *buffer == 'E') break;
    buffer++;
  }

  if (*buffer)
  {
    int n = strnlen(buffer,256);
    while (n > 0) 
    {
      buffer[n + 1] = buffer[n];
      n--;
    }

    *buffer = '.';
  }
  else
  {
    *buffer++ = '.';
    *buffer = '\0';
  }
}

static void cropzeros(char *buffer)
{
  char *stop;

  while (*buffer && *buffer != '.') buffer++;
  if (*buffer++)
  {
    while (*buffer && *buffer != 'e' && *buffer != 'E') buffer++;
    stop = buffer--;
    while (*buffer == '0') buffer--;
    if (*buffer == '.') buffer--;
    while (buffer!=stop)
		*++buffer=0;
  }
}
/**
@fn static char *flt(char *str, double num, int size, int precision, char fmt, int flags)
@brief Floating point format specifier convertor
@param[in] char *str
@param[in] double num
@param[in] int size
@param[in] int precision
@param[in] char fmt
@param[in] int flags
@return The string pointer
*/
static char *flt(char *str, double num, int size, int precision, char fmt, int flags)
{
  char tmp[80];
  char c, sign;
  int n, i;

  // Left align means no zero padding
  if (flags & LEFT) flags &= ~ZEROPAD;

  // Determine padding and sign char
  c = (flags & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (flags & SIGN)
  {
    if (num < 0.0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if (flags & PLUS)
    {
      sign = '+';
      size--;
    }
    else if (flags & SPACE)
    {
      sign = ' ';
      size--;
    }
  }

  // Compute the precision value
  if (precision < 0)
    precision = 6; // Default precision: 6

  // Convert floating point number to text
  parse_float(num, tmp, fmt, precision);

  if ((flags & HEX_PREP) && precision == 0) decimal_point(tmp);
  if (fmt == 'g' && !(flags & HEX_PREP)) cropzeros(tmp);

  n = strnlen(tmp,256);

  // Output number with alignment and padding
  size -= n;
  if (!(flags & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
  if (sign) *str++ = sign;
  if (!(flags & LEFT)) while (size-- > 0) *str++ = c;
  for (i = 0; i < n; i++) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';

  return str;
}

#endif
/**
@fn static int ee_vsprintf(char *buf, const char *fmt, va_list args)
@brief Format parser for printf function
@param[in] const char *fmt
@param[in] char *buf
@param[in] va_list args
@return Number of charactes
*/
static int ee_vsprintf(char *buf, const char *fmt, va_list args)
{
  int len;
  unsigned long num;
  int i, base;
  char *str;
  char *s;

  int flags;            // Flags to number()

  int field_width;      // Width of output field
  int precision;        // Min. # of digits for integers; max number of chars for from string
  int qualifier;        // 'h', 'l', or 'L' for integer fields

  for (str = buf; *fmt; fmt++)
  {
    if (*fmt != '%')
    {
      *str++ = *fmt;
      continue;
    }
                  
    // Process flags
    flags = 0;
repeat:
    fmt++; // This also skips first '%'
    switch (*fmt)
    {
      case '-': flags |= LEFT; goto repeat;
      case '+': flags |= PLUS; goto repeat;
      case ' ': flags |= SPACE; goto repeat;
      case '#': flags |= HEX_PREP; goto repeat;
      case '0': flags |= ZEROPAD; goto repeat;
    }
          
    // Get field width
    field_width = -1;
    if (is_digit(*fmt))
      field_width = skip_atoi(&fmt);
    else if (*fmt == '*')
    {
      fmt++;
      field_width = va_arg(args, int);
      if (field_width < 0)
      {
        field_width = -field_width;
        flags |= LEFT;
      }
    }

    // Get the precision
    precision = -1;
    if (*fmt == '.')
    {
      ++fmt;    
      if (is_digit(*fmt))
        precision = skip_atoi(&fmt);
      else if (*fmt == '*')
      {
        ++fmt;
        precision = va_arg(args, int);
      }
      if (precision < 0) precision = 0;
    }

    // Get the conversion qualifier
    qualifier = -1;
    if (*fmt == 'l' || *fmt == 'L')
    {
      qualifier = *fmt;
      fmt++;
    }

    // Default base
    base = 10;

    switch (*fmt)
    {
      case 'c':
        if (!(flags & LEFT)) while (--field_width > 0) *str++ = ' ';
        *str++ = (unsigned char) va_arg(args, int);
        while (--field_width > 0) *str++ = ' ';
        continue;

      case 's':
        s = va_arg(args, char *);
        if (!s) s = "<NULL>";
        len = strnlen(s, precision);
        if (!(flags & LEFT)) while (len < field_width--) *str++ = ' ';
        for (i = 0; i < len; ++i) *str++ = *s++;
        while (len < field_width--) *str++ = ' ';
        continue;

      case 'p':
        if (field_width == -1)
        {
          field_width = 2 * sizeof(void *);
          flags |= ZEROPAD;
        }
        str = number(str, (unsigned long) va_arg(args, void *), 16, field_width, precision, flags);
        continue;

      case 'A':
        flags |= UPPERCASE;

      case 'a':
        if (qualifier == 'l')
          str = eaddr(str, va_arg(args, unsigned char *), field_width, precision, flags);
        else
          str = iaddr(str, va_arg(args, unsigned char *), field_width, precision, flags);
        continue;

      // Integer number formats - set up the flags and "break"
      case 'o':
        base = 8;
        break;

      case 'X':
        flags |= UPPERCASE;

      case 'x':
        base = 16;
        break;

      case 'd':
      case 'i':
        flags |= SIGN;

      case 'u':
        break;

#ifdef HAS_FLOAT

      case 'f':
        str = flt(str, va_arg(args, double), field_width, precision, *fmt, flags | SIGN);
        continue;

#endif

      default:
        if (*fmt != '%') *str++ = '%';
        if (*fmt)
          *str++ = *fmt;
        else
          --fmt;
        continue;
    }

    if (qualifier == 'l')
      num = va_arg(args, unsigned long);
    else if (flags & SIGN)
      num = va_arg(args, int);
    else
      num = va_arg(args, unsigned int);

    str = number(str, num, base, field_width, precision, flags);
  }

  *str = '\0';
  return str - buf;
}

#undef puts
int puts(const char *p)
{
    int i = 0;
    while (*p)
    {
        if( putchar(*p++) == EOF)  //if we got the EOF value from writing the char
        { 
            return EOF;
        }      
    }
   if(putchar('\n') == EOF)  //this will occur right after we quit due to the null terminated character.
   {
       return EOF;
   }
   return 1; //to meet spec.
}


/**
@fn int printf(const char *fmt, ...)
@brief To print the string in to a standard console
@param[in] const char *fmt
@return Number of printed charactes
*/
#undef printf
int printf(const char *fmt, ...)
{
  char buf[256],*p;
  va_list args;
  int n=0;

  va_start(args, fmt);
  ee_vsprintf(buf, fmt, args);
  va_end(args);
  p=buf;
  while (*p) {
    	putchar((int)*p);
	n++;
	p++;
  }

  return n;
}
/**
@fn int sprintf(char * buffer,const char *fmt, ...)
@brief To print the string in to a give buffer
@param[in] const char *fmt
@param[out] char * buffer
@return Number of printed charactes
*/
#undef sprintf
int sprintf(char * buffer,const char *fmt, ...)
{
  char *p;
  va_list args;
  int n=0;

  va_start(args, fmt);
  ee_vsprintf(buffer, fmt, args);
  va_end(args);
  /*p=buffer;
  while (*p) {
    	putchar((int)*p);
	n++;
	p++;
  }*/

  return n;
}

void *_sbrk(int incr) { return (void *)-1; }


