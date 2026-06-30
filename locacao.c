#include <stdio.h>
#include <stdlib.h>
#include "locacao.h"

Locacao* criar_locacao(int qtd) {
    Locacao *historico = (Locacao*) malloc(qtd * sizeof(Locacao));
    if (historico == NULL) {
        printf("Erro de alocacao de memoria para locacoes!\n");
        return NULL;
    }
    return historico;
}

void realizar_locacao(Locacao *historico, int indice, Carro *frota, int qtd_carros, Cliente *cliente, int qtd_clientes) {
    historico[indice].id_locacao = indice + 1;
    
    int id_cliente, id_carro;
    int cliente_encontrado = 0;
    int carro_encontrado = 0;

    printf("Digite o ID do cliente: ");
    scanf("%d", &id_cliente);
    
    for (int i = 0; i < qtd_clientes; i++) {
        if (cliente[i].id == id_cliente) {
            historico[indice].cliente_locador = &cliente[i];
            cliente_encontrado = 1;
            break;
        }
    }

    if (!cliente_encontrado) {
        printf("Erro: Cliente com ID %d nao encontrado!\n", id_cliente);
        return; 
    }

    printf("Digite o ID do carro que deseja alugar: ");
    scanf("%d", &id_carro);
    
    for (int i = 0; i < qtd_carros; i++) {
        if (frota[i].id == id_carro) {
            carro_encontrado = 1;
            
            if (frota[i].disponivel == 0) {
                printf("Erro: O carro '%s' ja esta alugado!\n", frota[i].modelo);
                return;
            }
            frota[i].disponivel = 0;
            historico[indice].carro_alugado = &frota[i];
            break;
        }
    }

    if (!carro_encontrado) {
        printf("Erro: Carro com ID %d nao encontrado!\n", id_carro);
        return;
    }

    printf("Digite a quantidade de dias da locacao: ");
    scanf("%d", &historico[indice].dias);

    historico[indice].valor_t = historico[indice].dias * historico[indice].carro_alugado->diaria;

    printf("\n>>> Locacao realizada com sucesso! <<<\n");
    printf("Valor Total: R$ %.2f\n", historico[indice].valor_t);
}

void realizar_devolucao(Locacao *historico, int qtd_locacoes) {
    int id_loc;
    int locacao_encontrada = 0;

    printf("Digite o ID da locacao que deseja encerrar (Devolucao): ");
    scanf("%d", &id_loc);

    for (int i = 0; i < qtd_locacoes; i++) {
        if (historico[i].id_locacao == id_loc) {
            locacao_encontrada = 1;
            
            if (historico[i].carro_alugado->disponivel == 1) {
                printf("Aviso: Essa devolucao ja foi realizada anteriormente!\n");
                return;
            }

            historico[i].carro_alugado->disponivel = 1;
            printf(">>> Devolucao do carro '%s' realizada com sucesso! <<<\n", historico[i].carro_alugado->modelo);
            break;
        }
    }

    if (!locacao_encontrada) {
        printf("Erro: Locação com ID %d nao encontrada!\n", id_loc);
    }
}

void listar_locacao(Locacao *historico, int qtd_locacoes) {
    printf("\n--- HISTORICO DE LOCACOES INTERNAS (AUTOBOTS) ---\n");
    if (qtd_locacoes == 0) {
        printf("Nenhuma locacao registrada.\n");
        return;
    }

    for (int i = 0; i < qtd_locacoes; i++) {
        if (historico[i].carro_alugado != NULL && historico[i].cliente_locador != NULL) {
            printf("Locacao ID: %d | Cliente: %s | Carro: %s (%s) | Dias: %d | Total: R$ %.2f\n",
                   historico[i].id_locacao,
                   historico[i].cliente_locador->nome,
                   historico[i].carro_alugado->modelo,
                   historico[i].carro_alugado->placa,
                   historico[i].dias,
                   historico[i].valor_t);
        }
    }
}

void liberar_locacao(Locacao *historico) {
    if (historico != NULL) {
        free(historico);
        printf("Memoria do historico de locacoes liberada!\n");
    }
}

void gerar_relatorio(Cliente *clientes, int qtd_clientes, Carro *frota, int qtd_carros, Locacao *historico, int qtd_locacoes) {
    FILE *arquivo = fopen("relatorio_geral.txt", "w");
    
    if (arquivo == NULL) {
        printf("\nErro ao criar o arquivo de relatorio TXT!\n");
        return;
    }

    fprintf(arquivo, "=========================================================\n");
    fprintf(arquivo, "                 AUTOBOTS - RELATORIO GERAL              \n");
    fprintf(arquivo, "=========================================================\n\n");

    fprintf(arquivo, "--- CLIENTES CADASTRADOS ---\n");
    if (qtd_clientes == 0) {
        fprintf(arquivo, "Nenhum cliente cadastrado.\n");
    } else {
        for (int i = 0; i < qtd_clientes; i++) {
            fprintf(arquivo, "ID: %d | Nome: %s | CPF: %s | Tel: %s\n", 
                    clientes[i].id, clientes[i].nome, clientes[i].cpf, clientes[i].tel);
        }
    }
    fprintf(arquivo, "\n");

    fprintf(arquivo, "--- FROTA DE CARROS ---\n");
    if (qtd_carros == 0) {
        fprintf(arquivo, "Nenhum carro cadastrado na frota.\n");
    } else {
        for (int i = 0; i < qtd_carros; i++) {
            fprintf(arquivo, "ID: %d | Modelo: %s | Placa: %s | Diaria: R$ %.2f | Status: %s\n", 
                    frota[i].id, frota[i].modelo, frota[i].placa, frota[i].diaria, 
                    frota[i].disponivel ? "Disponivel" : "Alugado");
        }
    }
    fprintf(arquivo, "\n");

    fprintf(arquivo, "--- RELACAO DE LOCACOES ---\n");
    if (qtd_locacoes == 0) {
        fprintf(arquivo, "Nenhuma locacao registrada no historico.\n");
    } else {
        for (int i = 0; i < qtd_locacoes; i++) {
            if (historico[i].cliente_locador != NULL && historico[i].carro_alugado != NULL) {
                fprintf(arquivo, "Locacao ID: %d\n", historico[i].id_locacao);
                fprintf(arquivo, "  Cliente: %s (CPF: %s)\n", historico[i].cliente_locador->nome, historico[i].cliente_locador->cpf);
                fprintf(arquivo, "  Carro:   %s (Placa: %s)\n", historico[i].carro_alugado->modelo, historico[i].carro_alugado->placa);
                fprintf(arquivo, "  Periodo: %d dias\n", historico[i].dias);
                fprintf(arquivo, "  Total:   R$ %.2f\n", historico[i].valor_t);
                fprintf(arquivo, "---------------------------------------------------------\n");
            }
        }
    }

    fclose(arquivo);
    printf("\n>>> Arquivo 'relatorio_geral.txt' gerado com sucesso! <<<\n");
}