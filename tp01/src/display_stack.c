#include <stdio.h>

/*
 * Displays the current values of ebp and esp.
 * func_name: the string to display before the values.
 */
void display_stack(char* func_name) {
    unsigned int ebp, esp;

    /* Save the ebp and esp values inside the specified variables above */
    asm("movl %%ebp, %0"
        "\n\t"
        "movl %%esp, %1"
      : "=r" (ebp),
        "=r" (esp));
    
    printf("%s:\n\tebp: %u, esp: %u", func_name, ebp, esp);
}

/*
 * Displays the specified j times.
 * string: the string to display
 * j: number of times
 */
void f(char * string, int j) {
    printf("\n/* Appel à f */\n");
    int i;
    char s;

    display_stack("f");
    /* Sorry for the ugly casts... */
    printf("\n\t&firstvar: %u, &lastvar: %u\n\t&firstparam: %u, &lastparam: %u\n",
            (int) &i, (int) &s, (int) &j, (int) &string);

    for (i = 0; i < j; ++i) {
        printf("%s %d ", string, i);
    }
    printf("\n");
}

/*
 * Calls the f function with the specified parameters.
 */
void g(char * string, int j) {
    printf("\n/* Appel à g */\n");
    int i;
    char s;

    display_stack("g");
    printf("\n\t&firstvar: %u, &lastvar: %u\n\t&firstparam: %u, &lastparam: %u\n",
        (int) &i, (int) &s, (int) &j, (int) &string);
    f(string, j);
}

int main() {
    printf("/* Appel à main */\n");


    display_stack("main");
    printf("\n");

    f("f", 3);

    g("f inside g", 2);

    return 0;
}