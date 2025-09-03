#ifdef PROTOTYPE
int subOK(int, int);
int test_subOK(int, int);
#endif
#ifdef DECL
 {"subOK", (funct_t) subOK, (funct_t) test_subOK, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
#ifdef FIX
  int diff = x+~y+1;
  int x_neg = x>>31;
  int y_neg = y>>31;
  int d_neg = diff>>31;
  /* Overflow when x and y have opposite sign, and d different from x */
  return !(~(x_neg ^ ~y_neg) & (x_neg ^ d_neg));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_subOK(int x, int y)
{
  long long ldiff = (long long) x - y;
  return ldiff == (int) ldiff;
}
#endif
