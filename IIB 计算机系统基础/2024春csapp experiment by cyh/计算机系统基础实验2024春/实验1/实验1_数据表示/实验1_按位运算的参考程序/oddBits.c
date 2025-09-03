#ifdef PROTOTYPE
int oddBits();
int test_oddBits();
#endif
#ifdef DECL
 {"oddBits", (funct_t) oddBits, (funct_t) test_oddBits, 0,
    "! ~ & ^ | + << >>", 8, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int oddBits(void) {
#ifdef FIX
  int byte = 0xAA;
  int word = byte | byte<<8;
  return word | word<<16;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_oddBits(void) {
  int result = 0;
  int i;
  for (i = 1; i < 32; i+=2)
    result |= 1<<i;
  return result;
}
#endif
