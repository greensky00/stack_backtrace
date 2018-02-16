#include "backtrace.h"

#include <signal.h>
#include <stdio.h>

int func_b(int* a) {
    int aa = *a * 2;
    return aa;
}

int func_a() {
    int* a = NULL;
    return func_b(a) + 2;
}

void (*old_handler)(int);
void sig_handler(int signum) {
    char buf[4096];
    size_t len = stack_backtrace(buf, 4096);
    printf("%.*s\n", (int)len, buf);
    signal(SIGSEGV, old_handler);
}

int main() {
    old_handler = signal(SIGSEGV, sig_handler);

    printf("%d\n", func_a());
    return 0;
}
