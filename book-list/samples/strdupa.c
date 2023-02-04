#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void foo(const char *str)
{
#ifndef _GNU_SOURCE
    char *my_str = strdup(str);
    printf("using strdup(): ");
#else
    char *my_str = strdupa(str);
    printf("using strdupa(): ");
#endif

    /* do something using my_str */
    printf("%s\n", my_str);

#ifndef _GNU_SOURCE
    free(my_str);
#endif
}

int main(void)
{
    foo("hello");
    return 0;
}

