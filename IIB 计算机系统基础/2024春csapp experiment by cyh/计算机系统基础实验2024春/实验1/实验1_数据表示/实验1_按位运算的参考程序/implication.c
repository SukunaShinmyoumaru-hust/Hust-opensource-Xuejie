#ifdef PROTOTYPE
int implication(int, int);
int test_implication(int, int);
#endif
#ifdef DECL
 {"implication", (funct_t) implication, (funct_t) test_implication, 2, "! ~ ^ |", 5, 2,
     {{0,1},{0,1},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
#ifdef FIX  
    return (!x)|y;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_implication(int x, int y)
{
  return !(x & (!y));
}
#endif
