#ifdef PROTOTYPE
int isPositive(int);
int test_isPositive(int);
#endif
#ifdef DECL
 {"isPositive", (funct_t) isPositive, (funct_t) test_isPositive, 1,
    "! ~ & ^ | + << >>", 8, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
#ifdef FIX
    return !((!x) | (x >> 31));
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isPositive(int x) {
  return x > 0;
}
#endif
