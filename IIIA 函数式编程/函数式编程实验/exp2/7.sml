fun reverse (x :: L) = reverse(L) @ [x]
    | reverse ([]) = [];

reverse [1, 3, 4, 5, 8];


fun helper (x :: L1, L2) = helper (L1, x :: L2)
    | helper ([], L2) = L2;

fun reverse' (L) = helper(L, []);

reverse' [1, 3, 4, 5, 8];
