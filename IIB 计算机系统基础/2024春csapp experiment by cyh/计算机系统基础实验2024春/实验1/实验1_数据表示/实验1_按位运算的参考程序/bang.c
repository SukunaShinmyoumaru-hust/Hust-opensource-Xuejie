#ifdef PROTOTYPE
int bang(int);
int test_bang(int);
#endif
#ifdef DECL
 {"bang", (funct_t) bang, (funct_t) test_bang, 1,
    "~ & ^ | + << >>", 12, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
#ifdef FIX
  int minus_x = ~x+1;
  /* Cute trick: 0 is the only value of x
   * for which neither x nor -x are negative */
  return (~(minus_x|x) >> 31) & 1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_bang(int x)
{
  return !x;
}
#endif

