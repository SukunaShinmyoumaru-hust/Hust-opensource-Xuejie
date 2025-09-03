#ifdef PROTOTYPE
int howManyBits(int);
int test_howManyBits(int);
#endif
#ifdef DECL
 {"howManyBits", (funct_t) howManyBits, (funct_t) test_howManyBits, 1, "! ~ & ^ | + << >>", 90, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
#ifdef FIX
    int sign, pos, bias;
    
    sign = x>>31;

    /* if negative, don't negate, just flip bits */
    x = (sign & (~x)) | (~sign & x);

    /* bias=1 if x==0 */
    bias = !(x^0);

    pos = (!!(x >> 16)) << 4;
    pos |= (!!(x >> (pos + 8))) << 3;
    pos |= (!!(x >> (pos + 4))) << 2;
    pos |= (!!(x >> (pos + 2))) << 1;
    pos |= x >> (pos + 1);

    return (pos + 2 + (~bias + 1));
#else
  return 0;
#endif
}
#endif
#ifdef TEST
int test_howManyBits(int x) {
    unsigned int a, cnt;
    
    x = x<0 ? -x-1 : x;
    a = (unsigned int)x;
    for (cnt=0; a; a>>=1, cnt++)
        ;

    return (int)(cnt + 1);
}
#endif
