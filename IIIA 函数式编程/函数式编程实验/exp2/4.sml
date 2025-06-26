fun divisibleByThree 0 = true
    | divisibleByThree 1 = false
    | divisibleByThree 2 = false
    | divisibleByThree n = divisibleByThree (n - 3);

divisibleByThree 4;
divisibleByThree 23;
divisibleByThree 542;
divisibleByThree 333;