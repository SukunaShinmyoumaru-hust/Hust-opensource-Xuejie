#ifdef PROTOTYPE
int sign(int);
int test_sign(int);
#endif
#ifdef DECL
 {"sign", (funct_t) sign, (funct_t) test_sign, 1, "! ~ & ^ | + << >>", 10, 2,
     {{-TMax, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
#ifdef FIX
    return (x>>31) | (!!x);    
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_sign(int x) {
    if ( !x ) return 0;
    return (x < 0) ? -1 : 1;
}
#endif
