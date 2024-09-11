(* 1.sml *)
fun all(your, base) = 
    case your of 
        0 => base
        |  _ => "are belong to us"::all(your-1,base);

(* 测试使用 *)
all (2, ["sd","sd"]); 

fun funny(f,[])=0
    | funny(f,x::xs)=f(x,funny(f,xs));

(fn x => (fn y => x)) "Hello, World!";

(* 2.sml *)
datatype order = LESS | EQUAL | GREATER;  

fun compare(x:int, y:int):order =
    if x<y then LESS else
    if y<x then GREATER else EQUAL;  

fun ins (x, []) = [x]
    | ins (x, y::L) = case compare(x, y) of
		    GREATER => y::ins(x, L)
		    |   _ 	=> x::y::L;

fun isort [ ] = [ ]
   |   isort (x::L) = ins (x, isort L);
   

(* 3.sml *)
fun fib n = if n<=2 then 1 else fib(n-1) + fib(n-2);

fun fibber (0: int) : int * int = (1, 1)
  | fibber (n: int) : int * int =
      let val (x: int, y: int) = fibber (n-1)
      in (y, x + y)
      end;


(* 4.sml *)
fun divisibleByThree 0 = true
    | divisibleByThree 1 = false
    | divisibleByThree 2 = false
    | divisibleByThree n = divisibleByThree (n - 3);

divisibleByThree 4;
divisibleByThree 23;
divisibleByThree 542;
divisibleByThree 333;

(* 5.sml *)
fun evenP (0 : int) : bool = true
    | evenP 1 = false
    | evenP n = evenP (n - 2);

fun oddP 0 = false
    | oddP 1 = true
    | oddP n = oddP (n - 2);

evenP 3;
oddP 4;
(* 6.sml *)
fun interleave (x :: L1, y :: L2) = x :: (y :: interleave(L1, L2))
    | interleave (x :: L1, []) = x :: interleave(L1, [])
    | interleave ([], y :: L2) = y :: interleave([], L2)
    | interleave ([], []) = [];

interleave([2],[4]);
interleave([2,3],[4,5]);
interleave([2,3],[4,5,6,7,8,9]);
interleave([2,3],[ ]);

(* 7.sml *)
fun reverse (x :: L) = reverse(L) @ [x]
    | reverse ([]) = [];

reverse [1, 3, 4, 5, 8];

fun helper (x :: L1, L2) = helper (L1, x :: L2)
    | helper ([], L2) = L2;

fun reverse' (L) = helper(L, []);

reverse' [1, 3, 4, 5, 8];

(* 8.sml *)
fun unit_sum (x, []) = []
	| unit_sum(x, y::A) = (x+y)::unit_sum(x, A)
fun PrefixSum [] = []
	| PrefixSum (x::L) = x::unit_sum(x, PrefixSum(L));

PrefixSum [2, 4, 6, 8];

fun fastPrefixSum [] = []
	| fastPrefixSum [x] = [x]
	| fastPrefixSum (x::y::L) = x::fastPrefixSum((x + y)::L);

PrefixSum [2, 4, 6, 8];


