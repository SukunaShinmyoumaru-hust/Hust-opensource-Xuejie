#ifdef PROTOTYPE
int greatestBitPos(int);
int test_greatestBitPos(int);
#endif
#ifdef DECL
 {"greatestBitPos", (funct_t) greatestBitPos, (funct_t) test_greatestBitPos, 1, "! ~ & ^ | + << >>", 70, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
#endif
#ifdef CODE
/* 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
#ifdef FIX
    /* Set of bits being probed */
    int probe_bits = x;
    /* 1 if x != 0 */
    int position = !!x;
    /* Use divide and conquer to move position to correct spot */
    int upper_zero;
    int uz_mask;

    /* Is there a 1 in the upper 16/32 bits? */
    upper_zero = !(probe_bits >> 16);
    uz_mask = ~upper_zero + 1;
    /* If so, shift */
    position <<= (!upper_zero << 4);
    /* Merge upper and lower probe bits */
    probe_bits = (probe_bits & uz_mask) | (probe_bits >> 16);

    /* Is there a 1 in the remaining upper 8/16 bits? */
    upper_zero = !(probe_bits >> 8);
    uz_mask = ~upper_zero + 1;
    /* If so, shift */
    position <<= (!upper_zero << 3);
    /* Merge upper and lower probe bits */
    probe_bits = (probe_bits & uz_mask) | (probe_bits >> 8);

    /* Is there a 1 in the remaining upper 4/8 bits? */
    upper_zero = !(probe_bits >> 4);
    uz_mask = ~upper_zero + 1;
    /* If so, shift */
    position <<= (!upper_zero << 2);
    /* Merge upper and lower probe bits */
    probe_bits = (probe_bits & uz_mask) | (probe_bits >> 4);

    /* Is there a 1 in the remaining upper 2/4 bits? */
    upper_zero = !(probe_bits >> 2);
    uz_mask = ~upper_zero + 1;
    /* If so, shift */
    position <<= (!upper_zero << 1);
    /* Merge upper and lower probe bits */
    probe_bits = (probe_bits & uz_mask) | (probe_bits >> 2);

    /* Is there a 1 in the remaining upper 1/2 bits? */
    upper_zero = !(probe_bits >> 1);
    /* If so, shift */
    position <<= !upper_zero;
    return position;
#else
  return 2;
#endif
}
#endif
#ifdef TEST
int test_greatestBitPos(int x) {
    unsigned mask = 1<<31;
    if (x == 0)
	return 0;
    while (!(mask & x)) {
	mask = mask >> 1;
    }
    return mask;
}
#endif
