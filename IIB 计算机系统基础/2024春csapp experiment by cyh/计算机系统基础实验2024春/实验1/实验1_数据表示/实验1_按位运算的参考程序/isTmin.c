#ifdef PROTOTYPE
int isTmin(int);
int test_isTmin(int);
#endif
#ifdef DECL
 {"isTmin", (funct_t) isTmin, (funct_t) test_isTmin, 1, "! ~ & ^ | +", 10, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
#ifdef FIX  
  return (!!x) & (!(x+x));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isTmin(int x) {
    return x == 0x80000000;
}
#endif
