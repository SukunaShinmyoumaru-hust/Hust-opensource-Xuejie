#ifdef PROTOTYPE
int conditional(int, int, int);
int test_conditional(int, int, int);
#endif
#ifdef DECL
 {"conditional", (funct_t) conditional, (funct_t) test_conditional, 3, "! ~ & ^ | << >>", 16, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
#ifdef FIX  
  int notx = !x;
  int mask = notx + ~0L;
  return (y & mask) | (z & ~mask);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_conditional(int x, int y, int z)
{
  return x?y:z;
}
#endif
