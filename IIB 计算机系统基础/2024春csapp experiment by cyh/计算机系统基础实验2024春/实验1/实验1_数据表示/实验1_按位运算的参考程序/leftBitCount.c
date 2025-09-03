#ifdef PROTOTYPE
int leftBitCount(int);
int test_leftBitCount(int);
#endif
#ifdef DECL
 {"leftBitCount", (funct_t) leftBitCount, (funct_t) test_leftBitCount, 1, "! ~ & ^ | + << >>", 40, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * leftBitCount - returns count of number of consective 1's in
 *     left-hand (most significant) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Rating: 4
 */
int leftBitCount(int x) {
#ifdef FIX
    int c16, s16, x16;
    int c8, s8, x8;
    int c4, s4, x4;
    int c2, s2, x2;
    int c1, s1, x1;
    int result = 0;
    /* Check upper 16 bits */
    c16 = !((x >> 16) + 1);
    s16 = c16<<4; /* == 16 if upper 16 bits are set to 1, 0 otherwise */
    result = s16;
    /* Move lower bits up if upper bits all 1's */
    x16 = x << s16;
    /* Check upper 8 bits */
    c8 = !((x16 >> 24) + 1);
    s8 = c8<<3; /* == 8 if upper 8 bits are set to 1, 0 otherwise */
    result += s8;
    /* Move lower bits up if upper bits all 1's */
    x8 = x16 << s8;
    /* Check upper 4 bits */
    c4 = !((x8 >> 28) + 1);
    s4 = c4<<2; /* == 4 if upper 4 bits are set to 1, 0 otherwise */
    result += s4;
    /* Move lower bits up if upper bits all 1's */
    x4 = x8 << s4;
    /* Check upper 2 bits */
    c2 = !((x4 >> 30) + 1);
    s2 = c2<<1; /* == 2 if upper 2 bits are set to 1, 0 otherwise */
    result += s2;
    /* Move lower bits up if upper bits all 1's */
    x2 = x4 << s2;
    /* Check upper bit */
    c1 = (x2 >> 31) & 0x1;
    s1 = c1; /* == 1 if upper bit is set to 1, 0 otherwise */
    result += s1;
    /* Move lower bits up if upper bit is 1 */
    x1 = x2 << s1;
    /* Now just add in the MSB */
    result += (x1 >> 31) & 0x1;
    return result;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_leftBitCount(int x) {
  int result = 0;
  int i;
  for (i = 31; i >= 0; i--) {
      int bit = (x >> i) & 0x1;
      if (!bit)
	  break;
      result ++;
  }
  return result;
}
#endif
