#ifdef PROTOTYPE
int bitOr(int, int);
int test_bitOr(int, int);
#endif
#ifdef DECL
 {"bitOr", (funct_t) bitOr, (funct_t) test_bitOr, 2, "& ~", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
#ifdef FIX
  return ~(~x & ~y);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitOr(int x, int y)
{
  return x|y;
}
#endif
