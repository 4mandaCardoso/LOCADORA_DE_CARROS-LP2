#ifndef ALUGUEL_H
#define ALUGUEL_H

/* BIBLIOTECAS DE ALUGUEL - TUDO QUE ENVOLVER ALUGUEL*/
#define TAM_DATA 11
#define TAM_STATUS 20

typedef struct Aluguel
{
    int id;

    int idCliente;
    int idCarro;

    char dataRetirada[TAM_DATA];
    char dataDevolucao[TAM_DATA];

    int dias;

    float valorTotal;

    char status[TAM_STATUS];

    struct Aluguel *prox;

} Aluguel;


#endif