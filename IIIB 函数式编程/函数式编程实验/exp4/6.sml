(* 补充定义数据类型：树 tree *)
datatype 'a tree = Empty | Node of 'a tree * 'a * 'a tree;

(*函数类型： (‘a -> bool) -> ‘a tree -> ‘a option tree*)
(*函数功能： 将树中满足条件P（ ‘a -> bool ）的节点封装成option类型保留，否则替换成NONE*)

(* 仿照1中的思想，进行构造 *)
fun treeFilter f Empty=Empty
    | treeFilter f (Node(left,x,right))=
    if (f x) then 
        Node(treeFilter f left,SOME x,treeFilter f right)
    else
        Node(treeFilter f left, NONE,treeFilter f right);


fun test temp= (temp mod 3) = 1;
(* 模3测试 *)

fun trav(Empty) = [] 
    | trav(Node(L, v, R)) = trav(L)@v::trav(R);

val tree1 = Node(Node(Empty,5,Empty), 10,Node(Empty,7,Empty));
val tree2 = treeFilter test tree1;
