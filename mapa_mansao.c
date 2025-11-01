/*
Detective Quest - Nível Novato
Mapa da mansão usando Árvore Binária
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

// Estrutura da Árvore Binária (cada nó é uma sala)
typedef struct Sala {
    char nome[50];           // Nome do cômodo
    struct Sala* esquerda;   // Caminho para a esquerda
    struct Sala* direita;    // Caminho para a direita
} Sala;

// [criarSala] Cria dinamicamente uma sala com nome
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para a sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita  = NULL;
    return nova;
}

// [explorarSalas] Navegação interativa pela árvore
// A exploração termina automaticamente ao alcançar um nó folha (sem filhos)
void explorarSalas(Sala* atual) {
    if (atual == NULL) {
        printf("Mapa vazio. Nada para explorar.\n");
        return;
    }

    printf("\n=== Detective Quest: Exploração da Mansão ===\n");
    while (1) {
        // Exibe a sala atual
        printf("\nVocê está em: %s\n", atual->nome);

        // Se for nó-folha, encerra automaticamente
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou a um cômodo sem saídas (fim da exploração).\n");
            break;
        }

        // Mostra opções disponíveis
        printf("\nCaminhos disponíveis:\n");
        printf("  [e] Esquerda: %s\n", (atual->esquerda ? atual->esquerda->nome : "-- sem caminho --"));
        printf("  [d] Direita : %s\n", (atual->direita  ? atual->direita->nome  : "-- sem caminho --"));
        printf("  [s] Sair\n");

        printf("Escolha (e/d/s): ");
        char op;
        if (scanf(" %c", &op) != 1) {
            // Entrada inválida — limpa e continua
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (op == 's' || op == 'S') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else if (op == 'e' || op == 'E') {
            if (atual->esquerda) {
                atual = atual->esquerda; // Move para a esquerda
            } else {
                printf("Não há caminho à esquerda a partir de %s.\n", atual->nome);
            }
        } else if (op == 'd' || op == 'D') {
            if (atual->direita) {
                atual = atual->direita;  // Move para a direita
            } else {
                printf("Não há caminho à direita a partir de %s.\n", atual->nome);
            }
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

// [liberarArvore] Libera memória de toda a árvore
// (pós-ordem: filhos antes do nó atual)
void liberarArvore(Sala* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// --------------------------------------------------
// Função principal [main] Monta manualmente o mapa (árvore) e inicia a exploração a partir do Hall de Entrada.

int main(void) {
    // Montagem manual do mapa da mansão (árvore binária fixa)
    Sala* hall         = criarSala("Hall de Entrada");
    Sala* salaEstar    = criarSala("Sala de Estar");
    Sala* biblioteca   = criarSala("Biblioteca");
    Sala* cozinha      = criarSala("Cozinha");
    Sala* jardim       = criarSala("Jardim");
    Sala* escritorio   = criarSala("Escritório");
    Sala* quarto       = criarSala("Quarto");

    // Ligações (estrutura da árvore):
    hall->esquerda      = salaEstar;
    hall->direita       = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita  = jardim;
    biblioteca->esquerda= escritorio;
    biblioteca->direita = quarto;

    // Exploração a partir do Hall
    explorarSalas(hall);

    // Liberação de memória
    liberarArvore(hall);

    return 0;
}
