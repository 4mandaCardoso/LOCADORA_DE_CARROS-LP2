#ifndef CARROS_H
#define CARROS_H

/* BIBLIOTECAS DE CARROS - TUDO QUE ENVOLVER CARROS*/
#define TAM_MARCA 30
#define TAM_MODELO 40
#define TAM_PLACA 10

typedef struct Carro
{
    int id;

    char marca[TAM_MARCA];
    char modelo[TAM_MODELO];

    int ano;

    char placa[TAM_PLACA];

    float diaria;

    int disponivel;

    struct Carro *prox;

} Carro;




#endif