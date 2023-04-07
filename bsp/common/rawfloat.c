/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Nov-2019
 Filename		: rawfloat.c
 Purpose		: Generic library functions
 Description		: Provides generic library functions like 
			  printf/memcpy etc 
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/

/**
 @file rawfloat.c
 @brief Contains routines for floating point conversions
 @detail 
 */

double modf(double value, register double * iptr);

#define CVTBUFSIZE 80
static char CVTBUF[CVTBUFSIZE];
/**
@fn static char *cvt(double arg, int ndigits, int *decpt, int *sign, char *buf, int eflag)
@brief To convert a floating point value to ascii string
@param[in] double arg
@param[in] int ndigits
@param[in] int *decpt
@param[in] int *sign
@param[in] char *buf
@param[in] int eflag
@param[Out] No ouput parameter.
@return Pointer to converted string

*/
static char *cvt(double arg, int ndigits, int *decpt, int *sign, char *buf, int eflag)
{
  int r2;
  double fi, fj;
  volatile char *p, *p1;

  if (ndigits < 0) ndigits = 0;
  if (ndigits >= CVTBUFSIZE - 1) ndigits = CVTBUFSIZE - 2;
  r2 = 0;
  *sign = 0;
  p = &buf[0];
  if (arg < 0)
  {
    *sign = 1;
    arg = -arg;
  }
  arg = modf(arg, &fi);
  p1 = &buf[CVTBUFSIZE];

  if (fi != 0) 
  {
    p1 = &buf[CVTBUFSIZE];
    while (fi != 0) 
    {
      fj = modf(fi / 10, &fi);
      *--p1 = (int)((fj + .03) * 10) + '0';
      r2++;
    }
    while (p1 < &buf[CVTBUFSIZE]) *p++ = *p1++;
  } 
  else if (arg > 0)
  {
    while ((fj = arg * 10) < 1) 
    {
      arg = fj;
      r2--;
    }
  }
  p1 = &buf[ndigits];
  if (eflag == 0) p1 += r2;
  *decpt = r2;
  if (p1 < &buf[0]) 
  {
    buf[0] = '\0';
    return buf;
  }
  while (p <= p1 && p < &buf[CVTBUFSIZE])
  {
    arg *= 10;
    arg = modf(arg, &fj);
    *p++ = (int) fj + '0';
  }
  if (p1 >= &buf[CVTBUFSIZE]) 
  {
    buf[CVTBUFSIZE - 1] = '\0';
    return buf;
  }
  p = p1;
  *p1 += 5;
  while (*p1 > '9') 
  {
    *p1 = '0';
    if (p1 > buf)
      ++*--p1;
    else 
    {
      *p1 = '1';
      (*decpt)++;
      if (eflag == 0) 
      {
        if (p > buf) *p = '0';
        p++;
      }
    }
  }
  *p = '\0';
  return buf;
}

char *ecvt(double arg, int ndigits, int *decpt, int *sign)
{
  return cvt(arg, ndigits, decpt, sign, CVTBUF, 1);
}

char *ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf)
{
  return cvt(arg, ndigits, decpt, sign, buf, 1);
}

char *fcvt(double arg, int ndigits, int *decpt, int *sign)
{
  return cvt(arg, ndigits, decpt, sign, CVTBUF, 0);
}

char *fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf)
{
  return cvt(arg, ndigits, decpt, sign, buf, 0);
}

