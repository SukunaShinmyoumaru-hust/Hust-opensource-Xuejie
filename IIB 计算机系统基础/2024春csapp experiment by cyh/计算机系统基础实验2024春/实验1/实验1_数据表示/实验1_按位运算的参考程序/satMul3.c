#ifdef PROTOTYPE
int satMul3(int);
int test_satMul3(int);
#endif
#ifdef DECL
 {"satMul3", (funct_t) satMul3, (funct_t) test_satMul3, 1,
    "~ & ^ | + << >>", 25, 3,
  {{TMin,TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * satMul3 - multiplies by 3, saturating to Tmin or Tmax if overflow
 *  Examples: satMul3(0x10000000) = 0x30000000
 *            satMul3(0x30000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0x70000000) = 0x7FFFFFFF (Saturate to TMax)
 *            satMul3(0xD0000000) = 0x80000000 (Saturate to TMin)
 *            satMul3(0xA0000000) = 0x80000000 (Saturate to TMin)
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 3
 */
int satMul3(int x) {
#ifdef FIX
    int sign = x >> 31;
    int tmin = 1 << 31;
    int tmax = ~tmin;
    int overflow = 0;
    int threex, twox = x << 1;
    overflow = x ^ twox;
    threex = twox + x;
    overflow |= twox ^ threex;
    overflow >>= 31;
    
    return (overflow & ((sign & tmin) | (~sign & tmax))) | (~overflow & threex);
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_satMul3(int x)
{
  if ((x+x+x)/3 != x)
    return x < 0 ? 0x80000000 : 0x7FFFFFFF;
  else
    return 3*x;
}
#endif

