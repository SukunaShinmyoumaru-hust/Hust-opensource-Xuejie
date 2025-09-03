#ifdef PROTOTYPE
int addOK(int, int);
int test_addOK(int, int);
#endif
#ifdef DECL
 {"addOK", (funct_t) addOK, (funct_t) test_addOK, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
#ifdef FIX
  int sum = x+y;
  int x_neg = x>>31;
  int y_neg = y>>31;
  int s_neg = sum>>31;
  /* Overflow when x and y have same sign, but s is different */
  return !(~(x_neg ^ y_neg) & (x_neg ^ s_neg));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_addOK(int x, int y)
{
    long long lsum = (long long) x + y;
    return lsum == (int) lsum;
}
#endif
