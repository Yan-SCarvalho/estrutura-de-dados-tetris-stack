#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

// Estrutura que representa uma peca de Tetris
struct Peca {
    int id;
    char tipo; // 'I', 'O', 'T', 'L', 'J', 'S', 'Z'
};

// --- ESTRUTURA DA FILA CIRCULAR ---
struct Peca fila[TAMANHO_FILA];
int frente = 0;
int tras = 0;
int totalFila = 0;
int proximoId = 1;

// --- ESTRUTURA DA PILHA LINEAR (NOVIDADE AVENTUREIRO) ---
struct Peca pilha[TAMANHO_PILHA];
int topo = -1; // -1 significa pilha vazia

// Funcao para gerar uma peca aleatoria automaticamente
struct Peca gerarPeca() {
    struct Peca novaPeca;
    char tiposPossiveis[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    novaPeca.id = proximoId++;
    novaPeca.tipo = tiposPossiveis[rand() % 7]; 
    return novaPeca;
}

// Funcoes de controle da Fila Circular
void enfileirar(struct Peca peca) {
    if (totalFila >= TAMANHO_FILA) return;
    fila[tras] = peca;
    tras = (tras + 1) % TAMANHO_FILA;
    totalFila++;
}

struct Peca desenfileirar() {
    struct Peca pecaRemovida = fila[frente];
    frente = (frente + 1) % TAMANHO_FILA;
    totalFila--;
    
    // Mantem a fila sempre cheia gerando uma nova peca no final automaticamente
    enfileirar(gerarPeca());
    return pecaRemovida;
}

// Funcoes de controle da Pilha (Push e Pop)
void empilhar(struct Peca peca) {
    if (topo >= TAMANHO_PILHA - 1) {
        printf("\n[AVISO] A pilha de reserva esta cheia! Maximo de %d pecas.\n", TAMANHO_PILHA);
        return;
    }
    topo++;
    pilha[topo] = peca;
    printf("\n[RESERVA] Peca [%c] (ID: %d) enviada para o topo da reserva!\n", peca.tipo, peca.id);
}

struct Peca desempilhar() {
    struct Peca pecaRemovida = pilha[topo];
    topo--;
    return pecaRemovida;
}

// Funcao para desenhar o estado atual do jogo na tela
void exibirEstado() {
    printf("\n=========================================\n");
    
    // 1. Exibir a Fila Circular
    printf(" PROXIMAS PECAS (FILA): ");
    int indice = frente;
    for (int i = 0; i < totalFila; i++) {
        if (i == 0) {
            printf("[%c](ID:%d) <= PROX ", fila[indice].tipo, fila[indice].id);
        } else {
            printf("| [%c](ID:%d) ", fila[indice].tipo, fila[indice].id);
        }
        indice = (indice + 1) % TAMANHO_FILA;
    }
    
    // 2. Exibir a Pilha de Reserva de cima para baixo
    printf("\n\n RESERVA (PILHA):\n");
    if (topo == -1) {
        printf("  [ VAZIA ]\n");
    } else {
        for (int i = topo; i >= 0; i--) {
            if (i == topo) {
                printf("  | [%c](ID:%d) | <-- TOPO (Disponivel)\n", pilha[i].tipo, pilha[i].id);
            } else {
                printf("  | [%c](ID:%d) |\n", pilha[i].tipo, pilha[i].id);
            }
        }
        printf("  +-----------+\n");
    }
    printf("=========================================\n");
}

int main() {
    srand(time(NULL));
    char buffer[100];
    int opcao;

    // Inicializa a fila com 5 pecas obrigatorias
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(gerarPeca());
    }

    do {
        exibirEstado(); // Exibe fila e pilha atualizadas a cada rodada
        
        printf("\n--- TETRIS STACK (AVENTUREIRO) ---\n");
        printf("1 - Jogar peca da frente\n");
        printf("2 - Reservar peca da frente\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        opcao = atoi(buffer);

        switch (opcao) {
            case 1: // JOGAR PEÇA
                if (totalFila > 0) {
                    struct Peca jogada = desenfileirar();
                    printf("\n[TABULEIRO] Voce jogou a peca [%c] (ID: %d)!\n", jogada.tipo, jogada.id);
                }
                break;
                
            case 2: // RESERVAR PEÇA (PUSH)
                if (topo >= TAMANHO_PILHA - 1) {
                    printf("\n[ERRO] Nao ha espaco na reserva. Use uma peca primeiro!\n");
                } else {
                    // Tira da frente da fila e joga na pilha de reserva
                    struct Peca paraReserva = desenfileirar();
                    empilhar(paraReserva);
                }
                break;
                
            case 3: // USAR PEÇA RESERVADA (POP)
                if (topo == -1) {
                    printf("\n[ERRO] Nao ha pecas guardadas na reserva!\n");
                } else {
                    struct Peca doTopo = desempilhar();
                    printf("\n[TABULEIRO] Voce resgatou e jogou a peca da reserva: [%c] (ID: %d)!\n", doTopo.tipo, doTopo.id);
                }
                break;
                
            case 0:
                printf("\nEncerrando o Tetris Stack Aventureiro. Progresso salvo!\n");
                break;
                
            default:
                printf("\n[ERRO] Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}