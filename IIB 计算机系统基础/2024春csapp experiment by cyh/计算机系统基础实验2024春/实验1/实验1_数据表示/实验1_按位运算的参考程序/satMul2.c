#ifdef PROTOTYPE
int satMul2(int);
int test_satMul2(int);
#endif
#ifdef DECL
 {"satMul2", (funct_t) satMul2, (funct_t) test_satMul2, 1,
    "~ & ^ | + << >>", 20, 3,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
#ifdef FIX
  int twox = x << 1;
  int mask = (x ^ twox) >> 31;
  int sat_val = (1 << 31) + (twox >> 31);
  return (twox & ~mask) | (sat_val & mask);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_satMul2(int x)
{
  if ((x+x)/2 != x)
    return x < 0 ? 0x80000000 : 0x7FFFFFFF;
  else
    return 2*x;
}
#endif

