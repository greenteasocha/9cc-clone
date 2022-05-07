#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ref: http://www9.plala.or.jp/sgwr-t/lib/strncmp.html
// strncmpを使ったことがないので使う
int main(int argc, char **argv)
{

    printf(
        "%d\n",
        strncmp("abc", "abc", 3));

    char *p = "abc";
    printf(
        "%d\n",
        strncmp(p, "abc", 3));

    printf(
        "%d\n",
        strncmp(p, "abd", 3));
    return 0;
}