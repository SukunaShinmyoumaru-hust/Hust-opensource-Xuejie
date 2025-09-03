#ifdef PROTOTYPE
int bitXor(int, int);
int test_bitXor(int, int);
#endif
#ifdef DECL
 {"bitXor", (funct_t) bitXor, (funct_t) test_bitXor, 2, "& ~", 14, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
#ifdef FIX
  int x_and_y = x&y;
  int x_or_y = ~(~x & ~y);
  return x_or_y & ~x_and_y;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitXor(int x, int y)
{
  return x^y;
}
#endif
