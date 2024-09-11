datatype 'a tree = Empty | Node of 'a tree * 'a * 'a tree;

fun treeFilter f Empty = Empty
|   treeFilter f (Node(L,x,R)) = 
    if f x then
        Node(treeFilter f L,SOME x,treeFilter f R)
    else
        Node(treeFilter f L,NONE,treeFilter f R);

fun test temp= (temp mod 2) = 1;

fun trav(Empty) = [] 
    | trav(Node(L, v, R)) = trav(L)@v::trav(R);

val tree1 = Node(Node(Empty,9,Empty), 10,Node(Empty,7,Empty));
val tree2 = treeFilter test tree1;