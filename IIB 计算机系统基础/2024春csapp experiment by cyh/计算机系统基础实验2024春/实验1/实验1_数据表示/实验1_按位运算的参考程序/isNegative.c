#ifdef PROTOTYPE
int isNegative(int);
int test_isNegative(int);
#endif
#ifdef DECL
 {"isNegative", (funct_t) isNegative, (funct_t) test_isNegative, 1,
    "! ~ & ^ | + << >>", 6, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isNegative - return 1 if x < 0, return 0 otherwise 
 *   Example: isNegative(-1) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNegative(int x) {
#ifdef FIX
    return (x>>31) & 0x1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isNegative(int x) {
  return x < 0;
}
#endif
