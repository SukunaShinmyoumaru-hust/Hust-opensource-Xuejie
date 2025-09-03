#ifdef PROTOTYPE
int bitAnd(int, int);
int test_bitAnd(int, int);
#endif
#ifdef DECL
 {"bitAnd", (funct_t) bitAnd, (funct_t) test_bitAnd, 2, "| ~", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
#ifdef FIX
  return ~(~x | ~y);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitAnd(int x, int y)
{
  return x&y;
}
#endif
