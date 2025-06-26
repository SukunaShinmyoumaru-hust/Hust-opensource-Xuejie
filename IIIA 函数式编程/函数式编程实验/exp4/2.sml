(*函数类型： ((int ->int) * int) -> int*)
(*函数功能： 为将一个整数通过函数变换(如翻倍、求平方或求阶乘)后再加1*)

fun thenAddOne (f,x)= f x + 1;
(* 设变换为f *)
(*测试*)

fun triple test = 3 * test;
(* 定义一个小函数，将传入数据翻为原来的3倍 *)
val a = 3;

val ans = thenAddOne(triple,a);
