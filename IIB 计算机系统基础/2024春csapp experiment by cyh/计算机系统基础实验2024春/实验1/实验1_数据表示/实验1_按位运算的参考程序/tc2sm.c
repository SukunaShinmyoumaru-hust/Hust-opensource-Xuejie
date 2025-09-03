#ifdef PROTOTYPE
int tc2sm(int);
int test_tc2sm(int);
#endif
#ifdef DECL
 {"tc2sm", (funct_t) tc2sm, (funct_t) test_tc2sm, 1, "! ~ & ^ | + << >>", 15, 4,
  {{TMin+1, TMax},{TMin+1,TMax},{TMin+1,TMax}}},
#endif
#ifdef CODE
/* 
 * tc2sm - Convert from two's complement to sign-magnitude 
 *   where the MSB is the sign bit
 *   You can assume that x > TMin
 *   Example: tc2sm(-5) = 0x80000005.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int tc2sm(int x) {
#if FIX
  int mask = x >> 31;
  int sign = mask & 1;
  /* Compute absolute value of x */
  int abs  = (mask ^ x) + sign;
  return abs | (sign << 31);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_tc2sm(int x) {
  int sign = x < 0;
  int mag  = x < 0 ? -x : x;
  return (sign << 31) | mag;
}
#endif
