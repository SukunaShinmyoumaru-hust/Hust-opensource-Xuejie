#ifdef PROTOTYPE
int allEvenBits();
int test_allEvenBits();
#endif
#ifdef DECL
 {"allEvenBits", (funct_t) allEvenBits, (funct_t) test_allEvenBits, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
#ifdef FIX
  int m8  = 0xAA;
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
int test_allEvenBits(int x) {
  int i;
  for (i = 0; i < 32; i+=2)
      if ((x & (1<<i)) == 0)
	  return 0;
  return 1;
}
#endif
