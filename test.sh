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

echo OK
