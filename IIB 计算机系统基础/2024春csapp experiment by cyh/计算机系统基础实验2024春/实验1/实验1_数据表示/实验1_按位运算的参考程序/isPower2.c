#ifdef PROTOTYPE
int isPower2(int);
int test_isPower2(int);
#endif
#ifdef DECL
 {"isPower2", (funct_t) isPower2, (funct_t) test_isPower2, 1, "! ~ & ^ | + << >>", 20, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
#ifdef FIX
  /* Solve using trick: x & (x-1) == 0 only when x==0 or has single 1 bit */
  int xm1 = x + ~0;  // Decrement x
  int possible = !(x&xm1);
  int result = possible & !!x & !(x & (1<<31));
  return result;
#if 0
    int result; 

    /* 1. Count the number of 1 bits */

    /* Sum 8 groups of 4 bits each */
    int bitcount;
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
    bitcount = (s + (s>>8)) & 0x3F;

    /* 2. If bitcount is 1 and x is not Tmin, then x is 
       a power of 2 */
    result = !(~bitcount + 2) & ~(x >> 31);
    return result;
#endif

#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isPower2(int x) {
  int i;
  for (i = 0; i < 31; i++) {
    if (x == 1<<i)
      return 1;
  }
  return 0;
}
#endif
