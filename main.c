#include "menu.h"
#include "util.h"
#include "carros.h"
#include "clientes.h"
#include "locacao.h"
#include "relatorio.h"

// main de inicialização do sistema, carregamento de dados e execução do menu principal e funções de relatório e ajuda
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--ajuda") == 0) {
            exibir_arquivo("ajuda.txt");
            pausar();
            return 0; 
        }
        else if (strcmp(argv[i], "--relatorio") == 0) {
            printf("\nCarregando informacoes...\n");

        }
    }

    //Inicialização do sistema
    Lista *lista_clientes = criarLista();
    Lista *lista_carros = criarLista();
    Lista *lista_locacoes = criarLista();

    carregar_dados_sistema(lista_clientes, lista_carros, lista_locacoes);

    // Se o argumento --relatorio foi usado, você pode gerar agora:
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--relatorio") == 0) {
            gerar_relatorio(lista_locacoes, lista_clientes, lista_carros);
            mostrar_relatorio(lista_locacoes, lista_clientes, lista_carros);
            pausar();
            return 0;
        }
    }
    iniciar_sistema(lista_clientes, lista_carros, lista_locacoes);

    return 0;
}
