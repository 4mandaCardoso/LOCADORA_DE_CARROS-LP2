#include "menu.h"
#include "util.h"

int main() {
    Lista *lista_clientes = criarLista();
    Lista *lista_carros = criarLista();
    Lista *lista_locacoes = criarLista();


    carregar_dados_sistema(lista_clientes, lista_carros, lista_locacoes);
    iniciar_sistema(lista_clientes, lista_carros, lista_locacoes);

    return 0;
}