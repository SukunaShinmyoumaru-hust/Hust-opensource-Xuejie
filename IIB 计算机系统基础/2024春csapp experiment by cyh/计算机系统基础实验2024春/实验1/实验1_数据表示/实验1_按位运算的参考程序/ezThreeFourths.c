#ifdef PROTOTYPE
int ezThreeFourths(int);
int test_ezThreeFourths(int);
#endif
#ifdef DECL
 {"ezThreeFourths", (funct_t) ezThreeFourths, (funct_t) test_ezThreeFourths, 1,
    "! ~ & ^ | + << >>", 12, 3,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
#ifdef FIX
  int threex = ((x << 1) + x);
  int bias = (threex >> 31) & 3;
  return (threex + bias) >> 2;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_ezThreeFourths(int x)
{
  return (x*3)/4;
}
#endif

