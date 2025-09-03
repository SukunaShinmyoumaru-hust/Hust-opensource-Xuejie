#ifdef PROTOTYPE
int isLessOrEqual(int, int);
int test_isLessOrEqual(int, int);
#endif
#ifdef DECL
 {"isLessOrEqual", (funct_t) isLessOrEqual, (funct_t) test_isLessOrEqual, 2,
    "! ~ & ^ | + << >>", 24, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
#ifdef FIX
  int x_neg = x>>31;
  int y_neg = y>>31;  
  return !(((!x_neg) & y_neg) | ((!(x_neg ^ y_neg)) & (y+~x+1)>>31));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isLessOrEqual(int x, int y)
{
  return x <= y;
}
#endif
