#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int j, k, pid;

    printf("Hello TV Land!\n");

    if ((pid = fork()) == 0) {
        printf("Who's your daddy? My pid = %d\n", pid);

        for (j = 0; j < 10; j++) {
            system(sleep(1));
            printf("j = %d\n", j);
            k = k - 3;
        }
    } else {
        printf("Yo Mamma does! My pid = %d\n", pid);

        for(k = 0; k < 10; k++) {
            system(sleep(2));
            printf("k = %d\n", k);
            j = j - 1;
        }
    }
}