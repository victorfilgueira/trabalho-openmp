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
    int num_threads, meu_ranque, inicio, salto;

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

#pragma omp parallel private(meu_ranque, inicio, salto, i) reduction(+ : cont)
    {
        meu_ranque = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        inicio = 3 + meu_ranque * 2;
        salto = num_threads * 2;

        for (i = inicio; i <= n; i += salto)
        {
            if (primo(i) == 1)
                cont++;
        }
    }

    total = cont + 1; /* Acrescenta o dois, que também é primo */

    t_final = omp_get_wtime();

    printf("Quant. de primos entre 1 e n: %d \n", total);
    printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);

    return 0;
}
