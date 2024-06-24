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
    int count = 0, total = 0;
    long int i, n;
    int num_threads = 1;

    if (argc < 2)
    {
        printf("Valor inválido! Entre com um valor do maior inteiro\n");
        return 0;
    }
    else if (argc == 2)
    {
        n = strtol(argv[1], (char **)NULL, 10);
    }
    else if (argc >= 3)
    {
        n = strtol(argv[1], (char **)NULL, 10);
        num_threads = strtol(argv[2], (char **)NULL, 10);
    }

    t_inicial = omp_get_wtime();

    // Definimos a quantidade de threads que serão utilizadas pelo 2 arg
    // 
    // Criamos uma diretiva parallel e for num loop que passa pelos
    // números impáres de 3 a n
    //
    // No for fazemos o cálculo se i é primo ou não e somamos no count que
    // é "reduzido" na última parte da diretiva fazendo uma soma
    omp_set_num_threads(num_threads);
    #pragma omp parallel for reduction(+ : count)
    for (i = 3; i <= n; i += 2)
    {
        if (primo(i) == 1)
            count++;
    }

    total = count + 1; /* Acrescenta o dois, que também é primo */

    t_final = omp_get_wtime();

    printf("Quant. de primos entre 1 e n: %d \n", total);
    printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);

    return 0;
}
