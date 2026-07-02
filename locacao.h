#ifndef LOCACAO_H
#define LOCACAO_H

#include "util.h"
#include "carros.h"
#include "clientes.h"
#include <time.h>

#define TAM_DATA_HORA 20
#define TAM_STATUS 20

#define STATUS_ATIVO 1
#define STATUS_ENCERRADO 0

typedef struct Aluguel {
    int id;
    Cliente *cliente_locador;
    Carro *carro_alugado;
    char dataHoraRetirada[TAM_DATA_HORA];
    char dataHoraDevolucao[TAM_DATA_HORA];
    time_t tempoRetirada;
    time_t tempoDevolucao;
    int dias;
    float valorTotal;
    int status;
    int idCliente;
    int idCarro;
} Aluguel;

void realizar_locacao(Lista *historico, Lista *lista_clientes, Lista *lista_frota);
void realizar_devolucao(Lista *historico);
void listar_locacoes(Lista *historico);
void salvar_locacoes_bin(Lista *historico, const char *arquivo_bin);
void carregar_locacoes_bin(Lista *historico, const char *arquivo_bin);
void reconstruir_relacoes_locacoes(Lista *historico, Lista *lista_clientes, Lista *lista_frota);
void liberar_locacoes(Lista *historico);

#endif // LOCACAO_H