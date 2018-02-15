#include "backtrace.h"

#include <signal.h>
#include <stdio.h>

int func_a(int a, char b) {
    char *p = (char *)0xdeadbeef;
    a = a + b;
    *p = 10;  /* CRASH here!! */
    return 2*a;
}

int func_b() {
    int res, a = 5;
    res = 5 + func_a(a, 't');
    return res;
}

void (*old_handler)(int);

void sig_handler(int signum) {
    char buf[4096];
    size_t len = stack_backtrace(buf, 4096);
    printf("%.*s\n", (int)len, buf);
    signal(SIGSEGV, old_handler);
    //exit(0);
}

int main() {
    old_handler = signal(SIGSEGV, sig_handler);

    printf("%d\n", func_b());
    return 0;
}
