#include <stdio.h>

int main(void)
{
    int sum = 0;

    printf("PRINTING THE SUMMATION OF 10\n");
    printf("----------------------------\n");

    for (int i = 1; i <= 10; i++) {
        printf("%d + %d = ", sum, i);
        sum += i;
        printf("%d\n", sum);
    }
    printf("Summation of 10 = %d\n", sum);


    return 0;
}
