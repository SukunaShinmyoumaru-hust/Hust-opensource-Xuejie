(* map: ('a -> 'b) -> 'a list -> 'b list *)
fun map f [ ] = [ ]
    | map f (x::xs) = (f x) :: (map f xs)
(* 函数作用：将list中按需求分离 *)

(* filter: ('a -> bool) -> 'a list -> 'a list *)
fun filter f [] = []
    | filter f (x::xs) = if f x then x :: (filter f xs)
			     else filter f xs;
(* 函数作用：满足f函数时保留，其余的都过滤掉 *)

map (filter (fn a => size a = 4)) [["Sunday", "Monday"], ["one", "two", "three", "four", "five"], ["year", "month", "day"]];

(* 按照要求，过滤掉其余的，只保留住长度为4的string *)