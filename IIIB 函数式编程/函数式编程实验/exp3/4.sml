(* 补充树的类型定义 *)
datatype tree = Empty | Node of tree * int * tree;
(*一棵minheap树定义为：	1. t is Empty;2. t is a Node(L, x, R), where R, L are minheaps and values(L),
 value(R) >= x	(value(T)函数用于获取树T的根节点的值）*)


(*函数类型： tree * tree -> order *)
(*when given two trees, returns a value of type order, 
based on which tree has a larger value at the root node*)
(* 根绝根结点的大小返回 order : LESS | GREATER | EQUAL *)
(* 思路：分为包含Empty的3中类型和均非空的第4种情况 *)
fun treecompare (Empty,Empty) = EQUAL
|   treecompare(Empty,Node(t1,x,t2)) = LESS
|   treecompare(Node(t1,x,t2),Empty) = GREATER
|   treecompare(Node(left1,x1,right1),Node(left2,x2,right2))=
    if (x1 > x2) then GREATER
    else if (x1 < x2) then LESS
    else EQUAL;


(*函数类型： tree -> tree*)
(* REQUIRES the subtrees of t are both minheaps 需要的两个子树均为 小顶堆
ENSURES swapDown(t) = if t is Empty or all of t’s immediate children are empty
then just return t, 
如果遇到空结点或者child是空结点，尽快返回
otherwise returns a minheap which contains exactly the elements in t. *)

(* 思路：前两种重载说明有empty的情况 *)
fun SwapDown Empty = Empty
| SwapDown (Node(Empty,x,Empty)) = Node(Empty,x,Empty)
| SwapDown (Node(left,x,right))=
    if treecompare(Node(left,x,right),left) = LESS andalso treecompare(Node(left,x,right),right) = LESS then Node(left,x,right)
    else if treecompare(left,right)=LESS then
        let
          val Node(l,root,r)=left
        in
          Node(Node(l,x,r),root,right)
        end
    else
        let
          val Node(l,root,r)=right
        in
          Node(left,root,Node(l,x,r))
        end;

(*函数类型：tree -> tree*)
(*函数功能：given an arbitrary tree t, evaluates to a minheap with exactly the elements of t.*)
(* 给一个任意树，转成带着元素t的 小顶堆 *)
fun heapify Empty = Empty
| heapify (Node(left,root,right))=
    let
      val temp_t1 = heapify(left)
      val temp_t2 = heapify(right)
      val temp_t3 = Node(temp_t1,root,temp_t2);
    in SwapDown(temp_t3)
    end;



(* 以下用于测试与演示 *)

val tree1 = Node(Node(Empty,123,Empty),124,Node(Empty,122,Empty));
val tree2 = Node(Empty,20211215,Empty);
val ans1 = SwapDown(tree1);

val tree3 = Node(Node(Node(Empty,70,Empty),90,Node(Empty,60,Empty)),100,Node(Empty,80,Empty));

val ans2 = heapify tree3;


(* SwapDown *)
(* W = O(d) = log(n);
   Span = O(d) = log(n);
*)

(* heapify *)
(* W = 2^^(d-1) + 2*2^^(d-2) +...+ (d-1)*2^^0 数量级 2^^d  *)
(* Span 并行可以优化掉分支部分，W的计算式的非指数的系数均被优化为1 ，数量级仍然为 2^^d *)
(* W = O(2^^d) = O(n);
   Span = O(2^^d) = O(n);
   虽然两者的复杂度一致，但是求解具体问题时，在一定范围内，
   肯定是并行并单行快一些~
*)