#ifdef PROTOTYPE
int bitParity(int);
int test_bitParity(int);
#endif
#ifdef DECL
 {"bitParity", (funct_t) bitParity, (funct_t) test_bitParity, 1, "! ~ & ^ | + << >>", 20, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
#ifdef FIX  
  /* Work things down.  At any time, upper part of words will
     contain junk.  Mask this off at the very end
  */
 int wd16 = x ^ x>>16; /* Combine into 16 bits */
 int wd8  = wd16 ^ wd16>>8; /* Combine into 8 bits */
 int wd4  = wd8 ^ wd8>>4;
 int wd2  = wd4 ^ wd4>>2;
 int bit  = (wd2 ^ wd2>>1) & 0x1;
 return bit;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitParity(int x) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i++)
    result ^=  (x >> i) & 0x1;
  return result;
}
#endif
