#ifdef PROTOTYPE
int logicalShift(int, int);
int test_logicalShift(int, int);
#endif
#ifdef DECL
 {"logicalShift", (funct_t) logicalShift, (funct_t) test_logicalShift,
   2, "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{0,31},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
#ifdef FIX
    /* Create mask for n = 0 */
    int zmask = (~!n)+1;
    /* Arithmetic shift right by n */
    int right = x >> n;
    /* Mask off upper 1's */
    int lmask = ~0 << (33 + ~n);
    right &= ~lmask;
    return (zmask & x) | (~zmask & right);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_logicalShift(int x, int n) {
  unsigned u = (unsigned) x;
  unsigned shifted = u >> n;
  return (int) shifted;
}
#endif
