(* 补充树的类型定义 *)
datatype tree = Empty | Node of tree * int * tree;
(*Node(t1, x, t2) (t1, t2: tree, x: integer)*)
(* length 是定义在表上的库函数 *)

(* 补充split函数的定义 *)
(* 作用：将一个列表分割为（L1,X,L2）,且length(L1)和length(L1)相差小于等于1 *)
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

(*将一个表转换成一棵平衡树。*)
fun listToTree [] = Empty
    | listToTree [x] = Node(Empty, x, Empty)
    | listToTree L =
        let val (L1,y,L2) = split L
        in Node(listToTree L1, y, listToTree L2)
        end;


(* 以下用于测试 *)
val list1=[1,3,5,7,9];
val list2=[2,4,6,8,10];
val tree1=listToTree(list1);
val tree2=listToTree(list2);

(* 用于展示输出结果，以中序遍历为例 *)
(*中序遍历*)
fun trav(Empty) = []
    |trav(Node(t1,x,t2)) = trav(t1)@[x]@trav(t2);

val treeTolist1 = trav tree1;
val treeTolist2 = trav tree2;

val test = [8, 6, 9, 3, 5, 1, 10];
val tmp_tree = listToTree test;
val test_list = trav tmp_tree;