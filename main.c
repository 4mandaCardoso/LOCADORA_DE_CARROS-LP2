#include "menu.h"
#include "clientes.h"
#include "carros.h"
#include "aluguel.h"
#include "util.h"
#include <stdio.h>


int main() {
    // Cria a lista principal
    Lista *lista_clientes = criarLista();

    printf("=== TESTE DO SISTEMA DA LOCADORA ===\n");

    // Testa a função  de criar
    cadastrar_cliente(lista_clientes);
    
    // Lista para ver se guardou certpo
    listar_clientes(lista_clientes);

    
    buscar_cliente(lista_clientes);

    atualizar_cliente(lista_clientes);

    remover_cliente(lista_clientes);

    // Limpa a memória no fim
    destruirLista(lista_clientes);

    return 0;
}