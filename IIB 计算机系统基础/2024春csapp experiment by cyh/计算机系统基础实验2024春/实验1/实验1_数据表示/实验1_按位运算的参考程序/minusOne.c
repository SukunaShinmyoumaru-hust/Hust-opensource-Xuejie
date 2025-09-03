#ifdef PROTOTYPE
int minusOne();
int test_minusOne();
#endif
#ifdef DECL
 {"minusOne", (funct_t) minusOne, (funct_t) test_minusOne, 0,
    "! ~ & ^ | + << >>", 2, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
#ifdef FIX
  return ~0;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_minusOne(void) {
  return -1;
}
#endif
