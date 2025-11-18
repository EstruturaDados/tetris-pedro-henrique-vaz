#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

int proximo_id = 0;

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;


Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice_tipo = rand() % 4;

    nova.nome = tipos[indice_tipo];
    nova.id = proximo_id++;

    return nova;
}


void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;

    for (int i = 0; i < MAX_FILA; i++) {
        fila->tras = (fila->tras + 1) % MAX_FILA;
        fila->pecas[fila->tras] = gerarPeca();
        fila->tamanho++;
    }
}

int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

int filaCheia(FilaCircular *fila) {
    return fila->tamanho == MAX_FILA;
}

int enfileirar(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0;
    }

    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
    return 1;
}

int desenfileirar(FilaCircular *fila, Peca *peca_removida) {
    if (filaVazia(fila)) {
        return 0;
    }

    *peca_removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->tamanho--;

    enfileirar(fila, gerarPeca());

    return 1;
}

Peca frenteFila(FilaCircular *fila) {
    return fila->pecas[fila->frente];
}


void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

int empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0;
    }

    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    return 1;
}

int desempilhar(Pilha *pilha, Peca *peca_removida) {
    if (pilhaVazia(pilha)) {
        return 0;
    }

    *peca_removida = pilha->pecas[pilha->topo];
    pilha->topo--;
    return 1;
}

Peca topoPilha(Pilha *pilha) {
    return pilha->pecas[pilha->topo];
}

int tamanhoPilha(Pilha *pilha) {
    return pilha->topo + 1;
}


void exibirEstado(FilaCircular *fila, Pilha *pilha) {
    printf("\n\n--- 🗺️ Estado Atual ---");
    printf("\n## Fila de Peças (Capacidade %d) ##", MAX_FILA);

    if (filaVazia(fila)) {
        printf("\nFila de peças está vazia.");
    } else {
        printf("\n[Frente -> Trás]: ");
        for (int i = 0; i < MAX_FILA; i++) {
            int indice = (fila->frente + i) % MAX_FILA;
            if (i < fila->tamanho) {
                 printf("[%c %d]", fila->pecas[indice].nome, fila->pecas[indice].id);
            } else {
                printf("[ - ]");
            }
            if (i < MAX_FILA - 1) printf(" ");
        }
    }

    printf("\n\n## Pilha de Reserva (Capacidade %d) ##", MAX_PILHA);

    if (pilhaVazia(pilha)) {
        printf("\nPilha de reserva está vazia.");
    } else {
        printf("\n(Topo -> Base): ");
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d]", pilha->pecas[i].nome, pilha->pecas[i].id);
            if (i > 0) printf(" ");
        }
    }
    printf("\n");
}


void reservarPeca(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n🚫 Erro: Fila de peças está vazia. Nenhuma peça para reservar.");
        return;
    }
    if (pilhaCheia(pilha)) {
        printf("\n🚫 Erro: Pilha de reserva está cheia. Nenhuma peça pode ser adicionada.");
        return;
    }

    Peca peca_reservar = frenteFila(fila);

    desenfileirar(fila, &peca_reservar);

    empilhar(pilha, peca_reservar);

    printf("\n✅ Ação: Peça [%c %d] foi reservada (movida da Fila para a Pilha).", peca_reservar.nome, peca_reservar.id);
}

void trocarPecaAtual(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\n🚫 Erro: Fila de peças está vazia. Não há peça para trocar.");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("\n🚫 Erro: Pilha de reserva está vazia. Não há peça para troca.");
        return;
    }

    Peca peca_fila = frenteFila(fila);

    Peca peca_pilha;
    desempilhar(pilha, &peca_pilha);

    Peca temp_removida;
    temp_removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->tamanho--;

    enfileirar(fila, peca_pilha);

    empilhar(pilha, peca_fila);

    printf("\n🔄 Ação: Troca realizada! Peça da Fila ([%c %d]) trocada com a peça da Pilha ([%c %d]).", 
           peca_fila.nome, peca_fila.id, peca_pilha.nome, peca_pilha.id);
}

void trocaMultipla(FilaCircular *fila, Pilha *pilha) {
    if (fila->tamanho < 3) {
        printf("\n🚫 Erro: Fila tem menos de 3 peças (%d). Ação cancelada.", fila->tamanho);
        return;
    }
    if (tamanhoPilha(pilha) < 3) {
        printf("\n🚫 Erro: Pilha tem menos de 3 peças (%d). Ação cancelada.", tamanhoPilha(pilha));
        return;
    }

    Peca temp_fila[3];
    Peca temp_pilha[3];

    for (int i = 0; i < 3; i++) {
        temp_fila[i] = fila->pecas[(fila->frente + i) % MAX_FILA];
    }
    fila->frente = (fila->frente + 3) % MAX_FILA;
    fila->tamanho -= 3;


    for (int i = 0; i < 3; i++) {
        desempilhar(pilha, &temp_pilha[i]);
    }

    
    for (int i = 2; i >= 0; i--) {
        enfileirar(fila, temp_pilha[i]);
    }


    for (int i = 0; i < 3; i++) {
        empilhar(pilha, temp_fila[i]);
    }

    printf("\n✨ Ação: Troca Múltipla realizada! As 3 primeiras peças da fila foram trocadas com as 3 peças da pilha.");
}


int main() {
    srand(time(NULL));

    FilaCircular fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int opcao;
    Peca peca_removida;

    do {
        exibirEstado(&fila, &pilha);
        
        printf("\n\n--- ⚙️ Opções Disponíveis ---");
        printf("\n1. Jogar peça da frente da fila (dequeue)");
        printf("\n2. Enviar peça da fila para a pilha de reserva (reservar)");
        printf("\n3. Usar peça da pilha de reserva (pop)");
        printf("\n4. Trocar peça da frente da fila com o topo da pilha");
        printf("\n5. Trocar os 3 primeiros da fila com as 3 peças da pilha");
        printf("\n0. Sair");
        printf("\n\nOpção escolhida: ");

        if (scanf("%d", &opcao) != 1) {
            printf("\n\n❌ Entrada inválida. Por favor, digite um número.");
            while (getchar() != '\n');
            opcao = -1;
            continue;
        }

        printf("\n");

        switch (opcao) {
            case 1:
                if (desenfileirar(&fila, &peca_removida)) {
                    printf("\n🎉 Ação: Peça JOGADA (removida da fila): [%c %d]", peca_removida.nome, peca_removida.id);
                    printf("\n... Uma nova peça foi gerada e adicionada ao final da fila.");
                } else {
                    printf("\n🚫 Erro: Fila de peças vazia. Não é possível jogar.");
                }
                break;

            case 2:
                reservarPeca(&fila, &pilha);
                break;

            case 3:
                if (desempilhar(&pilha, &peca_removida)) {
                    printf("\n🗑️ Ação: Peça USADA (removida da pilha): [%c %d]", peca_removida.nome, peca_removida.id);
                } else {
                    printf("\n🚫 Erro: Pilha de reserva vazia. Nenhuma peça para usar.");
                }
                break;
                
            case 4:
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\n👋 Encerrando o Gerenciador de Peças. Até mais!\n");
                break;

            default:
                printf("\n❌ Opção inválida. Tente novamente.");
                break;
        }
        
        if (opcao != 0) {
            printf("\n\nPressione ENTER para continuar...");
            while (getchar() != '\n');
            getchar();
        }
        
    } while (opcao != 0);

    return 0;
}
