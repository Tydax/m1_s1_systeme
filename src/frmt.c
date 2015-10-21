#include <stdio.h>
#include <stdlib.h>

#include <driver.h>

int main()
{
    int i;

    for(i = 0; i < HDA_MAXCYLINDER; i++) {
        format_sector(i, 0, HDA_MAXSECTOR, 0xCA);
    }

    return 0;
}