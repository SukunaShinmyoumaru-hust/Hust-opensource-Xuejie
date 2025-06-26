fun interleave (x :: L1, y :: L2) = x :: (y :: interleave(L1, L2))
    | interleave (x :: L1, []) = x :: interleave(L1, [])
    | interleave ([], y :: L2) = y :: interleave([], L2)
    | interleave ([], []) = [];

interleave([2],[4]);
interleave([2,3],[4,5]);
interleave([2,3],[4,5,6,7,8,9]);
interleave([2,3],[ ]);