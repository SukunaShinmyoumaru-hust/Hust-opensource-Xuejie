#ifdef PROTOTYPE
int anyOddBit();
int test_anyOddBit();
#endif
#ifdef DECL
 {"anyOddBit", (funct_t) anyOddBit, (funct_t) test_anyOddBit, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
#ifdef FIX
    int m8  = 0xAA;
    int m16 = m8  | m8  << 8;
    int m32 = m16 | m16 <<16;
    int oddx = x & m32;
    return !!oddx;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_anyOddBit(int x) {
    int i;
    for (i = 1; i < 32; i+=2)
        if (x & (1<<i))
	     return 1;
    return 0;
}
#endif
