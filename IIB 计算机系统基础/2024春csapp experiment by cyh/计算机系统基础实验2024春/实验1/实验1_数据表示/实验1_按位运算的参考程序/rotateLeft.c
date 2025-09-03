#ifdef PROTOTYPE
int rotateLeft(int, int);
int test_rotateLeft(int, int);
#endif
#ifdef DECL
 {"rotateLeft", (funct_t) rotateLeft, (funct_t) test_rotateLeft,
   2, "! ~ & ^ | + << >>", 25, 3,
  {{TMin, TMax},{0,31},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
#ifdef FIX
    /* Create mask for n = 0 */
    int zmask = (~!n)+1;
    int left = x << n;
    /* Arithmetic shift right by 32-n */
    int right = x >> (33 + ~n);
    /* Mask off upper 1's */
    int lmask = ~0 << n;
    right &= ~lmask;
    return (zmask&x) | (~zmask&(left|right));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_rotateLeft(int x, int n) {
  unsigned u = (unsigned) x;
  int i;
  for (i = 0; i < n; i++) {
      unsigned msb = u >> 31;
      unsigned rest = u << 1;
      u = rest | msb;
  }
  return (int) u;

}
#endif
