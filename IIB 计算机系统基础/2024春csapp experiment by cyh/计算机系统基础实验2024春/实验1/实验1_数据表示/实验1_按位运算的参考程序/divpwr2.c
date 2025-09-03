#ifdef PROTOTYPE
int divpwr2(int, int);
int test_divpwr2(int, int);
#endif
#ifdef DECL
 {"divpwr2", (funct_t) divpwr2, (funct_t) test_divpwr2, 2,
    "! ~ & ^ | + << >>", 15, 2,
  {{TMin, TMax},{0,30},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
#ifdef FIX
    /* Handle rounding by generating bias:
       0 when x >= 0
       2^n-1 when x < 0
    */
    int mask = (1 << n) + ~0;
    int bias = (x >> 31) & mask;
    return (x+bias) >> n;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_divpwr2(int x, int n)
{
    int p2n = 1<<n;
    return x/p2n;
		
}
#endif
