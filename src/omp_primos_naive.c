#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int primo(long int n)
{
    int i;
    for (i = 3; i < (int)(sqrt(n) + 1); i += 2)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    double t_inicial, t_final;
    int cont = 0, total = 0;
    long int i, n;

    if (argc < 2)
    {
        printf("Valor inválido! Entre com um valor do maior inteiro\n");
        return 0;
    }
    else
    {
        n = strtol(argv[1], (char **)NULL, 10);
    }

    t_inicial = omp_get_wtime();

#pragma omp parallel for reduction(+ : cont) schedule(dynamic)
    for (i = 3; i <= n; i += 2)
    {
        if (primo(i) == 1)
            cont++;
    }

    t_final = omp_get_wtime();

    total = cont + 1; /* Acrescenta o dois, que também é primo */
    printf("Quant. de primos entre 1 e %ld: %d \n", n, total);
    printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);

    return 0;
}
