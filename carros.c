#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    scanf(" %[^\n]", novo_carro->modelo);

    printf("Placa: ");
    scanf(" %[^\n]", novo_carro->placa);

    printf("Preco da Diaria: ");
    scanf("%f", &novo_carro->diaria);

    novo_carro->disponivel = 1;

    inserirFinal(frota, novo_carro);
    
    printf("\nCarro cadastrado com sucesso! ID: %d\n", novo_carro->id);
}

void deletar_Carros(Lista *frota) {
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

    if (remover(frota, &id_busca, comparar_carro_id)) {
        printf("Carro removido com sucesso!\n");
    } else {
        printf("Carro com ID %d nao encontrado.\n", id_busca);
    }
}

void listar_frota(Lista *frota) {
    if (listaVazia(frota)) {
        printf("\nNenhum carro cadastrado na frota no momento.\n");
        return;
    }

    printf("\n--- FROTA DE VEICULOS ---\n");
    
    No *aux = frota->cabeca;

    while (aux != NULL) {
        Carro *c = (Carro *)aux->dado;
        
        printf("ID: %d | Modelo: %s | Placa: %s | Diaria: R$ %.2f | %s\n",
               c->id, c->modelo, c->placa, c->diaria, 
               c->disponivel ? "Disponivel" : "Alugado");
        
        aux = aux->prox;
    }
}

void liberar_frota(Lista *frota) {
    if (frota != NULL) {
        destruirLista(frota);
        printf("\nMemoria da frota liberada com sucesso!\n");
    }
}