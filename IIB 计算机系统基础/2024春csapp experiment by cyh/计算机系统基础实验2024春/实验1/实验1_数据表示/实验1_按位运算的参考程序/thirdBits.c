#ifdef PROTOTYPE
int thirdBits();
int test_thirdBits();
#endif
#ifdef DECL
 {"thirdBits", (funct_t) thirdBits, (funct_t) test_thirdBits, 0,
    "! ~ & ^ | + << >>", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
#ifdef FIX
  int bits9 = 0x49;
  int bits18 = bits9 | (bits9<<9);
  return bits18 | (bits18<<18);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_thirdBits(void) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i+=3)
    result |= 1<<i;
  return result;
}
#endif
