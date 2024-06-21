#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
#define TAMANHO 500000

int primo(int n)
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
{ /* mpi_primosbag.c  */
    double t_inicial, t_final;
    int cont = 0, total = 0;
    int i, n;
    int meu_ranque, num_procs, inicio, dest, raiz = 0, tag = 1, stop = 0;
    MPI_Status estado;
    /* Verifica o número de argumentos passados */
    if (argc < 2)
    {
        printf("Entre com o valor do maior inteiro como parâmetro para o programa.\n");
        return 0;
    }
    else
    {
        n = strtol(argv[1], (char **)NULL, 10);
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    /* Se houver menos que dois processos aborta */
    if (num_procs < 2)
    {
        printf("Este programa deve ser executado com no mínimo dois processos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        return (1);
    }
    /* Registra o tempo inicial de execução do programa */
    t_inicial = MPI_Wtime();
    /* Envia pedaços com TAMANHO números para cada processo */
    if (meu_ranque == 0)
    {
        // Para todo processo nós enviamos uma fatia inicial para ser calculado.
        //
        // Dentro do for é checado se nossa fatia inicial (inicio) estoura o valor a ser calculado (n)
        //  -> isso ocorre em casos que queremos calcular um n em que a divisão dele entre os processos pedidos fique impossível
        //  -> exemplo: número de processos >= 4 e n = 500003
        //      -> nesse caso vamos enviar os seguintes:
        //           processo 1 recebe n = 3
        //           processo 2 recebe n = 500003
        //           processo 3 receberia n = 1000003, que fica maior que o n que queremos calcular
        //                      daí nesse caso nós enviamos a tag 99 para finalizar esse processo e qualquer outro próximo
        //                      e somamos +1 no contador do stop, para que o while do ranque = 0 consiga finalizar de forma correta
        inicio = 3;
        for (dest = 1; dest < num_procs; dest++)
        {
            if (inicio <= n)
            {
                MPI_Send(&inicio, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                inicio += TAMANHO;
            }
            else
            {
                MPI_Send(&inicio, 1, MPI_INT, dest, 99, MPI_COMM_WORLD);
                stop++;
            }
        }
        /* Fica recebendo as contagens parciais de cada processo */
        while (stop < (num_procs - 1))
        {
            MPI_Recv(&cont, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
            total += cont;
            dest = estado.MPI_SOURCE;
            if (inicio > n)
            {
                tag = 99;
                stop++;
            }
            /* Envia um nvo pedaço com TAMANHO números para o mesmo processo*/
            MPI_Send(&inicio, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            inicio += TAMANHO;
        }
    }
    else
    {
        /* Cada processo escravo recebe o início do espaço de busca */
        while (estado.MPI_TAG != 99)
        {
            MPI_Recv(&inicio, 1, MPI_INT, raiz, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
            if (estado.MPI_TAG != 99)
            {
                for (i = inicio, cont = 0; i < (inicio + TAMANHO) && i < n; i += 2)
                    if (primo(i) == 1)
                        cont++;
                /* Envia a contagem parcial para o processo mestre */
                MPI_Send(&cont, 1, MPI_INT, raiz, tag, MPI_COMM_WORLD);
            }
        }
        /* Registra o tempo final de execução */
        t_final = MPI_Wtime();
    }
    if (meu_ranque == 0)
    {
        t_final = MPI_Wtime();
        total += 1; /* Acrescenta o 2, que é primo */
        printf("Quant. de primos entre 1 e %d: %d \n", n, total);
        printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);
    }
    /* Finaliza o programa */
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return (0);
}