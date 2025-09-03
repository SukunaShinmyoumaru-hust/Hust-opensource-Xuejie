#ifdef PROTOTYPE
int isLess(int, int);
int test_isLess(int, int);
#endif
#ifdef DECL
 {"isLess", (funct_t) isLess, (funct_t) test_isLess, 2,
    "! ~ & ^ | + << >>", 24, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
#ifdef FIX
  int x_neg = (x>>31);
  int y_neg = (y>>31);
  return !((!x_neg & y_neg) | (!(x_neg ^ y_neg) & (y+~x)>>31));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isLess(int x, int y)
{
  return x < y;
}
#endif
