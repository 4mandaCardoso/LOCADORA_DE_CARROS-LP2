#include <stdio.h>
#include <stdlib.h>

#include "util.h"

Lista *criarLista()
{
    Lista *lista = (Lista *) malloc(sizeof(Lista));

    if (lista == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        exit(EXIT_FAILURE);
    }

    lista->cabeca = NULL;

    return lista;
}


int listaVazia(Lista *lista)
{
    if(lista == NULL)
        return 1;

    return (lista->cabeca == NULL);
}

void inserirInicio(Lista *lista, void *dado)
{
    if(lista == NULL || dado == NULL)
        return;

    No *novo = (No *) malloc(sizeof(No));

    if(novo == NULL)
    {
        printf("Erro de memoria!\n");
        return;
    }

    novo->dado = dado;
    novo->prox = lista->cabeca;

    lista->cabeca = novo;
}

void inserirFinal(Lista *lista, void *dado)
{
    if(lista == NULL || dado == NULL)
        return;

    No *novo = (No *) malloc(sizeof(No));

    if(novo == NULL)
    {
        printf("Erro de memoria!\n");
        return;
    }

    novo->dado = dado;
    novo->prox = NULL;

    /* Lista vazia */
    if(lista->cabeca == NULL)
    {
        lista->cabeca = novo;
        return;
    }

    No *aux = lista->cabeca;

    while(aux->prox != NULL)
    {
        aux = aux->prox;
    }

    aux->prox = novo;
}

void *buscar(Lista *lista,
             void *chave,
             int (*comparar)(void *, void *))
{
    if(lista == NULL)
        return NULL;

    No *aux = lista->cabeca;

    while(aux != NULL)
    {
        if(comparar(aux->dado, chave))
        {
            return aux->dado;
        }

        aux = aux->prox;
    }

    return NULL;
}


int remover(Lista *lista,
            void *chave,
            int (*comparar)(void *, void *))
{
    if(lista == NULL)
        return 0;

    No *atual = lista->cabeca;
    No *anterior = NULL;

    while(atual != NULL)
    {
        if(comparar(atual->dado, chave))
        {
            /* Remove o primeiro nó */
            if(anterior == NULL)
            {
                lista->cabeca = atual->prox;
            }
            else
            {
                anterior->prox = atual->prox;
            }

            free(atual->dado);
            free(atual);

            return 1;
        }

        anterior = atual;
        atual = atual->prox;
    }

    return 0;
}


void destruirLista(Lista *lista)
{
    if(lista == NULL)
        return;

    No *aux = lista->cabeca;

    while(aux != NULL)
    {
        No *temp = aux;

        aux = aux->prox;

        free(temp->dado);
        free(temp);
    }

    lista->cabeca = NULL;

    free(lista);
}


int tamanhoLista(Lista *lista)
{
    if(lista == NULL)
        return 0;

    int contador = 0;

    No *aux = lista->cabeca;

    while(aux != NULL)
    {
        contador++;

        aux = aux->prox;
    }

    return contador;
}


void salvarLista(Lista *lista, char arquivo[], size_t tamanho)
{
    if(lista == NULL || lista->cabeca == NULL)
    {
        printf("Lista vazia ou inexistente. Nada para salvar.\n");
        return;
    }

    /* Abre o arquivo para escrita binária ("wb") */
    FILE *file = fopen(arquivo, "wb");
    if(file == NULL)
    {
        printf("Erro ao criar/abrir o arquivo para salvar!\n");
        return;
    }

    No *aux = lista->cabeca;

    while(aux != NULL)
    {
        /* Escreve o bloco de memória do dado genérico no arquivo */
        fwrite(aux->dado, tamanho, 1, file);
        aux = aux->prox;
    }

    fclose(file);
    printf("Dados salvos com sucesso em '%s'.\n", arquivo);
}


void carregarLista(Lista *lista, char arquivo[], size_t tamanho)
{
    if(lista == NULL)
        return;

    /* Abre o arquivo para leitura binária ("rb") */
    FILE *file = fopen(arquivo, "rb");
    if(file == NULL)
    {
        printf("Arquivo '%s' nao encontrado ou erro de leitura.\n", arquivo);
        return;
    }

    while(1)
    {
        /* Aloca memória para o novo dado que será lido */
        void *novoDado = malloc(tamanho);
        if(novoDado == NULL)
        {
            printf("Erro de memoria ao carregar arquivo!\n");
            break;
        }

        /* Lê o bloco de memória do tamanho especificado */
        if(fread(novoDado, tamanho, 1, file) != 1)
        {
            /* Se não conseguiu ler (ex: fim do arquivo), libera a memória e para */
            free(novoDado);
            break;
        }

        /* Insere o dado lido no final da lista */
        inserirFinal(lista, novoDado);
    }

    fclose(file);
    printf("Dados carregados com sucesso de '%s'.\n", arquivo);
}


void limparTela()
{
    /* Verifica o sistema operacional para rodar o comando correto */
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}


void pausar()
{
    printf("\nPressione [ENTER] para continuar...\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}


int gerarID(TipoLista tipo)
{
    switch(tipo)
    {
        case TIPO_CLIENTE:
            return id_cliente_atual++;
            
        case TIPO_CARRO:
            return id_carro_atual++;
            
        case TIPO_ALUGUEL:
            return id_aluguel_atual++;
            
        default:
            printf("Erro: Tipo de lista invalido para gerar ID!\n");
            return -1;
    }
}


void sincronizarID(TipoLista tipo, int maior_id_encontrado)
{

    switch(tipo)
    {
        case TIPO_CLIENTE:
            id_cliente_atual = maior_id_encontrado + 1;
            break;
            
        case TIPO_CARRO:
            id_carro_atual = maior_id_encontrado + 1;
            break;
            
        case TIPO_ALUGUEL:
            id_aluguel_atual = maior_id_encontrado + 1;
            break;
    }
}