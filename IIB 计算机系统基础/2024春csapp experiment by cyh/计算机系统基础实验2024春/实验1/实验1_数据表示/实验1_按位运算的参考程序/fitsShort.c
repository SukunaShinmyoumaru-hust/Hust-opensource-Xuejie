#ifdef PROTOTYPE
int fitsShort(int);
int test_fitsShort(int);
#endif
#ifdef DECL
 {"fitsShort", (funct_t) fitsShort, (funct_t) test_fitsShort, 1,
    "! ~ & ^ | + << >>", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * fitsShort - return 1 if x can be represented as a 
 *   16-bit, two's complement integer.
 *   Examples: fitsShort(33000) = 0, fitsShort(-32768) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int fitsShort(int x) {
#ifdef FIX
  int shift1 = x >> 15;
  int shift2 = x >> 16;
  return !(shift1 ^ shift2);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_fitsShort(int x)
{
  short int sx = (short int) x;
  return x == sx;
}
#endif
