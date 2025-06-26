fun unit_sum (x, []) = []
	| unit_sum(x, y::A) = (x+y)::unit_sum(x, A)
fun PrefixSum [] = []
	| PrefixSum (x::L) = x::unit_sum(x, PrefixSum(L));

PrefixSum [2, 4, 6, 8];

fun fastPrefixSum [] = []
	| fastPrefixSum [x] = [x]
	| fastPrefixSum (x::y::L) = x::fastPrefixSum((x + y)::L);

PrefixSum [2, 4, 6, 8];

