#ifdef PROTOTYPE
int sm2tc(int);
int test_sm2tc(int);
#endif
#ifdef DECL
 {"sm2tc", (funct_t) sm2tc, (funct_t) test_sm2tc, 1, "! ~ & ^ | + << >>", 15, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * sm2tc - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: sm2tc(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int sm2tc(int x) {
#ifdef FIX
  int mask = x>>31;
  int mag = x & ~(1<<31);
  return (mag ^ mask) + ~mask + 1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_sm2tc(int x) {
  int sign = x < 0;
  int mag  = x & 0x7FFFFFFF;
  return sign ? -mag : mag;
}
#endif
