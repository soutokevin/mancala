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

// Checks if a the player can excute de specified move.
// If 'player' is 'Undefined', it'll check for any possible move.
// If 'hole' is a negative number, it'll check if 'player' has moves.
bool is_move_valid(int board[], player_t player, int hole) {
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
        assert(hole <= BOARD/2-1); // Make sure is a valid number for 'hole'
        if (player == Computer) hole += BOARD/2; // Calculate the position on the vector.
        return board[hole] > 0;
    }
}

player_t exec_move(int board[], player_t player, int hole, bool silent) {
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
        if (!silent) {
            printf("You cannot choose an empty hole!\n");
            getchar();
        }
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
        if (!silent) {
            printf("player wins a free turn!\n");
            getchar();
        }
    }

    // If the last filled hole was empty and it was one of the player holes, AND the opposite isnt empty
    // The player sends both his and the opponent's khala contents to his kahala;
    if (board[i] == 1 && i >= start && i < end) {
        if ( i != kahala_player ) {
            int opposite = BOARD-i-2; // Should work, no matter who's the player.

            // Transfer the hole content
            if(board[opposite] != 0){
                board[kahala_player] += board[opposite];
                board[opposite] = 0;
                board[kahala_player]++;
                board[i]--;
            }
        }
    }

    return is_move_valid(board, next, -1) ? next : Undefined;
}

// Finishes the game and returns the winner
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

    printf(jogadores == 2
        ? "│ Player 2                                                       │\n"
        : "│ Computer                                                       │\n"
    );

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

bool game_over(int const board[]) {
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
    int board[BOARD];
    player_t turn, next_turn;
    bool ended;
    struct Node *child[BOARD/2-1];
} node_t;

node_t* node_new(player_t p1, player_t p2) {
    node_t *new = (node_t*)malloc(sizeof(node_t));

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

// Creates a game tree from the given 'board' with 'level' heigth
void make_tree(node_t *root, int const level) {
    // Condition to stop recursion
    if (level <= 0) return;

    for (int i = 0; i < (BOARD-2)/2; i++) {
        // Skip invalid moves
        if (!is_move_valid(root -> board, root -> next_turn, i)) continue;

        // Create the new child
        node_t *new = node_new(root -> next_turn, Undefined);
        memcpy(new -> board, root -> board, sizeof(int) * BOARD);
        root -> child[i] = new;

        new -> next_turn = exec_move(new -> board, root -> turn, i, true);
        if (!game_over(new -> board)) {
            make_tree(new, level-1);
        }
    }
}

// Returns a number that represents how good the game is to 'player'
int board_quality(int const board[], player_t player) {
    int user_moves = 0,
        computer_moves = 0,
        user_score = 0,
        computer_score = 0,
        score = 0;

    for (int i = 0; i <= U_Kahala; ++i) {
        user_score += board[i];
        computer_score += board[i+7];

        if (i != U_Kahala) {
            if (board[i] != 0) user_moves++;
            if (board[i+7] != 0 ) computer_moves++;
        }
    }

    if (player == User) {
        if (user_moves > computer_moves) score++;
        if (user_score > computer_score) score += 2;
    } else if (player == Computer) {
        if (computer_moves > user_moves) score++;
        if (computer_score > user_score) score += 2;
    }

    return score;
}

// Walks the tree to get the child with the best board quality for the computer
int min_max(node_t *root) {
    if (root == NULL) return 0;

    int min = 100000,
        max = -100000,
        min_index = -1,
        max_index = -1;

    for (int i = 0; i < (BOARD-2)/2; i++) {
        if (root -> child[i] == NULL) continue;

        int quality = board_quality(root -> child[i] -> board, root -> child[i] -> turn);
        int best = min_max(root -> child[i]);
        quality += best;

        if (quality < min) {
            min = quality;
            min_index = i;
        }

        if (quality > max) {
            max = quality;
            max_index = i;
        }
    }

    return root -> turn == User ? min_index : max_index;
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
        printf("Node { ");

        // Print the Turn
        node -> turn == User ? printf("Turn: (User ") : printf("Turn: (Computer ");
        node -> next_turn == User ? printf("-> User)") : printf("-> Computer)");

        printf(", User: [");
        for (int i=0; i < 6; i++) printf("%d, ", node -> board[i]);
        printf("│%d│], Computer: [", node -> board[6]);
        for (int i=7; i < BOARD-1; i++) printf("%d, ", node -> board[i]);
        printf("│%d│] }\n", node-> board[BOARD-1]);
        // Print the childs
        for (int i=0; i < BOARD/2-1; i++) debug_node(node -> child[i], level+1, i == 5);
    }
}

void debug_tree(node_t *root) {
    debug_node(root, -1, false);
    getchar();
}

int decide(int const board[], int level) {
    node_t *root = node_new(User, Computer);
    memcpy(root -> board, board, sizeof(int) * BOARD);
    make_tree(root, level);
    int move = min_max(root);
    node_drop(root);
    return move;
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
    while (is_move_valid(b, player, -1)) {
        print_board(b,jogadores);
        int move;

        if (player == User) {
            if(jogadores==2)
                printf("Player 1:\n");
            move = get_move();
        } else if ((player == Computer) && jogadores == 1) {
            move = decide(b, level);
        } else if ((player == Computer) && jogadores == 2) {
            printf("Player 2:\n");
            move = get_move();
        } else break;

        player = exec_move(b, player, move, false);
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
