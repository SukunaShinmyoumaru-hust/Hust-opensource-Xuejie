#ifdef PROTOTYPE
int logicalNeg(int);
int test_logicalNeg(int);
#endif
#ifdef DECL
 {"logicalNeg", (funct_t) logicalNeg, (funct_t) test_logicalNeg, 1,
    "~ & ^ | + << >>", 12, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
#ifdef FIX
  int minus_x = ~x+1;
  return ~((minus_x|x) >> 31) & 1;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_logicalNeg(int x)
{
  return !x;
}
#endif
