#ifdef PROTOTYPE
int isZero(int);
int test_isZero(int);
#endif
#ifdef DECL
 {"isZero", (funct_t) isZero, (funct_t) test_isZero, 1, "! ~ & ^ | + << >>", 2, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * isZero - returns 1 if x == 0, and 0 otherwise 
 *   Examples: isZero(5) = 0, isZero(0) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int isZero(int x) {
#ifdef FIX  
  return !x;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isZero(int x) {
  return x == 0;
}
#endif
