#ifdef PROTOTYPE
int trueThreeFourths(int);
int test_trueThreeFourths(int);
#endif
#ifdef DECL
 {"trueThreeFourths", (funct_t) trueThreeFourths, (funct_t) test_trueThreeFourths, 1,
    "! ~ & ^ | + << >>", 20, 4,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * trueThreeFourths - multiplies by 3/4 rounding toward 0,
 *   avoiding errors due to overflow
 *   Examples: trueThreeFourths(11) = 8
 *             trueThreeFourths(-9) = -6
 *             trueThreeFourths(1073741824) = 805306368 (no overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int trueThreeFourths(int x)
{
#ifdef FIX
  int xs1 = x >> 1;
  int xs2 = x >> 2;
  /* Compute value from low-order 2 bits */
  int bias = (x >> 31) & 0x3;
  int xl2 = x & 0x3;
  int xl1 = (x & 0x1) << 1;
  int incr = (xl2 + xl1 + bias) >> 2;
  return xs1 + xs2 + incr;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_trueThreeFourths(int x)
{
  return (int) (((long long int) x * 3)/4);
}
#endif

