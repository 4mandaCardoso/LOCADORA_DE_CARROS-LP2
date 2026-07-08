#ifndef CARROS_H
#define CARROS_H

#include "util.h"

// Definições de tamanho para os campos do carro
#define TAM_MODELO 50
#define TAM_PLACA 10
#define TAM_DIARIA 20

// Estrutura que representa um carro
typedef struct {
    int id;
    char modelo[TAM_MODELO];
    char placa[TAM_PLACA];
    float diaria;
    int disponivel;// 1 para disponível, 0 para alugado
} Carro;

// CRUD dos carros
int comparar_carro_id(void *dado, void *chave);
void cadastrar_carro(Lista *frota);
void deletar_Carros(Lista *frota, Lista *historico);
void listar_frota(Lista *frota);
void editar_carro(Lista *frota);


#endif // CARROS_H