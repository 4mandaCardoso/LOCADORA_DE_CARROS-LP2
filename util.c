#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#endif

#include "util.h"
#include "clientes.h"
#include "carros.h"
#include "locacao.h"

static int id_cliente_atual = 0;
static int id_carro_atual = 0;
static int id_aluguel_atual = 0;

Lista *criarLista()
{
    Lista *lista = (Lista *) malloc(sizeof(Lista));

    if (lista == NULL)
    {
        printf("Erro ao alocar memoria!\n");
        exit(EXIT_FAILURE);
    }

    lista->cabeca = NULL;
    lista->cauda = NULL;

    return lista;
}


int listaVazia(Lista *lista)
{
    if(lista == NULL)
        return 1;

    return (lista->cabeca == NULL && lista->cauda == NULL);
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
    novo->ant = NULL; // ant recebe NULL, pois será o primeiro nó da lista

    if(lista->cabeca == NULL)
    {
        lista->cauda = novo;
    }
    else
    {
        lista->cabeca->ant = novo;
    }

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
   novo->ant = lista->cauda;

    if(lista->cauda == NULL) // se a Lista estava vazia
    {
        lista->cabeca = novo;
    }
    else
    {
        lista->cauda->prox = novo;
    }
    lista->cauda = novo;

}

void *buscar(Lista *lista,void *chave,int (*comparar)(void *, void *))
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

    return NULL; // significa que não encontrou o dado na lista, logo o dado não existe na lista ainda
}


int remover(Lista *lista, void *chave, int (*comparar)(void *, void *))
{
    if(lista == NULL)
        return 0;

    No *atual = lista->cabeca;

    while(atual != NULL)
    {
        if(comparar(atual->dado, chave))
        {
            /* Remove o primeiro nó */
            if(atual->ant == NULL)
            {
                lista->cabeca = atual->prox;
            }
            else
            {
                atual->ant->prox = atual->prox;
            }

            if(atual->prox == NULL) // faz ponteiro da cauda apontar para o nó anterior
                lista->cauda = atual->ant;
            else 
                atual->prox->ant = atual->ant;

            free(atual->dado);
            free(atual);

            return 1;
        }

        atual = atual->prox; // faz o ponteiro atual apontar para o próximo nó
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
    lista->cauda = NULL;

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
    /* Abre o arquivo para escrita binária ("wb"). Mesmo lista vazia deve truncar o arquivo. */
    FILE *file = fopen(arquivo, "wb");
    if(file == NULL)
    {
        printf("Erro ao criar/abrir o arquivo para salvar!\n");
        return;
    }

    if(lista == NULL || lista->cabeca == NULL)
    {
        printf("Lista vazia. Arquivo binario '%s' truncado.\n", arquivo);
        fclose(file);
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

void salvarLista_txt(Lista *lista, const char *arquivo_txt, size_t tamanho, int tipo_lista)
{
    if(arquivo_txt == NULL)
    {
        printf("Arquivo de texto invalido. Nada para salvar em texto.\n");
        return;
    }

    FILE *file = fopen(arquivo_txt, "w");
    if(file == NULL)
    {
        printf("Erro ao criar/abrir o arquivo de texto '%s'.\n", arquivo_txt);
        return;
    }

    if(lista == NULL || lista->cabeca == NULL)
    {
        printf("Lista vazia. Arquivo texto '%s' truncado.\n", arquivo_txt);
        fclose(file);
        return;
    }

    No *aux = lista->cabeca;
    while(aux != NULL)
    {
        if(tipo_lista == TIPO_CLIENTE)
        {
            Cliente *cliente = (Cliente *) aux->dado;
            if(cliente != NULL)
            {
                fprintf(file, "%d;%s;%s;%s\n", cliente->id, cliente->nome, cliente->cpf, cliente->idade);
            }
        }
        else if(tipo_lista == TIPO_CARRO)
        {
            Carro *carro = (Carro *) aux->dado;
            if(carro != NULL)
            {
                fprintf(file, "%d;%s;%s;%.2f;%d\n", carro->id, carro->modelo, carro->placa, carro->diaria, carro->disponivel);
            }
        }
        else if(tipo_lista == TIPO_ALUGUEL)
        {
            Aluguel *aluguel = (Aluguel *) aux->dado;
            if(aluguel != NULL)
            {
                fprintf(file, "%d;%d;%d;%s;%s;%ld;%ld;%d;%.2f;%d\n",
                        aluguel->id,
                        aluguel->idCliente,
                        aluguel->idCarro,
                        aluguel->dataHoraRetirada,
                        aluguel->dataHoraDevolucao,
                        (long)aluguel->tempoRetirada,
                        (long)aluguel->tempoDevolucao,
                        aluguel->dias,
                        aluguel->valorTotal,
                        aluguel->status);
            }
        }
        aux = aux->prox;
    }

    fclose(file);
    printf("Dados salvos em texto com sucesso em '%s'.\n", arquivo_txt);
}

void salvarListaBinarioETxt(Lista *lista, const char *arquivo_bin, const char *arquivo_txt, size_t tamanho, int tipo_lista)
{
    salvarLista(lista, (char *) arquivo_bin, tamanho);
    salvarLista_txt(lista, arquivo_txt, tamanho, tipo_lista);
}

void carregarLista(Lista *lista, char arquivo[], size_t tamanho)
{
    if(lista == NULL)
        return;

    FILE *file = fopen(arquivo, "rb");
    if(file == NULL)
    {
        printf("Arquivo '%s' nao encontrado. Lista permanece vazia.\n", arquivo);
        return;
    }

    fseek(file, 0, SEEK_END);
    long tamanho_arquivo = ftell(file);
    if (tamanho_arquivo <= 0) {
        printf("Arquivo '%s' existe, mas esta vazio. Lista permanece vazia.\n", arquivo);
        fclose(file);
        return;
    }
    rewind(file);

    while(1)
    {
        void *novoDado = malloc(tamanho);
        if(novoDado == NULL)
        {
            printf("Erro de memoria ao carregar arquivo!\n");
            break;
        }

        if(fread(novoDado, tamanho, 1, file) != 1)
        {
            free(novoDado);
            break;
        }

        inserirFinal(lista, novoDado);
    }

    fclose(file);
    printf("Dados carregados com sucesso de '%s'.\n", arquivo);
}

static int achar_maior_id_clientes(Lista *lista) {
    int maior = 0;
    if (lista == NULL) return maior;

    No *aux = lista->cabeca;
    while (aux != NULL) {
        Cliente *cliente = (Cliente *) aux->dado;
        if (cliente != NULL && cliente->id > maior) {
            maior = cliente->id;
        }
        aux = aux->prox;
    }
    return maior;
}

static int achar_maior_id_carros(Lista *lista) {
    int maior = 0;
    if (lista == NULL) return maior;

    No *aux = lista->cabeca;
    while (aux != NULL) {
        Carro *carro = (Carro *) aux->dado;
        if (carro != NULL && carro->id > maior) {
            maior = carro->id;
        }
        aux = aux->prox;
    }
    return maior;
}

static int achar_maior_id_locacoes(Lista *lista) {
    int maior = 0;
    if (lista == NULL) return maior;

    No *aux = lista->cabeca;
    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL && aluguel->id > maior) {
            maior = aluguel->id;
        }
        aux = aux->prox;
    }
    return maior;
}

static void criar_pasta_dados() {
    struct stat st = {0};
    if (stat(DADOS_DIR, &st) == -1) {
        #if defined(_WIN32) || defined(_WIN64)
            _mkdir(DADOS_DIR);
        #else
            mkdir(DADOS_DIR, 0755);
        #endif
    }
}

void carregar_dados_sistema(Lista *lista_clientes, Lista *lista_carros, Lista *lista_locacoes) {
    if (lista_clientes == NULL || lista_carros == NULL || lista_locacoes == NULL) {
        return;
    }

    criar_pasta_dados();

    carregarLista(lista_clientes, DADOS_CLIENTES_BIN, sizeof(Cliente));
    carregarLista(lista_carros, DADOS_CARROS_BIN, sizeof(Carro));
    carregar_locacoes_bin(lista_locacoes, DADOS_LOCACOES_BIN);

    sincronizarID(TIPO_CLIENTE, achar_maior_id_clientes(lista_clientes));
    sincronizarID(TIPO_CARRO, achar_maior_id_carros(lista_carros));
    sincronizarID(TIPO_ALUGUEL, achar_maior_id_locacoes(lista_locacoes));

    reconstruir_relacoes_locacoes(lista_locacoes, lista_clientes, lista_carros);
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

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}

void remover_quebra_linha(char *dadoRecebido)
{
        dadoRecebido[strcspn(dadoRecebido, "\r\n")] = '\0'; // remove CR/LF de entradas no Windows e LF no Unix
        // caso não encontre nenhum desses caracteres, strcspn retorna o tamanho da string sem alterar o conteúdo
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