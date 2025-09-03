#ifdef PROTOTYPE
int multFiveEighths(int);
int test_multFiveEighths(int);
#endif
#ifdef DECL
 {"multFiveEighths", (funct_t) multFiveEighths, (funct_t) test_multFiveEighths, 1,
    "! ~ & ^ | + << >>", 12, 3,
  {{-(1<<28)-1, (1<<28)-1},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * multFiveEighths - multiplies by 5/8 rounding toward 0.
 *   Should exactly duplicate effect of C expression (x*5/8),
 *   including overflow behavior.
 *   Examples: multFiveEighths(77) = 48
 *             multFiveEighths(-22) = -13
 *             multFiveEighths(1073741824) = 13421728 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int multFiveEighths(int x) {
#ifdef FIX
  int fivex = ((x << 2) + x);
  int bias = (fivex >> 31) & 7;
  return (fivex + bias) >> 3;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_multFiveEighths(int x)
{
  return (x*5)/8;
}
#endif

