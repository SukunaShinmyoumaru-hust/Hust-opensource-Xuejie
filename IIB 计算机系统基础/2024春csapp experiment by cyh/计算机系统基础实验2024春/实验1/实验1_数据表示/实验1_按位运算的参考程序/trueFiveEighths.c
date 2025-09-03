#ifdef PROTOTYPE
int trueFiveEighths(int);
int test_trueFiveEighths(int);
#endif
#ifdef DECL
 {"trueFiveEighths", (funct_t) trueFiveEighths, (funct_t) test_trueFiveEighths, 1,
    "! ~ & ^ | + << >>", 25, 4,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * trueFiveEighths - multiplies by 5/8 rounding toward 0,
 *  avoiding errors due to overflow
 *  Examples: trueFiveEighths(11) = 6
 *            trueFiveEighths(-9) = -5
 *            trueFiveEighths(0x30000000) = 0x1E000000 (no overflow)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 4
 */
int trueFiveEighths(int x)
{
#ifdef FIX
    int xs1 = x >> 1;
    int xs2 = x >> 3;
    int bias = (x >> 31) & 7;
    int xl2 = x & 7;
    int xl1 = (x & 1) << 2;
    int incr = (xl2 + xl1 + bias) >> 3;
    return xs1 + xs2 + incr;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_trueFiveEighths(int x)
{
  return (int) (((long long int) x * 5)/8);
}
#endif

