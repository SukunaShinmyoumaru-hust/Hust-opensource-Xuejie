#ifdef PROTOTYPE
int copyLSB(int);
int test_copyLSB(int);
#endif
#ifdef DECL
 {"copyLSB", (funct_t) copyLSB, (funct_t) test_copyLSB, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
#ifdef FIX
  /* Shift bit to MSB and then right shift (arithmetically) back */
  int result = (x<<31)>>31;
  return result;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_copyLSB(int x)
{
  return (x & 0x1) ? -1 : 0;
}
#endif
