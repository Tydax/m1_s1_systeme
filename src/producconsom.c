#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <ctx.h>
#include <sem.h>


/* Max size of the buffer */
#define MAX 100

static struct sem_s * mutex,
             * vide,
             * plein;

static int i;

void producer() {

}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    /* code */
    return 0;
}