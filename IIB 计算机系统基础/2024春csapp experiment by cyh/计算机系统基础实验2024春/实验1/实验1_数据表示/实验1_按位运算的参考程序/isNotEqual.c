#ifdef PROTOTYPE
int isNotEqual(int, int);
int test_isNotEqual(int, int);
#endif
#ifdef DECL
 {"isNotEqual", (funct_t) isNotEqual, (funct_t) test_isNotEqual, 2,
    "! ~ & ^ | + << >>", 6, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
#ifdef FIX
  return !!(x ^ y);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isNotEqual(int x, int y)
{
  return x != y; 
}
#endif
