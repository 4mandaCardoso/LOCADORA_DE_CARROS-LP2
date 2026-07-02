#include "menu.h"
#include "util.h"
#include "locacao.h"
#include "relatorio.h"

int main() {
    Lista *lista_clientes = criarLista();
    Lista *lista_carros = criarLista();
    Lista *lista_locacoes = criarLista();

    carregar_dados_sistema(lista_clientes, lista_carros, lista_locacoes);
    iniciar_sistema(lista_clientes, lista_carros, lista_locacoes);

    salvarListaBinarioETxt(lista_clientes, DADOS_CLIENTES_BIN, DADOS_CLIENTES_TXT, sizeof(Cliente), TIPO_CLIENTE);
    salvarListaBinarioETxt(lista_carros, DADOS_CARROS_BIN, DADOS_CARROS_TXT, sizeof(Carro), TIPO_CARRO);
    salvar_locacoes_bin(lista_locacoes, DADOS_LOCACOES_BIN);
    salvarLista_txt(lista_locacoes, DADOS_LOCACOES_TXT, sizeof(Aluguel), TIPO_ALUGUEL);
    gerar_relatorio(lista_locacoes, lista_clientes, lista_carros);

    destruirLista(lista_clientes);
    destruirLista(lista_carros);
    destruirLista(lista_locacoes);

    return 0;
}
