#ifdef PROTOTYPE
int ilog2(int);
int test_ilog2(int);
#endif
#ifdef DECL
 {"ilog2", (funct_t) ilog2, (funct_t) test_ilog2, 1, "! ~ & ^ | + << >>", 90, 4,
  {{1, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
#ifdef FIX
  int m16 = ((1<<16) + ~0) << 16;                         /* groups of 16 */
  int m8 = (((1<<8)  + ~0) << 24) + (((1<<8) + ~0) << 8); /* groups of 8 */
  int m4 = (0xf0<<24) + (0xf0<<16) + (0xf0<<8) + 0xf0;    /* groups of 4 */
  int m2 = (0xcc<<24) + (0xcc<<16) + (0xcc<<8) + 0xcc;    /* groups of 2 */
  int m1 = (0xaa<<24) + (0xaa<<16) + (0xaa<<8) + 0xaa;    /* groups of 1 */
  int result = 0;
  int upper;
  int mask;

  /* m16 */
  upper = !!(x & m16);
  result += upper << 4;
  mask = m16 ^ ~((upper<<31)>>31);

  /* m8 */
  upper = !!(x & m8 & mask);
  result += upper << 3;
  mask &= (m8 ^ ~((upper<<31)>>31)); 

  /* m4 */
  upper = !!(x & m4 & mask);
  result += upper << 2;
  mask &= (m4 ^ ~((upper<<31)>>31)); 

  /* m2 */
  upper = !!(x & m2 & mask);
  result += upper << 1;
  mask &= (m2 ^ ~((upper<<31)>>31)); 

  /* m1 */
  upper = !!(x & m1 & mask);
  result += upper;

  return result;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_ilog2(int x) {
  int mask, result;
  /* find the leftmost bit */
  result = 31;
  mask = 1 << result;
  while (!(x & mask)) {
    result--;
    mask = 1 << result;
  }
  return result;
}
#endif
