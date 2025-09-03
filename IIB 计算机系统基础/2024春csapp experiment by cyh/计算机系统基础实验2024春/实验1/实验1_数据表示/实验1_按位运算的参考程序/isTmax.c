#ifdef PROTOTYPE
int isTmax(int);
int test_isTmax(int);
#endif
#ifdef DECL
 {"isTmax", (funct_t) isTmax, (funct_t) test_isTmax, 1, "! ~ & ^ | +", 10, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
#ifdef FIX
    int nx = ~x;
    int nxnz = !!nx;
    int nxovf = !(nx+nx);
    return nxnz & nxovf;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_isTmax(int x) {
    return x == 0x7FFFFFFF;
}
#endif
