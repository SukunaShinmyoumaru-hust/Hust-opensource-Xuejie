#ifdef PROTOTYPE
int bitCount(int);
int test_bitCount(int);
#endif
#ifdef DECL
 {"bitCount", (funct_t) bitCount, (funct_t) test_bitCount, 1, "! ~ & ^ | + << >>", 40, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
#ifdef FIX  
    /* Sum 8 groups of 4 bits each */
    int m1 = 0x11 | (0x11 << 8);
    int mask = m1 | (m1 << 16);
    int s = x & mask;
    s += x>>1 & mask;
    s += x>>2 & mask;
    s += x>>3 & mask;
    /* Now combine high and low order sums */
    s = s + (s >> 16);

    /* Low order 16 bits now consists of 4 sums,
       each ranging between 0 and 8.
       Split into two groups and sum */
    mask = 0xF | (0xF << 8);
    s = (s & mask) + ((s >> 4) & mask);
    return (s + (s>>8)) & 0x3F;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bitCount(int x) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i++)
    result +=  (x >> i) & 0x1;
  return result;
}
#endif
