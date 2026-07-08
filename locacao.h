#ifndef LOCACAO_H
#define LOCACAO_H

#include "util.h"
#include "carros.h"
#include "clientes.h"
#include <time.h>

// Definições de tamanho para os campos de data e hora
#define TAM_DATA_HORA 20
#define TAM_STATUS 20

// Definições de status para as locações
#define STATUS_ATIVO 1
#define STATUS_ENCERRADO 0

// Estrutura que representa uma locação
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


// Funções para gerenciar locações
void realizar_locacao(Lista *historico, Lista *lista_clientes, Lista *lista_frota, Cliente *cliente_logado);
void realizar_devolucao(Lista *historico, Cliente *cliente_logado);
void listar_locacoes(Lista *historico);
void salvar_locacoes_bin(Lista *historico, const char *arquivo_bin);
void carregar_locacoes_bin(Lista *historico, const char *arquivo_bin);
void reconstruir_relacoes_locacoes(Lista *historico, Lista *lista_clientes, Lista *lista_frota);
int carro_possui_aluguel(Lista *historico, int id_carro); //para evitar que quando o carro for deletado e  adm pedir um relatorio, o programa nao quebre, pois o carro nao existe mais, mas a locacao ainda existe.


#endif // LOCACAO_H


