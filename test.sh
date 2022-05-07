#!/bin/bash

assert() {
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    cc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected. But got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 100 " 100 + 5 - 4 - 1 "
assert 0 "1-1"
assert 20 "4/2*10"
assert 6 "2*(4+2)/2"
assert 2 "-1+3"
# equal
assert 1 "1 == 1"
assert 0 "2 == 1"
# neq
assert 0 "1 != 1"
assert 1 "2 != 1"
# less, leq, greater, geq
assert 1 "200 > 100 + 99"
assert 0 "100 >= 100 + 99"
assert 0 "200 < 100 + 99"
assert 1 "100 <= 100 + 99"
# combined
assert 1 "(100 > 1) + (100 != 1) == 2"


echo OK
