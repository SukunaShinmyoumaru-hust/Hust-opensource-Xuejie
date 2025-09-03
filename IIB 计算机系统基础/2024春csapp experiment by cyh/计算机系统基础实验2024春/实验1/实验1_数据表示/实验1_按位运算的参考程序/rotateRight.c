#ifdef PROTOTYPE
int rotateRight(int, int);
int test_rotateRight(int, int);
#endif
#ifdef DECL
 {"rotateRight", (funct_t) rotateRight, (funct_t) test_rotateRight,
   2, "! ~ & ^ | + << >>", 25, 3,
  {{TMin, TMax},{0,31},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
#ifdef FIX
    /* Create mask for n = 0 */
    int zmask = (~!n)+1;
    int lsval = 33+~n;
    /* Shift left by 32-n */
    int left = x << lsval;
    /* Arithmetic shift right by n */
    int right = x >> n;
    /* Mask off upper 1's */
    int lmask = ~0 << lsval;
    right &= ~lmask;
    return (zmask&x) | (~zmask&(left|right));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_rotateRight(int x, int n) {
  unsigned u = (unsigned) x;
  int i;
  for (i = 0; i < n; i++) {
      unsigned lsb = (u & 1) << 31;
      unsigned rest = u >> 1;
      u = lsb | rest;
  }
  return (int) u;

}
#endif
