#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

typedef struct No
{
    void *dado;
    struct No *ant; // duplamente encadeada
    struct No *prox;// duplamente encadeada
} No; 

typedef struct
{
    No *cabeca; 
    No *cauda; 
} Lista; 

//========================
// Lista
//========================
Lista *criarLista();
void inserirInicio(Lista *lista, void *dado);
void inserirFinal(Lista *lista, void *dado);
void *buscar(Lista *lista, void *chave, int (*comparar)(void *, void *));
int remover(Lista *lista, void *chave, int (*comparar)(void *, void *));
void destruirLista(Lista *lista);

//========================
// Arquivos
//========================
void salvarLista(Lista *lista, char arquivo[], size_t tamanho);
void carregarLista(Lista *lista, char arquivo[], size_t tamanho);

//========================
// Auxiliares
//========================
void limparTela();
void pausar();
void limparBuffer();
void remover_quebra_linha(char *dadoRecebido);
int listaVazia(Lista *lista);
int tamanhoLista(Lista *lista);

typedef enum {
    TIPO_CLIENTE, 
    TIPO_CARRO,
    TIPO_ALUGUEL
} TipoLista;

int gerarID(TipoLista tipo);
void sincronizarID(TipoLista tipo, int maior_id_encontrado);

#endif