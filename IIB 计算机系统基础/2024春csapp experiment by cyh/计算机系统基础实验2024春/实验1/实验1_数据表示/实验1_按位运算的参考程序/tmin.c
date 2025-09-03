#ifdef PROTOTYPE
int tmin();
int test_tmin();
#endif
#ifdef DECL
 {"tmin", (funct_t) tmin, (funct_t) test_tmin, 0, "! ~ & ^ | + << >>", 4, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
#ifdef FIX
  return 1<<31;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_tmin(void) {
  return 0x80000000;
}
#endif
