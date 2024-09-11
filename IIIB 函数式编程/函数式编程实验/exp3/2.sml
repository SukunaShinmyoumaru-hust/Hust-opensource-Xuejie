(* 补充树的类型定义 *)
datatype tree = Empty | Node of tree * int * tree;

(*中序遍历*)
fun trav(Empty) = []
    |trav(Node(t1,x,t2)) = trav(t1)@[x]@trav(t2);

(* 编写函数revT: tree -> tree，对树进行反转，使trav(revT t) = reverse(trav t) *)
(* 实则就是交换每个结点的非空左右子树 *)
fun revT Empty = Empty
  | revT(Node(t1,x,t2)) =  Node(revT t2, x, revT t1);


(* 以下用于测试与演示 *)
fun split [] = ([],0,[])
    | split [x] = ([],x,[])
    | split (x::L) =
        let
            val (L1,y,L2) = split L
        in
            if length(L1) > length(L2) then
                (L1, x, y::L2)
            else
                (y::L1, x, L2)
        end;

fun listToTree [] = Empty
    | listToTree [x] = Node(Empty, x, Empty)
    | listToTree L =
        let
            val (L1,y,L2) = split L
        in
            Node(listToTree L1, y, listToTree L2)
        end;

fun trav(Empty) = []
    |trav(Node(t1,x,t2)) = trav(t1)@[x]@trav(t2);

val list1 = [1,3,5,7,9];
val bal_Tree1 = listToTree(list1);
val ans = revT bal_Tree1;

(* 对比下下面两个list，则验证功能成功 *)
val treeTolist1 = trav bal_Tree1;
val ans_list = trav ans;

(* n为结点数, d为平衡二叉树层数: O(d) = O(log(n)) *)
(* W(revT) = O(n) *) (* a(d) = 2a(d-1) + 1; d = log(n); 2^^d = n *)
(* Span(revT) = depth of tree = O(d) = O(log(n)) *)