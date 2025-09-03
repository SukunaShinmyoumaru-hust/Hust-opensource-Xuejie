#ifdef PROTOTYPE
int isGreater(int, int);
int test_isGreater(int, int);
#endif
#ifdef DECL
 {"isGreater", (funct_t) isGreater, (funct_t) test_isGreater, 2,
    "! ~ & ^ | + << >>", 24, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
#ifdef FIX
  int x_neg = x>>31;
  int y_neg = y>>31;  
  return !((x_neg & !y_neg) | (!(x_neg ^ y_neg) & (~y+x)>>31));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isGreater(int x, int y)
{
  return x > y;
}
#endif
