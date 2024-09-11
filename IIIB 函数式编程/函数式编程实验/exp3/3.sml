(* 补充树的类型定义 *)
datatype tree = Empty | Node of tree * int * tree;
(* 补充order的定义 *)
datatype order = GREATER | EQUAL | LESS;

fun compare(x:int, y:int):order =
    if x<y then LESS else
    if y<x then GREATER else EQUAL;

(*在树中查找节点*)
fun binarySearch(Empty,_) = false
    |binarySearch(Node(left,root,right), input) =
    case compare(root,input) of
        GREATER => binarySearch(right,input)
        | LESS => binarySearch(left,input);
        | EQUAL => true;


fun Ins(x,Empty) = Node(Empty,x,Empty)
    |Ins(x,Node(left,root,right))=
        case compare(x,root) of
        GREATER=>Node(left,root,Ins(x,right))
        | LESS   =>Node(Ins(x,left),root,right);
        | EQUAL => Node(Ins(x,left),root,right);

(*创建一颗排序树*)
fun create_SortedTree[] = Empty
    |create_SortedTree(x::L)=Ins(x,create_SortedTree(L));

fun trav(Empty)=[]
    |trav(Node(t1,x,t2))=trav(t1)@[x]@trav(t2);

val list=[1,2,3,4,5,6];
val sortedTree = create_SortedTree(list);


binarySearch (sortedTree,7);

