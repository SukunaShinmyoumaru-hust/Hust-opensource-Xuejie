#ifdef PROTOTYPE
unsigned float_i2f(int);
unsigned test_float_i2f(int);
#endif
#ifdef DECL
 {"float_i2f", (funct_t) float_i2f, (funct_t) test_float_i2f, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
#ifdef FIX
  unsigned sign = (x < 0);
  unsigned ax = (x < 0) ? -x : x;
  unsigned exp = 127+31;
  unsigned residue;
  unsigned frac = 0;
  if (ax == 0) {
    exp = 0;
    frac = 0;
  } else {
    /* Normalize so that msb = 1 */
    while ((ax & (1<<31)) == 0) {
      ax = ax << 1;
      exp--;
    }
    /* Now have Bit 31 = MSB (becomes implied leading one)
       Bits 8-30 are tentative fraction,
       Bits 0-7 require rounding.
    */
    residue = ax & 0xFF;
    frac = (ax >> 8) & 0x7FFFFF; /* 23 bits */
    if (residue > 0x80 || (residue == 0x80 && (frac & 0x1))) {
      /* Round up */
      frac ++;
      /* Might need to renormalize */
      if (frac > 0x7FFFFF) {
	frac = (frac & 0x7FFFFF) >> 1;
	exp++;
      }
    }
  }
  return (sign << 31) | (exp << 23) | frac;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_float_i2f(int x) {
  float f = (float) x;
  return f2u(f);
}
#endif
