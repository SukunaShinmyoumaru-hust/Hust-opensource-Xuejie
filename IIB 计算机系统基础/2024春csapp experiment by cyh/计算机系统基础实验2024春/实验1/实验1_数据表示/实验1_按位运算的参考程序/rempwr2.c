#ifdef PROTOTYPE
int rempwr2(int, int);
int test_rempwr2(int, int);
#endif
#ifdef DECL
 {"rempwr2", (funct_t) rempwr2, (funct_t) test_rempwr2, 2,
    "! ~ & ^ | + << >>", 20, 3,
  {{TMin, TMax},{0,30},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int rempwr2(int x, int n) {
#ifdef FIX
    /* First compute x / (1<<n) */
    int mask = (1 << n) + ~0;
    int bias = (x >> 31) & mask;
    int quo  = (x+bias) >> n;
    /* Now multiply by (1<<n) and subtract off */
    int factor = ~(quo << n) + 1;
    return factor + x;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_rempwr2(int x, int n)
{
    int p2n = 1<<n;
    return x%p2n;
}
#endif
