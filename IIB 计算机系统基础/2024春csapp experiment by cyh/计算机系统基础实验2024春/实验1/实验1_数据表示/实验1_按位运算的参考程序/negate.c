#ifdef PROTOTYPE
int negate(int);
int test_negate(int);
#endif
#ifdef DECL
 {"negate", (funct_t) negate, (funct_t) test_negate, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
#ifdef FIX
  return ~x+1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_negate(int x) {
  return -x;
}
#endif
