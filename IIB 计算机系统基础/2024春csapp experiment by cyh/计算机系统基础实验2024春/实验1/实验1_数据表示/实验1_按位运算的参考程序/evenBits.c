#ifdef PROTOTYPE
int evenBits();
int test_evenBits();
#endif
#ifdef DECL
 {"evenBits", (funct_t) evenBits, (funct_t) test_evenBits, 0,
    "! ~ & ^ | + << >>", 8, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
#ifdef FIX
  int byte = 0x55;
  int word = byte | byte<<8;
  return word | word<<16;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_evenBits(void) {
  int result = 0;
  int i;
  for (i = 0; i < 32; i+=2)
    result |= 1<<i;
  return result;
}
#endif
