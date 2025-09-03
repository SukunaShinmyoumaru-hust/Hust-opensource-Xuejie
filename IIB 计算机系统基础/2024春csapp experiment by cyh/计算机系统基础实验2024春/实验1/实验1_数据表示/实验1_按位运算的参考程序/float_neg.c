#ifdef PROTOTYPE
unsigned float_neg(unsigned);
unsigned test_float_neg(unsigned);
#endif
#ifdef DECL
 {"float_neg", (funct_t) float_neg, (funct_t) test_float_neg, 1,
    "$", 10, 2,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
#ifdef FIX
	unsigned mask = 1 << 31;
	unsigned result = uf ^ mask;
	unsigned abs = uf & ~mask;
	if (abs > 0x7F800000){
	     /* NaN */
	     result = uf;
	}
	return result;
#else
	return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_float_neg(unsigned uf) {
    float f = u2f(uf);
    float nf = -f;
    if (isnan(f))
	return uf;
    else
	return f2u(nf);
}
#endif
