#ifdef PROTOTYPE
int isAsciiDigit(int);
int test_isAsciiDigit(int);
#endif
#ifdef DECL
 {"isAsciiDigit", (funct_t) isAsciiDigit, (funct_t) test_isAsciiDigit, 1,
    "! ~ & ^ | + << >>", 15, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
#ifdef FIX
  int bias1 = ~0x2F;
  int bias2 =  0x3a;
  int lower = x + bias1;
  int upper = ~x + bias2;
  return !((lower|upper) >> 31);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isAsciiDigit(int x) {
  return (0x30 <= x) && (x <= 0x39);
}
#endif
