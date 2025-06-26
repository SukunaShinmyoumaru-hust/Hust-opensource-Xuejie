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


