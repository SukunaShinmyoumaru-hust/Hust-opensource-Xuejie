#ifdef PROTOTYPE
int isNonZero(int);
int test_isNonZero(int);
#endif
#ifdef DECL
 {"isNonZero", (funct_t) isNonZero, (funct_t) test_isNonZero, 1,
    "~ & ^ | + << >>", 10, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4 
 */
int isNonZero(int x) {
#ifdef FIX
  int minus_x = ~x+1;
  return ((minus_x|x) >> 31) & 1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isNonZero(int x)
{
  return x!=0;
}
#endif

