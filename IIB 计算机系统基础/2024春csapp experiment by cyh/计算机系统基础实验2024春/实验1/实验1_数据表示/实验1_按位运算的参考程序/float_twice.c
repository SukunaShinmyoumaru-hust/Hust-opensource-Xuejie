#ifdef PROTOTYPE
unsigned float_twice(unsigned);
unsigned test_float_twice(unsigned);
#endif
#ifdef DECL
 {"float_twice", (funct_t) float_twice, (funct_t) test_float_twice, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
#ifdef FIX
  unsigned sign = uf>>31;
  unsigned exp = uf>>23 & 0xFF;
  unsigned frac = uf & 0x7FFFFF;
  if (exp == 0) {
    /* Denormalized.  Must double fraction */
    frac = 2*frac;
    if (frac > 0x7FFFFF) {
      /* Result normalized */
      frac = frac & 0x7FFFFF; /* Chop off leading bit */
      exp = 1;
    }
  } else if (exp < 0xFF) {
    /* Normalized.  Increase exponent */
    exp++;
    if (exp == 0xFF) {
      /* Infinity */
      frac = 0;
    }
  }
  /* Infinity and NaN do not require any changes */
  return (sign << 31) | (exp << 23) | frac;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_float_twice(unsigned uf) {
  float f = u2f(uf);
  float tf = 2*f;
  if (isnan(f))
    return uf;
  else
    return f2u(tf);
}
#endif
