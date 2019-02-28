#include <stdio.h>

int main(void)
{
    int fact = 1;

    printf("PRINTING THE FACTORIAL OF 10\n");
    printf("----------------------------\n");

    for (int i = 1; i <= 10; i++) {
        printf("%d * %d = ", fact, i);
        fact *= i;
        printf("%d\n", fact);
    }
    printf("Factorial of 10 = %d\n", fact);

    return 0;
}
