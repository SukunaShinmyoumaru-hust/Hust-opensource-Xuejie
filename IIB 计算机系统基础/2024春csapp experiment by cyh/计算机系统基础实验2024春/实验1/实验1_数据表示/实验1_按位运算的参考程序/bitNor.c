#ifdef PROTOTYPE
int bitNor(int, int);
int test_bitNor(int, int);
#endif
#ifdef DECL
 {"bitNor", (funct_t) bitNor, (funct_t) test_bitNor, 2, "& ~", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
#ifdef FIX
  return (~x & ~y);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitNor(int x, int y)
{
  return ~(x|y);
}
#endif
