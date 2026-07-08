#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definições dos nomes dos arquivos gerados pelos relatorios
#define DADOS_DIR "dados/"
#define DADOS_CLIENTES_BIN DADOS_DIR "dados_clientes.bin"
#define DADOS_CARROS_BIN DADOS_DIR "dados_carros.bin"
#define DADOS_LOCACOES_BIN DADOS_DIR "dados_locacoes.bin"
#define DADOS_CLIENTES_TXT DADOS_DIR "dados_clientes.txt"
#define DADOS_CARROS_TXT DADOS_DIR "dados_carros.txt"
#define DADOS_LOCACOES_TXT DADOS_DIR "dados_locacoes.txt"
#define DADOS_RELATORIO_TXT DADOS_DIR "relatorio_geral.txt"

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
void salvarLista_txt(Lista *lista, const char *arquivo_txt, size_t tamanho, int tipo_lista);
void salvarListaBinarioETxt(Lista *lista, const char *arquivo_bin, const char *arquivo_txt, size_t tamanho, int tipo_lista);
void carregar_dados_sistema(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes);

//========================
// Auxiliares
//========================
void limparTela();
void pausar();
void limparBuffer();
void remover_quebra_linha(char *dadoRecebido);
int listaVazia(Lista *lista);
int tamanhoLista(Lista *lista);
void exibir_arquivo(const char *nome_arquivo);

typedef enum {
    TIPO_CLIENTE, 
    TIPO_CARRO,
    TIPO_ALUGUEL
} TipoLista;

int gerarID(TipoLista tipo);
void sincronizarID(TipoLista tipo, int maior_id_encontrado);

//========================
// Buscas
//========================
void** busca_universal(Lista *lista, TipoLista tipo_lista, char *termo_busca, int *qtd_resultados);

#endif