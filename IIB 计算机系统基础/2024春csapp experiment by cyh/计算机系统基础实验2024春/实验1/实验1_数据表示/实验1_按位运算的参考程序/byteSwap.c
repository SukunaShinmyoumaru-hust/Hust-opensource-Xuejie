#ifdef PROTOTYPE
int byteSwap(int, int, int);
int test_byteSwap(int, int, int);
#endif
#ifdef DECL
{"byteSwap", (funct_t) byteSwap, (funct_t) test_byteSwap, 3,
     "! ~ & ^ | + << >>", 25, 2,
    {{TMin, TMax},{0,3},{0,3}}},
#endif
#ifdef CODE
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
#ifdef FIX
    int n8 = n << 3;
    int m8 = m << 3;
    int n_mask = 0xff << n8;
    int m_mask = 0xff << m8;

    int n_byte = (( x & n_mask ) >> n8) & 0xff;
    int m_byte = (( x & m_mask ) >> m8) & 0xff;

    int bytes_mask = n_mask | m_mask;
    int left_over  = x & ~bytes_mask;

    return left_over | (n_byte << m8) | (m_byte << n8);
#else
    return 2;
#endif
}
#endif
#ifdef TEST
int test_byteSwap(int x, int n, int m)
{
    /* little endiamachine */
    /* least significant byte stored first */

    unsigned int nmask, mmask;

    switch(n) {
    case 0:
      nmask = x & 0xFF;
      x &= 0xFFFFFF00;
      break;
    case 1:
      nmask = (x & 0xFF00) >> 8;
      x &= 0xFFFF00FF;
      break;
    case 2:
      nmask = (x & 0xFF0000) >> 16;
      x &= 0xFF00FFFF;      
      break;
    default:
      nmask = ((unsigned int)(x & 0xFF000000)) >> 24;
      x &= 0x00FFFFFF;
      break;
    }

    switch(m) {
    case 0:
      mmask = x & 0xFF;
      x &= 0xFFFFFF00;
      break;
    case 1:
      mmask = (x & 0xFF00) >> 8;
      x &= 0xFFFF00FF;
      break;
    case 2:
      mmask = (x & 0xFF0000) >> 16;
      x &= 0xFF00FFFF;      
      break;
    default:
      mmask = ((unsigned int)(x & 0xFF000000)) >> 24;
      x &= 0x00FFFFFF;
      break;
    }

    nmask <<= 8*m;
    mmask <<= 8*n;

    return x | nmask | mmask;
}
#endif
