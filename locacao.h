#include "carro.h"
#include "cliente.h"

typedef struct{
    int id_locacao;
    Carro *carro_alugado; //  -> carro que será alugado
    Cliente *cliente_locador; // -> cliente que está alugando
    int dias;
    float valor_t;
} Locacao;

Locacao* criar_locacao(int qtd);
void realizar_locacao(Locacao *historico, int indice, Carro *frota, int qtd_carros, Cliente *cliente, int qtd_clientes);
void realizar_devolucao(Locacao *historico, int qtd_locacoes);
void listar_locacao(Locacao *historico, int qtd_locacoes);
void liberar_locacao(Locacao *historico);
void gerar_relatorio(Cliente *clientes, int qtd_clientes, Carro *frota, int qtd_carros, Locacao *historico, int qtd_locacoes);