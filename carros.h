#ifndef CARROS_H
#define CARROS_H

#include "util.h"

#define TAM_MODELO 50
#define TAM_PLACA 10
#define TAM_DIARIA 20

typedef struct {
    int id;
    char modelo[TAM_MODELO];
    char placa[TAM_PLACA];
    float diaria;
    int disponivel;
} Carro;

int comparar_carro_id(void *dado, void *chave);
void cadastrar_carro(Lista *frota);
void deletar_Carros(Lista *frota, Lista *historico);
void listar_frota(Lista *frota);
void editar_carro(Lista *frota);


#endif // CARROS_H