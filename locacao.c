#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "locacao.h"

static Cliente *buscar_cliente_por_id(Lista *lista_clientes, int id) {
    if (lista_clientes == NULL) {
        return NULL;
    }

    No *aux = lista_clientes->cabeca;
    while (aux != NULL) {
        Cliente *cliente = (Cliente *) aux->dado;
        if (cliente != NULL && cliente->id == id) {
            return cliente;
        }
        aux = aux->prox;
    }
    return NULL;
}

static Carro *buscar_carro_por_id(Lista *lista_frota, int id) {
    if (lista_frota == NULL) {
        return NULL;
    }

    No *aux = lista_frota->cabeca;
    while (aux != NULL) {
        Carro *carro = (Carro *) aux->dado;
        if (carro != NULL && carro->id == id) {
            return carro;
        }
        aux = aux->prox;
    }
    return NULL;
}

static int contar_carros_disponiveis(Lista *lista_frota) {
    if (lista_frota == NULL) {
        return 0;
    }

    int contagem = 0;
    No *aux = lista_frota->cabeca;
    while (aux != NULL) {
        Carro *carro = (Carro *) aux->dado;
        if (carro != NULL && carro->disponivel == 1) {
            contagem++;
        }
        aux = aux->prox;
    }
    return contagem;
}

static Aluguel *buscar_aluguel_por_id(Lista *historico, int id) {
    if (historico == NULL) {
        return NULL;
    }

    No *aux = historico->cabeca;
    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL && aluguel->id == id) {
            return aluguel;
        }
        aux = aux->prox;
    }
    return NULL;
}

static void atualizar_status_locacoes(Lista *historico) {
    if (historico == NULL) {
        return;
    }

    time_t agora = time(NULL);
    No *aux = historico->cabeca;
    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL && aluguel->status == STATUS_ATIVO && agora >= aluguel->tempoDevolucao) {
            aluguel->status = STATUS_ENCERRADO;
            if (aluguel->carro_alugado != NULL) {
                aluguel->carro_alugado->disponivel = 1;
            }
        }
        aux = aux->prox;
    }
}

void realizar_locacao(Lista *historico, Lista *lista_clientes, Lista *lista_frota, Cliente *cliente_logado) {
    if (historico == NULL || lista_clientes == NULL || lista_frota == NULL) {
        printf("Erro: Listas invalidas para realizar locacao.\n");
        return;
    }

    int carros_disponiveis = contar_carros_disponiveis(lista_frota);
    if (carros_disponiveis == 0) {
        printf("\nErro: Nenhum carro disponivel para locacao no momento. Tente mais tarde!\n");
        return; 
    }

    Cliente *cliente = NULL;

    if (cliente_logado != NULL) {

        cliente = cliente_logado;
        printf("Cliente: %s (CPF: %s)\n", cliente->nome, cliente->cpf);
    } else {
        // Se for o Administrador (passou NULL), pedimos o CPF
        char cpf_cliente[TAM_CPF];
        printf("Digite o CPF do cliente: ");
        scanf("%14s", cpf_cliente);
        limparBuffer();

        cliente = (Cliente *) buscar(lista_clientes, cpf_cliente, compara_cliente_cpf);
        if (cliente == NULL) {
            printf("Erro: Cliente com CPF %s nao encontrado!\n", cpf_cliente);
            return; 
        }
    }

    int id_carro, dias;

    printf("Digite o ID do carro que deseja alugar: ");
    scanf("%d", &id_carro);
    limparBuffer();

    Carro *carro = buscar_carro_por_id(lista_frota, id_carro);
    if (carro == NULL) {
        printf("Erro: Carro com ID %d nao encontrado!\n", id_carro);
        pausar();
        return;
    }

    if (carro->disponivel == 0) {
        printf("Erro: O carro '%s' ja esta alugado!\n", carro->modelo);
        pausar();
        return;
    }

    printf("Digite a quantidade de dias da locacao: ");
    scanf("%d", &dias);
    limparBuffer();
    if (dias <= 0) {
        printf("Erro: A quantidade de dias deve ser maior que zero.\n");
        pausar();
        return;
    }

    Aluguel *novo = (Aluguel *) malloc(sizeof(Aluguel));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para a locacao!\n");
        pausar();
        return;
    }

    novo->id = gerarID(TIPO_ALUGUEL);
    novo->cliente_locador = cliente;
    novo->carro_alugado = carro;
    novo->idCliente = cliente->id;
    novo->idCarro = carro->id;
    novo->dias = dias;
    novo->valorTotal = dias * carro->diaria;
    novo->tempoRetirada = time(NULL);
    novo->tempoDevolucao = novo->tempoRetirada + (time_t)dias * 24 * 3600;
    novo->status = STATUS_ATIVO;

    novo->tempoRetirada = time(NULL);
    struct tm tmInfo = *localtime(&novo->tempoRetirada); 
    strftime(novo->dataHoraRetirada, TAM_DATA_HORA, "%d/%m/%Y %H:%M:%S", &tmInfo);

    
    tmInfo.tm_mday += dias;                 
    novo->tempoDevolucao = mktime(&tmInfo); 
    strftime(novo->dataHoraDevolucao, TAM_DATA_HORA, "%d/%m/%Y %H:%M:%S", &tmInfo);

    carro->disponivel = 0;
    inserirFinal(historico, novo);

    printf("\n>>> Locacao realizada com sucesso! <<<\n");
    printf("ID da locacao: %d\n", novo->id);
    printf("Cliente: %s\n", cliente->nome);
    printf("Carro: %s (%s)\n", carro->modelo, carro->placa);
    printf("Dias: %d\n", dias);
    printf("Valor Total: R$ %.2f\n", novo->valorTotal);
    printf("Retirada: %s\n", novo->dataHoraRetirada);
    printf("Devolucao prevista: %s\n", novo->dataHoraDevolucao);
    printf("\nLocacao confirmada e salva no sistema!\n");

}

void realizar_devolucao(Lista *historico) {
    if (historico == NULL || listaVazia(historico)) {
        printf("Nenhuma locacao registrada no sistema.\n");
        return;
    }

    atualizar_status_locacoes(historico);

    int id_loc;
    printf("Digite o ID da locacao que deseja encerrar (Devolucao): ");
    scanf("%d", &id_loc);

    Aluguel *aluguel = buscar_aluguel_por_id(historico, id_loc);
    if (aluguel == NULL) {
        printf("Erro: Locacao com ID %d nao encontrada!\n", id_loc);
        pausar();
        return;
    }

    if (aluguel->status == STATUS_ENCERRADO) {
        printf("Aviso: Esta locacao ja esta encerrada. Nao e possivel fazer a devolucao novamente.\n");
        pausar();
        return;
    }

    aluguel->status = STATUS_ENCERRADO;
    if (aluguel->carro_alugado != NULL) {
        aluguel->carro_alugado->disponivel = 1;
    }
    printf(">>> Devolucao do carro '%s' realizada com sucesso! <<<\n", aluguel->carro_alugado ? aluguel->carro_alugado->modelo : "(carro desconhecido)");
    printf("Devolucao confirmada e salva no sistema!\n");
}

void listar_locacoes(Lista *historico) {
    if (historico == NULL || listaVazia(historico)) {
        printf("\nNenhuma locacao registrada.\n");
        return;
    }

    atualizar_status_locacoes(historico);

    printf("\n--- HISTORICO DE LOCACOES INTERNAS (LOCAR) ---\n");
    No *aux = historico->cabeca;

    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL) {
            printf("Locacao ID: %d | Cliente: %s | Carro: %s (%s) | Retirada: %s | Devolucao: %s | Dias: %d | Total: R$ %.2f | Status: %s\n",
                   aluguel->id,
                   aluguel->cliente_locador ? aluguel->cliente_locador->nome : "(cliente desconhecido)",
                   aluguel->carro_alugado ? aluguel->carro_alugado->modelo : "(carro desconhecido)",
                   aluguel->carro_alugado ? aluguel->carro_alugado->placa : "---",
                   aluguel->dataHoraRetirada,
                   aluguel->dataHoraDevolucao,
                   aluguel->dias,
                   aluguel->valorTotal,
                   aluguel->status == STATUS_ATIVO ? "Ativo" : "Encerrado");
        }
        aux = aux->prox;
    }
}

void salvar_locacoes_bin(Lista *historico, const char *arquivo_bin) {
    if (historico == NULL || arquivo_bin == NULL) {
        return;
    }

    FILE *arquivo = fopen(arquivo_bin, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo binario para salvar locacoes.\n");
        return;
    }

    No *aux = historico->cabeca;
    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL) {
            Aluguel locar;
            locar.id = aluguel->id;
            locar.idCliente = aluguel->idCliente;
            locar.idCarro = aluguel->idCarro;
            strncpy(locar.dataHoraRetirada, aluguel->dataHoraRetirada, TAM_DATA_HORA);
            locar.dataHoraRetirada[TAM_DATA_HORA - 1] = '\0';
            strncpy(locar.dataHoraDevolucao, aluguel->dataHoraDevolucao, TAM_DATA_HORA);
            locar.dataHoraDevolucao[TAM_DATA_HORA - 1] = '\0';
            locar.tempoRetirada = aluguel->tempoRetirada;
            locar.tempoDevolucao = aluguel->tempoDevolucao;
            locar.dias = aluguel->dias;
            locar.valorTotal = aluguel->valorTotal;
            locar.status = aluguel->status;
            fwrite(&locar, sizeof(Aluguel), 1, arquivo);
        }
        aux = aux->prox;
    }

    fclose(arquivo);
    printf("Dados de locacoes salvos em '%s'.\n", arquivo_bin);
}

void carregar_locacoes_bin(Lista *historico, const char *arquivo_bin) {
    if (historico == NULL || arquivo_bin == NULL) {
        return;
    }

    FILE *arquivo = fopen(arquivo_bin, "rb");
    if (arquivo == NULL) {
        printf("Arquivo de locacoes binario '%s' nao encontrado.\n", arquivo_bin);
        return;
    }

    while (1) {
        Aluguel locar;
        if (fread(&locar, sizeof(Aluguel), 1, arquivo) != 1) {
            break;
        }

        Aluguel *aluguel = (Aluguel *) malloc(sizeof(Aluguel));
        if (aluguel == NULL) {
            printf("Erro de memoria ao carregar locacoes.\n");
            break;
        }

        aluguel->id = locar.id;
        aluguel->cliente_locador = NULL;
        aluguel->carro_alugado = NULL;
        aluguel->idCliente = locar.idCliente;
        aluguel->idCarro = locar.idCarro;
        strncpy(aluguel->dataHoraRetirada, locar.dataHoraRetirada, TAM_DATA_HORA);
        aluguel->dataHoraRetirada[TAM_DATA_HORA - 1] = '\0';
        strncpy(aluguel->dataHoraDevolucao, locar.dataHoraDevolucao, TAM_DATA_HORA);
        aluguel->dataHoraDevolucao[TAM_DATA_HORA - 1] = '\0';
        aluguel->tempoRetirada = locar.tempoRetirada;
        aluguel->tempoDevolucao = locar.tempoDevolucao;
        aluguel->dias = locar.dias;
        aluguel->valorTotal = locar.valorTotal;
        aluguel->status = locar.status;

        inserirFinal(historico, aluguel);
    }

    fclose(arquivo);
    printf("Locacoes carregadas de '%s'.\n", arquivo_bin);
}

void reconstruir_relacoes_locacoes(Lista *historico, Lista *lista_clientes, Lista *lista_frota) {
    if (historico == NULL) {
        return;
    }

    No *aux = historico->cabeca;
    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL) {
            aluguel->cliente_locador = buscar_cliente_por_id(lista_clientes, aluguel->idCliente);
            aluguel->carro_alugado = buscar_carro_por_id(lista_frota, aluguel->idCarro);
            if (aluguel->carro_alugado != NULL && aluguel->status == STATUS_ATIVO) {
                aluguel->carro_alugado->disponivel = 0;
            }
        }
        aux = aux->prox;
    }
}

void liberar_locacoes(Lista *historico) {
    if (historico != NULL) {
        destruirLista(historico);
    }
}
