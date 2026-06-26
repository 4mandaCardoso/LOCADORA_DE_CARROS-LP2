#ifndef CLIENTES_H
#define CLIENTES_H

/* BIBLIOTECAS DE CLIENTES - TUDO QUE ENVOLVER CLIENTES*/

#define TAM_NOME 100
#define TAM_CPF 15
#define TAM_TELEFONE 20

typedef struct Cliente
{
    int id;
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char telefone[TAM_TELEFONE];

    struct Cliente *prox;

} Cliente;

//usar lista encadeada com cabeça!



#endif