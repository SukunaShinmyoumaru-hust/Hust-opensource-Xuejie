#ifdef PROTOTYPE
int anyEvenBit();
int test_anyEvenBit();
#endif
#ifdef DECL
 {"anyEvenBit", (funct_t) anyEvenBit, (funct_t) test_anyEvenBit, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
#ifdef FIX
  int m8  = 0x55;
  int m16 = m8  | m8  << 8;
  int m32 = m16 | m16 <<16;
  int evenx = x & m32;
  return !!evenx;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_anyEvenBit(int x) {
  int i;
  for (i = 0; i < 32; i+=2)
      if (x & (1<<i))
	  return 1;
  return 0;
}
#endif
