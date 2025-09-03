#ifdef PROTOTYPE
int leastBitPos(int);
int test_leastBitPos(int);
#endif
#ifdef DECL
 {"leastBitPos", (funct_t) leastBitPos, (funct_t) test_leastBitPos, 1, "! ~ & ^ | + << >>", 6, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2 
 */
int leastBitPos(int x) {
#ifdef FIX
  /* The only bit set in both x and -x will be the least significant one */
  return x & (~x+1);
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_leastBitPos(int x) {
  int mask = 1;

  if (x == 0)
    return 0;
  while (!(mask & x)) {
    mask = mask << 1;
  }
  return mask;
}
#endif
