(*函数类型：((‘a -> ‘b) * ‘a list) -> ‘b list*)
(*函数功能：实现整数集的数学变换(如翻倍、求平方或求阶乘)*)

fun mapList (f,[])=[]
|   mapList (f,x::L)=  f(x):: mapList(f,L);

fun triple test = 3 * test ;
val alist = [3, 4, 5, 6];
val ans = mapList(triple,alist);
(* ans 若等于 [9, 12, 15, 18] 说明功能正确 *)