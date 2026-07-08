#ifndef CLIENTES_H
#define CLIENTES_H

#include "util.h"

// Definições de tamanho para os campos do cliente
#define TAM_NOME 100
#define TAM_CPF 50
#define TAM_IDADE 20

// Estrutura que representa um cliente
typedef struct
{
    int id;
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char idade[TAM_IDADE];
} Cliente;


//funções de validação e comparação
int compara_cliente_cpf(void *dado, void *chave);
int validar_cpf(Lista *lista_clientes, char *cpf, int id_cliente_atual);
int validar_nome(Lista *lista_clientes, char *nome, int id_cliente_atual);
int validar_idade(Lista *lista_clientes, char *idade, int id_cliente_atual);

//funções de cadastro, listagem, busca, remoção e atualização (CRUD)
void cadastrar_cliente(Lista *lista_clientes);
void listar_clientes(Lista *lista_clientes);
void buscar_cliente(Lista *lista_clientes);
void remover_cliente(Lista *lista_clientes);
void atualizar_cliente(Lista *lista_clientes);


#endif

