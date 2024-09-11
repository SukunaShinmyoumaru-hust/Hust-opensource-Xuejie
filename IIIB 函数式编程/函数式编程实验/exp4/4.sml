(*函数类型： (‘a -> ‘b) -> (‘a list -> ‘b list) *)
(*函数功能：实现整数集的数学变换(如翻倍、求平方或求阶乘)*)
fun mapList' f = fn L =>
    case L of [] => []
    | x::R => (f x):: mapList' f R;

(* 测试 *)
val alist=[5,6,7,8,9,10];
(* 定义函数:triple *)
fun triple test = 3 * test ;
(* 题3中函数 maplist *)
fun mapList (f,[])=[]
|   mapList (f,x::L)=  f(x):: mapList(f,L);

(* 展示结果 *)
val showlist1 = mapList(triple,alist);
val showlist2 = (mapList' triple) alist;