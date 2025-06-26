fun evenP (0 : int) : bool = true
    | evenP 1 = false
    | evenP n = evenP (n - 2);

fun oddP 0 = false
    | oddP 1 = true
    | oddP n = oddP (n - 2);

evenP 3;
oddP 4;