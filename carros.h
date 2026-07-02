#include "util.h"

typedef struct {
    int id;
    char modelo[50];
    char placa[10];
    float diaria;
    int disponivel;
} Carro;

int comparar_carro_id(void *dado, void *chave);
void cadastrar_carro(Lista *frota);
void listar_frota(Lista *frota);
void liberar_frota(Lista *frota);