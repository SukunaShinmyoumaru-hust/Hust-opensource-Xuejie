#ifdef PROTOTYPE
int isNonNegative(int);
int test_isNonNegative(int);
#endif
#ifdef DECL
 {"isNonNegative", (funct_t) isNonNegative, (funct_t) test_isNonNegative, 1,
    "! ~ & ^ | + << >>", 6, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
#ifdef FIX
    return ((~x)>>31) & 0x1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isNonNegative(int x) {
  return x >= 0;
}
#endif
