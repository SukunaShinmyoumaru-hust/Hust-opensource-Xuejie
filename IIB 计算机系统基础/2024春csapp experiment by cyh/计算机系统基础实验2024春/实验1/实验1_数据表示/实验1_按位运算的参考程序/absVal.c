#ifdef PROTOTYPE
int absVal(int);
int test_absVal(int);
#endif
#ifdef DECL
 {"absVal", (funct_t) absVal, (funct_t) test_absVal, 1, "! ~ & ^ | + << >>", 10, 4,
  {{-TMax, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
#ifdef FIX
  int mask = x>>31;
  return (x ^ mask) + ~mask + 1L;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_absVal(int x) {
  return (x < 0) ? -x : x; 
}
#endif
