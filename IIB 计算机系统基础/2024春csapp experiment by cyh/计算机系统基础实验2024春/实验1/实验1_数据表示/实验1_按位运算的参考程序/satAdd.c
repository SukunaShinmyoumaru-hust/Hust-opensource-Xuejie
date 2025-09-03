#ifdef PROTOTYPE
int satAdd(int, int);
int test_satAdd(int, int);
#endif
#ifdef DECL
 {"satAdd", (funct_t) satAdd, (funct_t) test_satAdd, 2,
    "! ~ & ^ | + << >>", 30, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
#ifdef FIX
  int signbit = 31;
  int buzz = ((x^y) | (x^~(x+y))) >> signbit;
  /* buzz is all 1's if no overflow. if overflow, return maxint or minint
   * depending on which way we hit the wall */
  int out = (buzz & (x+y)) | (~buzz & (((x+y)>>signbit) ^ (1L<<signbit)));
  return out;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_satAdd(int x, int y)
{
  if (x > 0 && y > 0 && x+y < 0)
    return (0x7FFFFFFF);
  if (x < 0 && y < 0 && x+y >= 0)
    return (0x80000000);
  return x + y;
}
#endif
