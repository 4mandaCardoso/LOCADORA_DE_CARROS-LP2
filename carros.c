#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "locacao.h"
#include "carros.h"
#include "util.h"

int comparar_carro_id(void *dado, void *chave) {
    Carro *c = (Carro *)dado;
    int *id_busca = (int *)chave;
    return (c->id == *id_busca);
}

void cadastrar_carro(Lista *frota) {
    Carro *novo_carro = (Carro *) malloc(sizeof(Carro));
    
    if(novo_carro == NULL){
        printf("Erro ao alocar memoria para o carro!\n");
        return;
    }

    novo_carro->id = gerarID(TIPO_CARRO);

    printf("\n--- CADASTRAR CARRO %d ---\n", novo_carro->id);

    printf("Modelo: ");
    fgets(novo_carro->modelo, TAM_MODELO, stdin);
    remover_quebra_linha(novo_carro->modelo);

    printf("Placa: ");
    fgets(novo_carro->placa, TAM_PLACA, stdin);
    remover_quebra_linha(novo_carro->placa);

    printf("Preco da Diaria: ");
    
    // um "intermediário" de texto temporário
    char temp_diaria[20]; 
    

    fgets(temp_diaria, 20, stdin);
    remover_quebra_linha(temp_diaria);
    
    //Converte o texto para FLOAT
    novo_carro->diaria = atof(temp_diaria);

    limparBuffer();

    novo_carro->disponivel = 1;

    inserirFinal(frota, novo_carro);
    
    printf("\nCarro cadastrado com sucesso! ID: %d\n", novo_carro->id);
}

void deletar_Carros(Lista *frota, Lista *historico) {
    if (listaVazia(frota)) {
        printf("\nNenhum carro cadastrado para remover.\n");
        return;
    }

    listar_frota(frota);
    int id_busca;

    printf("\nDigite o ID do carro que deseja remover: ");
    if (scanf("%d", &id_busca) != 1) {
        limparBuffer();
        printf("ID invalido. Operacao cancelada.\n");
        return;
    }
    limparBuffer();

    // Buscar o carro para exibir informações antes de remover
    Carro *carro_remover = (Carro *) buscar(frota, &id_busca, comparar_carro_id);
    if (carro_remover == NULL) {
        printf("Carro com ID %d nao encontrado.\n", id_busca);
        return;
    }

    if (carro_possui_aluguel(historico, id_busca) == 1) {
        printf("\nERRO CRITICO: Nao e possivel excluir o carro '%s'.\n", carro_remover->modelo);
        printf("Motivo: Ele possui um historico de locacao vinculado no sistema.\n");
        return;
    }

    printf("\nCarro encontrado:\n");
    printf("ID: %d | Modelo: %s | Placa: %s | Diaria: R$ %.2f\n",
           carro_remover->id, carro_remover->modelo, carro_remover->placa, carro_remover->diaria);

    char opcao;
    printf("\nTem certeza que deseja remover este carro da frota? (S/N): ");
    scanf(" %c", &opcao);
    limparBuffer();

    if (opcao == 'S' || opcao == 's') {
        if (remover(frota, &id_busca, comparar_carro_id)) {
            printf("\nCarro removido com sucesso da frota!\n");
            
        } else {
            printf("\nErro ao remover carro.\n");
            
        }
    } else {
        printf("\nOperacao cancelada. Nenhum carro foi removido.\n");
        
    }
}

void listar_frota(Lista *frota) {
    if (listaVazia(frota)) {
        printf("\nNenhum carro cadastrado na frota no momento.\n");
        return;
    }

    printf("\n--- FROTA DE VEICULOS ---\n");
    printf("=== NUMERO DE CARROS CADASTRADOS: %d ===\n\n", tamanhoLista(frota));
    
    No *aux = frota->cabeca;

    while (aux != NULL) {
        Carro *c = (Carro *)aux->dado;
        
        printf("ID: %d | Modelo: %s | Placa: %s | Diaria: R$ %.2f | %s\n",
               c->id, c->modelo, c->placa, c->diaria, 
               c->disponivel ? "Disponivel" : "Alugado");
        
        aux = aux->prox;
    }
}

void editar_carro(Lista *frota) {
    if (listaVazia(frota)) {
        printf("\nNenhum carro cadastrado para editar.\n");
        return;
    }

    listar_frota(frota);
    int id_busca;

    printf("\nDigite o ID do carro que deseja editar: ");
    if (scanf("%d", &id_busca) != 1) {
        limparBuffer();
        printf("ID invalido. Operacao cancelada.\n");
        return;
    }
    limparBuffer();

    // Busca o carro na lista
    Carro *carro_editar = (Carro *) buscar(frota, &id_busca, comparar_carro_id);
    
    if (carro_editar == NULL) {
        printf("Carro com ID %d nao encontrado.\n", id_busca);
        return;
    }

    printf("\n--- EDITANDO CARRO ID %d ---\n", carro_editar->id);
    printf("DICA: Pressione [ENTER] sem digitar nada para manter o valor atual.\n\n");

    char buffer[100]; // Buffer temporário para capturar as entradas

    // Atualização do Modelo
    printf("Modelo atual [%s]: ", carro_editar->modelo);
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) {
        strcpy(carro_editar->modelo, buffer);
    }

    // Atualização da Placa
    printf("Placa atual [%s]: ", carro_editar->placa);
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) {
        strcpy(carro_editar->placa, buffer);
    }

    // Atualização da Diária
    printf("Preco da Diaria atual [R$ %.2f]: ", carro_editar->diaria);
    fgets(buffer, sizeof(buffer), stdin);
    remover_quebra_linha(buffer);
    if (strlen(buffer) > 0) {
        carro_editar->diaria = atof(buffer);
    }

    // O status de disponibilidade (disponivel) geralmente não é editado manualmente aqui, 
    // pois ele é controlado pelo sistema de locação (alugar/devolver).

    printf("\nCarro atualizado com sucesso!\n");
}