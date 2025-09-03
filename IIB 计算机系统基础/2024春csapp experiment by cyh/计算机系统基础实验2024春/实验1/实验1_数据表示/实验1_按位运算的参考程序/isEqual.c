#ifdef PROTOTYPE
int isEqual(int, int);
int test_isEqual(int, int);
#endif
#ifdef DECL
 {"isEqual", (funct_t) isEqual, (funct_t) test_isEqual, 2,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
#ifdef FIX
  return !(x ^ y);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isEqual(int x, int y)
{
  return x == y; 
}
#endif
