#ifdef PROTOTYPE
int allOddBits();
int test_allOddBits();
#endif
#ifdef DECL
 {"allOddBits", (funct_t) allOddBits, (funct_t) test_allOddBits, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
#ifdef FIX
  int m8  = 0x55;
  int m16 = m8  | m8  << 8;
  int m32 = m16 | m16 <<16;
  int fillx = x | m32;
  return !~fillx;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_allOddBits(int x) {
  int i;
  for (i = 1; i < 32; i+=2)
      if ((x & (1<<i)) == 0)
	  return 0;
  return 1;
}
#endif
