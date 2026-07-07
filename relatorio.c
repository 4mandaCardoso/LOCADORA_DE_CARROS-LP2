#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "relatorio.h"
#include "util.h"

static int extrair_mes_ano(const char *dataHora, int *mes, int *ano) {
    int dia, hora, minuto, segundo;
    if (dataHora == NULL || mes == NULL || ano == NULL) {
        return 0;
    }

    int campos = sscanf(dataHora, "%d/%d/%d %d:%d:%d", &dia, mes, ano, &hora, &minuto, &segundo);
    if (campos < 5) {
        return 0;
    }

    if (*mes < 1 || *mes > 12 || *ano < 1900) {
        return 0;
    }

    return 1;
}

void mostrar_relatorio(Lista *historico, Lista *lista_clientes, Lista *lista_carros) {
    if (historico == NULL || lista_clientes == NULL || lista_carros == NULL) {
        printf("Erro: Listas invalidas para mostrar relatorio.\n");
        return;
    }

    printf("\n=========================================================\n");
    printf("                 LOCAR - RELATORIO GERAL              \n");
    printf("=========================================================\n\n");

    printf("--- CLIENTES CADASTRADOS ---\n");
    No *aux = lista_clientes->cabeca;
    if (aux == NULL) {
        printf("Nenhum cliente cadastrado.\n");
    } else {
        while (aux != NULL) {
            Cliente *cliente = (Cliente *) aux->dado;
            if (cliente != NULL) {
            printf("ID: %d | Nome: %s | Idade: %s | CPF: %s\n", cliente->id, cliente->nome, cliente->idade, cliente->cpf);
            }
            aux = aux->prox;
        }
    }
    printf("\n");

    printf("--- FROTA DE CARROS ---\n");
    aux = lista_carros->cabeca;
    if (aux == NULL) {
        printf("Nenhum carro cadastrado na frota.\n");
    } else {
        while (aux != NULL) {
            Carro *carro = (Carro *) aux->dado;
            if (carro != NULL) {
                printf("ID: %d | Modelo: %s | Placa: %s | Diaria: R$ %.2f | Status: %s\n",
                       carro->id, carro->modelo, carro->placa, carro->diaria,
                       carro->disponivel ? "Disponivel" : "Alugado");
            }
            aux = aux->prox;
        }
    }
    printf("\n");

    printf("--- RELACAO DE LOCACOES ---\n");
    aux = historico->cabeca;
    if (aux == NULL) {
        printf("Nenhuma locacao registrada no historico.\n");
    } else {
        while (aux != NULL) {
            Aluguel *aluguel = (Aluguel *) aux->dado;
            if (aluguel != NULL) {
                printf("Locacao ID: %d\n", aluguel->id);
                printf("  Cliente: %s (CPF: %s)\n", aluguel->cliente_locador ? aluguel->cliente_locador->nome : "(cliente desconhecido)", aluguel->cliente_locador ? aluguel->cliente_locador->cpf : "---");
                printf("  Carro:   %s (Placa: %s)\n", aluguel->carro_alugado ? aluguel->carro_alugado->modelo : "(carro desconhecido)", aluguel->carro_alugado ? aluguel->carro_alugado->placa : "---");
                printf("  Retirada: %s\n", aluguel->dataHoraRetirada);
                printf("  Devolucao: %s\n", aluguel->dataHoraDevolucao);
                printf("  Periodo: %d dias\n", aluguel->dias);
                printf("  Total:   R$ %.2f\n", aluguel->valorTotal);
                printf("  Status:  %s\n", aluguel->status == STATUS_ATIVO ? "Ativo" : "Encerrado");
                printf("---------------------------------------------------------\n");
            }
            aux = aux->prox;
        }
    }

    int mes_atual = 0, ano_atual = 0;
    time_t agora = time(NULL);
    struct tm *tm_hoje = localtime(&agora);
    if (tm_hoje) {
        mes_atual = tm_hoje->tm_mon + 1;
        ano_atual = tm_hoje->tm_year + 1900;
    }

    double receita_mes = calcular_receita_mes(historico, mes_atual, ano_atual);
    double receita_total = calcular_receita_total(historico);
    printf("\n--- RECEITA DO MES %02d/%d ---\n", mes_atual, ano_atual);
    printf("Receita do mes: R$ %.2f\n", receita_mes);
    printf("Receita total acumulada: R$ %.2f\n", receita_total);
}

void gerar_relatorio(Lista *historico, Lista *lista_clientes, Lista *lista_carros) {
    if (historico == NULL || lista_clientes == NULL || lista_carros == NULL) {
        printf("Erro: Listas invalidas para gerar relatorio.\n");
        return;
    }

    FILE *arquivo = fopen(DADOS_RELATORIO_TXT, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de relatorio TXT!\n");
        return;
    }

    fprintf(arquivo, "=========================================================\n");
    fprintf(arquivo, "                 LOCAR - RELATORIO GERAL              \n");
    fprintf(arquivo, "=========================================================\n\n");

    fprintf(arquivo, "--- CLIENTES CADASTRADOS ---\n");
    No *aux = lista_clientes->cabeca;
    if (aux == NULL) {
        fprintf(arquivo, "Nenhum cliente cadastrado.\n");
    } else {
        while (aux != NULL) {
            Cliente *cliente = (Cliente *) aux->dado;
            if (cliente != NULL) {
                fprintf(arquivo, "ID: %d | Nome: %s | idade: %s| CPF: %s\n", cliente->id, cliente->nome, cliente->idade, cliente->cpf);
            }
            aux = aux->prox;
        }
    }
    fprintf(arquivo, "\n");

    fprintf(arquivo, "--- FROTA DE CARROS ---\n");
    aux = lista_carros->cabeca;
    if (aux == NULL) {
        fprintf(arquivo, "Nenhum carro cadastrado na frota.\n");
    } else {
        while (aux != NULL) {
            Carro *carro = (Carro *) aux->dado;
            if (carro != NULL) {
                fprintf(arquivo, "ID: %d | Modelo: %s | Placa: %s | Diaria: R$ %.2f | Status: %s\n",
                        carro->id, carro->modelo, carro->placa, carro->diaria,
                        carro->disponivel ? "Disponivel" : "Alugado");
            }
            aux = aux->prox;
        }
    }
    fprintf(arquivo, "\n");

    fprintf(arquivo, "--- RELACAO DE LOCACOES ---\n");
    aux = historico->cabeca;
    if (aux == NULL) {
        fprintf(arquivo, "Nenhuma locacao registrada no historico.\n");
    } else {
        while (aux != NULL) {
            Aluguel *aluguel = (Aluguel *) aux->dado;
            if (aluguel != NULL) {
                fprintf(arquivo, "Locacao ID: %d\n", aluguel->id);
                fprintf(arquivo, "  Cliente: %s (CPF: %s)\n", aluguel->cliente_locador ? aluguel->cliente_locador->nome : "(cliente desconhecido)", aluguel->cliente_locador ? aluguel->cliente_locador->cpf : "---");
                fprintf(arquivo, "  Carro:   %s (Placa: %s)\n", aluguel->carro_alugado ? aluguel->carro_alugado->modelo : "(carro desconhecido)", aluguel->carro_alugado ? aluguel->carro_alugado->placa : "---");
                fprintf(arquivo, "  Retirada: %s\n", aluguel->dataHoraRetirada);
                fprintf(arquivo, "  Devolucao: %s\n", aluguel->dataHoraDevolucao);
                fprintf(arquivo, "  Periodo: %d dias\n", aluguel->dias);
                fprintf(arquivo, "  Total:   R$ %.2f\n", aluguel->valorTotal);
                fprintf(arquivo, "  Status:  %s\n", aluguel->status == STATUS_ATIVO ? "Ativo" : "Encerrado");
                fprintf(arquivo, "---------------------------------------------------------\n");
            }
            aux = aux->prox;
        }
    }

    int mes_atual = 0, ano_atual = 0;
    time_t agora = time(NULL);
    struct tm *tm_hoje = localtime(&agora);
    if (tm_hoje) {
        mes_atual = tm_hoje->tm_mon + 1;
        ano_atual = tm_hoje->tm_year + 1900;
    }

    double receita_mes = calcular_receita_mes(historico, mes_atual, ano_atual);
    double receita_total = calcular_receita_total(historico);
    fprintf(arquivo, "\n--- RECEITA DO MES %02d/%d ---\n", mes_atual, ano_atual);
    fprintf(arquivo, "Receita do mes: R$ %.2f\n", receita_mes);
    fprintf(arquivo, "Receita total acumulada: R$ %.2f\n", receita_total);

    fclose(arquivo);
    printf("\n>>> Arquivo 'relatorio_geral.txt' gerado com sucesso! <<<\n");
}

double calcular_receita_mes(Lista *historico, int mes, int ano) {
    if (historico == NULL) {
        return 0.0;
    }

    double total = 0.0;
    No *aux = historico->cabeca;

    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        int loc_mes = 0, loc_ano = 0;
        if (aluguel != NULL && extrair_mes_ano(aluguel->dataHoraRetirada, &loc_mes, &loc_ano)) {
            if (loc_mes == mes && loc_ano == ano) {
                total += aluguel->valorTotal;
            }
        }
        aux = aux->prox;
    }

    return total;
}

double calcular_receita_total(Lista *historico) {
    if (historico == NULL) {
        return 0.0;
    }

    double total = 0.0;
    No *aux = historico->cabeca;

    while (aux != NULL) {
        Aluguel *aluguel = (Aluguel *) aux->dado;
        if (aluguel != NULL) {
            total += aluguel->valorTotal;
        }
        aux = aux->prox;
    }

    return total;
}
