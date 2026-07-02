#ifndef CLIENTES_H
#define CLIENTES_H

#include "util.h"

/* BIBLIOTECAS DE CLIENTES - TUDO QUE ENVOLVER CLIENTES*/

#define TAM_NOME 100
#define TAM_CPF 15
#define TAM_IDADE 4

typedef struct
{
    int id;
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char idade[TAM_IDADE];
} Cliente;

int compara_cliente_cpf(void *dado, void *chave);
int validar_cpf(Lista *lista_clientes, char *cpf, int id_cliente_atual);

void cadastrar_cliente(Lista *lista_clientes);
void listar_clientes(Lista *lista_clientes);
void buscar_cliente(Lista *lista_clientes);
void remover_cliente(Lista *lista_clientes);
void atualizar_cliente(Lista *lista_clientes);

#endif