#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CLEAR system("clear")

#define BOARD 14
#define U_Kahala ((BOARD-2)/2)
#define C_Kahala (BOARD-1)

typedef enum {
    Computer,
    User,
    Undefined
} player_t;

// ----------------------------------------------------------------------------------------------------------------- //
//                                                 Defining the Game                                                 //
// ----------------------------------------------------------------------------------------------------------------- //

void board_new(int b[]) {
    for(int i=0; i < BOARD; i++) b[i] = 4;
    b[U_Kahala] = 0;
    b[C_Kahala] = 0;
}

bool is_valid(int board[], player_t player, int hole) {

    if (hole < 0 || player == Undefined) {
        int start = player == Computer ? BOARD/2 : 0;
        int end = player == User ? BOARD/2 : BOARD;
        bool valid = false;

        for (int i = start; i < end; i++) if ( board[i] > 0 ) {
            valid = true;
            break;
        }

        return valid;
    } else  {
        assert(hole <= BOARD/2-1);
        if (player == Computer) hole += BOARD/2; // Calculate the position on the vector.
        return board[hole] > 0;
    }

}

player_t exec_move(int board[], player_t player, int hole) {
    assert(hole <= BOARD/2-1 && hole >= 0 && player != Undefined);

    bool next;
    int kahala_adver, kahala_player, i, start, end;

    if (player == User) {
        next = Computer;
        kahala_player = U_Kahala;
        kahala_adver = C_Kahala;

        start = 0;
        end = BOARD/2;
    } else {
        next = User;
        kahala_player = C_Kahala;
        kahala_adver = U_Kahala;
        hole += BOARD/2; // Calculate the position on the vector.

        start = BOARD/2;
        end = BOARD;
    }

    // the player cannot choose an empty hole
    if(board[hole] == 0) {
        printf("You cannot choose an empty hole!\n");
        getchar();
        next = player;
        return next;
    }
    // Spread the seeds
    for ( i = hole+1; board[hole] > 0; i = (i+1) % BOARD){
         if ( i != kahala_adver ) {
            board[i]++;
            board[hole]--;
        }
    }
    if(i==0)
        i=13; // if 'i' was the computer's kahala
    else
        i--; // Make `i` point to the last hole incremented

    // The player wins a free turn. Only when the last filled hole was the player's kahala
    if(i == kahala_player){
        next = player;
        printf("player wins a free turn!\n");
        getchar();
    }

    // If the last filled hole was empty and it was one of the player holes, AND the opposite isnt empty
    // The player sends both his and the opponent's khala contents to his kahala;
    if (board[i] == 1 && i >= start && i < end) {
        if ( i != kahala_player ) {
            int opposite = BOARD-i-2; // Should work, no matter who's the player.

            // Transfer the hole content
            getchar();
            if(board[opposite] != 0){
                board[kahala_player] += board[opposite];
                board[opposite] = 0;
                board[kahala_player]++;
                board[i]--;
            }
        }
    }

    if( is_valid(board, next, -1) )
        return next;
    else
        return Undefined;
}

player_t end_game(int board[]) {
    for (int i=0; i < U_Kahala; i++) {
        board[U_Kahala] += board[i];
        board[i] = 0;
    }

    for (int i=U_Kahala+1; i < C_Kahala; i++) {
        board[C_Kahala] += board[i];
        board[i] = 0;
    }

    if (board[U_Kahala] == board[C_Kahala])
        return Undefined;
    else if (board[U_Kahala] > board[C_Kahala])
        return User;
    else
        return Computer;
}

void print_board(int *board, int jogadores) {
    CLEAR;
    printf("┌────────────────────────────────────────────────────────────────┐\n");
    if (jogadores==2)
        printf("│ Player 2                                                       │\n");
    else
        printf("│ Computer                                                       │\n");
    printf("│                                                                │\n");
    printf("│ ┌↑───┐  ┌6───┐  ┌5───┐  ┌4───┐  ┌3───┐  ┌2───┐  ┌1───┐  ┌────┐ │\n");

    printf("│ │    │ ");
    for(int i=BOARD-2; i > U_Kahala; i--) printf(" │ %2d │ ", board[i]);
    printf(" │    │ │\n");

    printf("│ │    │  └────┘  └────┘  └────┘  └────┘  └────┘  └────┘  │    │ │\n");
    printf("│ │ %2d │                                                  │ %2d │ │\n",
        board[C_Kahala], board[U_Kahala]);
    printf("│ │    │  ┌1───┐  ┌2───┐  ┌3───┐  ┌4───┐  ┌5───┐  ┌6───┐  │    │ │\n");

    printf("│ │    │ ");
    for(int i=0; i < U_Kahala; i++) printf(" │ %2d │ ", board[i]);
    printf(" │    │ │\n");

    printf("│ └────┘  └────┘  └────┘  └────┘  └────┘  └────┘  └────┘  └↓───┘ │\n");
    printf("│                                                                │\n");

    printf(jogadores == 2
        ? "│                                                       Player 1 │\n"
        : "│                                                            You │\n"
    );

    printf("└────────────────────────────────────────────────────────────────┘\n\n");
}

void print_menu() {
    CLEAR;
    printf(" _______   _____________ ________  ________                                \n");
    printf("|  ___\\ \\ / |_   _| ___ |  ___|  \\/  |  ___|                            \n");
    printf("| |__  \\ V /  | | | |_/ | |__ | .  . | |__                                \n");
    printf("|  __| /   \\  | | |    /|  __|| |\\/| |  __|                              \n");
    printf("| |___/ /^\\ \\ | | | |\\ \\| |___| |  | | |___                            \n");
    printf("\\____/\\/   \\/ \\_/ \\_| \\_\\____/\\_|  |_\\____/                       \n");
    printf("                       ___  ___ ___  _   _ _____  ___  _      ___          \n");
    printf("                       |  \\/  |/ _ \\| \\ | /  __ \\/ _ \\| |    / _ \\   \n");
    printf("                       | .  . / /_\\ |  \\| | /  \\/ /_\\ | |   / /_\\ \\  \n");
    printf("                       | |\\/| |  _  | . ` | |   |  _  | |   |  _  |       \n");
    printf("                       | |  | | | | | |\\  | \\__/| | | | |___| | | |      \n");
    printf("                       \\_|  |_\\_| |_\\_| \\_/\\____\\_| |_\\_____\\_| |_/\n");
    printf("                 __  ____ ____________  ______ __________  _______  ___  ___    \n");
    printf("                / / / / //_  __/  _/  |/  / _ /_  __/ __/ |_  / _ \\/ _ \\/ _ \\\n");
    printf("               / /_/ / /__/ / _/ // /|_/ / __ |/ / / _/  _/_ / // / // / // /   \n");
    printf("               \\____/____/_/ /___/_/  /_/_/ |_/_/ /___/ /____\\___/\\___/\\___/\n");
    printf("                   __  _________   __   _____  ________  ____  ___              \n");
    printf("                  /  |/  /  _/ /  / /  / __/ |/ /  _/ / / /  |/  /              \n");
    printf("                 / /|_/ _/ // /__/ /__/ _//    _/ // /_/ / /|_/ /               \n");
    printf("                /_/  /_/___/____/____/___/_/|_/___/\\____/_/  /_/               \n");
    printf("                   _______  __________________  _  __                           \n");
    printf("                  / __/ _ \\/  _/_  __/  _/ __ \\/ |/ /                         \n");
    printf("                 / _// // _/ /  / / _/ // /_/ /    /                            \n");
    printf("                /___/____/___/ /_/ /___/\\____/_/|_/                            \n");
    getchar();
    return;
}

int get_move(void) {
    int hole = 0, input = 0;
    char buf[200] = "";

    do {

        printf("Digite o numero de uma casa [1-%d]: ", BOARD/2-1);
        scanf("%[^\n]s", buf);
        getchar();

        input = atoi(buf);

        if (input > 0 && input < BOARD/2) hole = input;
        else printf("Entrada Invalida.\n");

    } while (!hole);

    return hole-1;
}

bool game_over(int board[]) {
    int sum1, sum2;
    sum1 = sum2 = 0;
    for (int i = 0; i < U_Kahala; ++i) {
        sum1 += board[i];
        sum2 += board[i+7];
    }

    return (sum1 == 0) || (sum2 == 0);
}

// End - Game ------------------------------------------------------------------------------------------------------ //

// ----------------------------------------------------------------------------------------------------------------- //
//                                       Defining the Artificial Intelligence                                        //
// ----------------------------------------------------------------------------------------------------------------- //

typedef struct Node {
    int fac, board[BOARD];
    player_t turn, next_turn;
    bool ended;
    struct Node *child[BOARD/2-1];
} node_t;

node_t* node_new(player_t p1, player_t p2) {
    node_t *new = (node_t*)malloc(sizeof(node_t));

    new -> fac = 0;
    new -> ended = false;
    new -> turn = p1;
    new -> next_turn = p2;

    // Clean the Board
    for (int i=0; i < BOARD; i++) new -> board[i] = 0;
    for (int i=0; i < BOARD/2-1; i++) new -> child[i] = NULL;

    return new;
}

void node_drop(node_t *node) {
    if (node) {
        for (int i=0; i < BOARD/2-1; i++) node_drop(node -> child[i]);
        free(node);
    }
}

void make_tree(node_t *node, int level) {
    // Condition to stop recursion
    if (level <= 0) return;

    for (int i=0; i < BOARD/2-1; i++) {
        // Create the new child
        node_t *new = node_new(node->next_turn, Undefined);
        node->child[i] = new;

        memcpy(new->board, node->board, sizeof(int) * BOARD);
        new->next_turn = exec_move(new->board, node->turn, i);

        if (new->next_turn != Undefined) make_tree(new, level-1);
        else new->ended = true;
    }
}

int node_quality(node_t *node) {
    if (node -> ended) {
        if (node -> next_turn == Computer) return 2000;
        else return -2000;
    } else {
        int user=0, computer=0;

        for(int i=0; i <= U_Kahala; i++) {
            user += node->board[i];
        }

        for(int i=U_Kahala; i <= C_Kahala; i++) {
            computer += node->board[i];
        }
        return computer - user;
    }
}

void node_calc(node_t *node) {
    bool empty = true;

    for (int i=0; i < BOARD/2-1; i++) {
        if(node->child[i]) {
            empty = false;
            node_calc(node->child[i]);
        }
    }

    if(empty) {
        for (int i=0; i < BOARD/2-1; i++) {
            node->fac = node_quality(node);
        }
    }

    if(!empty) {
        int max = -5000;

        for (int i=0; i < BOARD/2-1; i++) {
            if (node->child[i]->fac > max) max = node->child[i]->fac;
        }
        node->fac=max;
    }

}

int decide(int board[], int level) {
    int hole;
    node_t *node = node_new(Computer, User);
    memcpy(node -> board, board, sizeof(int) * BOARD);
    make_tree(node, level);
    node_calc(node);

    int max = -5000;

    for (int i=0; i < BOARD/2-1; i++) {
        if (node->child[i]->fac > max) {
            max = node->child[i]->fac;
            hole=i;
            printf("node->child[i]->fac = %d\nmax=%d\nhole=%d\n",node->child[i]->fac, max, hole);
            getchar();
        }
    }

    node_drop(node);
    return hole;
}

void debug_node(node_t *node, int level, bool last) {
    if(!node) return;
    for(int i=0; i < level; i++) printf("│  ");

    if (level != -1)
        last ? printf("└─ ") : printf("├─ ");

    if (!node) {
        printf("NULL\n");
    } else if (node->turn == Undefined || node->next_turn == Undefined) {
        printf("Invalid\n");
    } else {

        printf("Node { Fact: %d, ", node -> fac);

        // Print the Turn
        node -> turn == User ? printf("Turn: (User ") : printf("Turn: (Computer ");
        node -> next_turn == User ? printf("-> User)") : printf("-> Computer)");

        printf(", User: [");
        for(int i=0; i < 6; i++) printf("%d, ", node -> board[i]);
        printf("│%d│], Computer: [", node -> board[6]);
        for(int i=7; i < BOARD-1; i++) printf("%d, ", node -> board[i]);
        printf("│%d│] }\n", node-> board[BOARD]);
        // Print the childs
        for (int i=0; i < BOARD/2-1; i++) debug_node(node -> child[i], level+1, i == 5);
    }
}

// End - AI -------------------------------------------------------------------------------------------------------- //

int main(int argc, char const *argv[]) {
    print_menu();
    int level, jogadores;
    if (argc < 3) {
        fprintf(stderr, "É necessário especificar a dificuldade e o número de jogadores.\n");
        exit(1);
    } else {
        level = atoi(argv[1]);
        jogadores = atoi(argv[2]);
        if (level <= 0 || level > 3 ) {
            fprintf(stderr, "Nível Dificuldade Invalido.\nO nível de dificuldade é de no mínimo 1 e no máximo 3.\n");
            exit(1);
        }
        if (jogadores <= 0 || jogadores > 2) {
            fprintf(stderr, "Número de Jogadores Inválido.\nApenas 1 ou 2 jogadores.\n");
            exit(1);
        }
        level += 2;
    }

    int b[BOARD];
    board_new(b);

    player_t player = Computer;
    while (is_valid(b, player, -1)) {
        print_board(b,jogadores);
        int move;

        if (player == User) {
            if(jogadores==2)
                printf("Player 1:\n");
            move = get_move();
        } else if ((player == Computer) && jogadores == 1) {
            //move = decide(b, level);
            move = rand() % 6;
        } else if ((player == Computer) && jogadores == 2) {
            printf("Player 2:\n");
            move = get_move();
        } else break;

        player = exec_move(b, player, move);
        if(game_over(b)){
            printf("GAME OVER!\n");
            getchar();
            break;
        }
    }

    CLEAR;
    player = end_game(b);
    print_board(b, jogadores);

    if (player == Undefined)
        printf("Result: Tie\n");
    else if (player == User) {
        if (jogadores == 1)
            printf("Result: You Won!!!\n");
        else
            printf("Result: Player 1 Wins!\n");
    } else {
        if (jogadores == 2)
            printf("Result: Player 2 Wins!\n");
        else
            printf("Result: Computer Won.\n");
    }

    return 0;
}
