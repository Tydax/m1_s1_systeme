#include <stdio.h>

void display_stack(char* func_name) {
    int ebp, esp;

    /* Save the ebp and esp values inside the specified variables above */
    asm("movl %%ebp, %0"
        "\n\t"
        "movl %%esp, %1"
      : "=r" (ebp),
        "=r" (esp));
    
    printf("%s:\nebp: %d\nesp: %d\n", func_name, ebp, esp);
}

void f() {
    printf("\n/* Appel à f */\n");
    int i;

    display_stack("f");

    for (i = 0; i < 5; ++i) {
        display_stack("f");
        printf("%d\n", i);
    }

    display_stack("f");
}

void g() {
    printf("\n/* Appel à g */\n");
    display_stack("g");

    f();

    display_stack("g");
}

int main(int argc, int * argv) {
    printf("\n/* Appel à main */\n");


    display_stack("main");

    f();

    display_stack("main");

    g();

    display_stack("main");

    return 0;
}