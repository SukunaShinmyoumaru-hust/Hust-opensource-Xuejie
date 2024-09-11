(* option的文档解释：option is a SML datatype for handling potential undefined values.
 Formally, the option type is parametrized by a single polymorphic type variable,
  so for every SML type t, there is a type t option. *)
fun exists(p) = fn L => foldr (fn (a, b) => a orelse b) false (map p L)
fun forall(p) = fn L => foldr (fn (a, b) => a andalso b) true (map p L)

(* 定义函数：判断是否是偶数 *)
fun IsEven(x) = x mod 2 = 0;

exists(IsEven) [3, 5, 7, 9, 11]; (* 不存在偶数 *)
exists(IsEven) [1, 3, 5, 8]; (* 存在偶数8 *)
forall(IsEven) [0, 2, 4, 7]; (* 存在7不是偶数 *)
forall(IsEven) [0, 2, 4, 6, 8, 10]; (* 所有数字均为偶数 *)
(* 分别对应之下要求 *)
(* exist p L =>* true if there is an x in L such that p x=true;
	         exits p L =>* false otherwise.
	         forall p L =>* true if p x = true for every item x in L;
	         forall p L =>* false otherwise.
 *)