#ifdef PROTOTYPE
int tmax();
int test_tmax();
#endif
#ifdef DECL
 {"tmax", (funct_t) tmax, (funct_t) test_tmax, 0, "! ~ & ^ | + << >>", 4, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
#ifdef FIX
  return ~(1 << 31);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_tmax(void) {
  return 0x7FFFFFFF;
}
#endif
