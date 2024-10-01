#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

//This is a a new line

class TicTacToe {
public:
    vector<vector<char>> board{{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};

    void printBoard() {
        cout << "      |       |\n";
        cout << "  " << board[0][0] << "   |   " << board[0][1] << "   |   " << board[0][2] << "\n";
        cout << "      |       |\n";
        cout << "---------------------\n";
        cout << "      |       |\n";
        cout << "  " << board[1][0] << "   |   " << board[1][1] << "   |   " << board[1][2] << "\n";
        cout << "      |       |\n";
        cout << "---------------------\n";
        cout << "      |       |\n";
        cout << "  " << board[2][0] << "   |   " << board[2][1] << "   |   " << board[2][2] << "\n";
        cout << "      |       |\n";
        cout << "\n";
    }

    bool win(char p) {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == p && board[i][1] == p && board[i][2] == p) return true;
            if (board[0][i] == p && board[1][i] == p && board[2][i] == p) return true;
        }
        if (board[0][0] == p && board[1][1] == p && board[2][2] == p) return true;
        if (board[0][2] == p && board[1][1] == p && board[2][0] == p) return true;
        return false;
    }

    bool tie() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') return false;
            }
        }
        return true;
    }

    void getUserMove(char p) {
        int g;
        cout << "Please Enter Grid Number (1 - 9): ";
        cin >> g;
        g -= 1;
        int x = g / 3;
        int y = g % 3;
        if (board[x][y] == 'X' || board[x][y] == 'O') {
            cout << "Please Enter A Valid Move\n";
            getUserMove(p);
        } else {
            board[x][y] = p;
        }
    }

    void getEasyAIMove(char ai, char pl) {
        srand(time(NULL));
        vector<pair<int, int>> moves;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    moves.push_back({i, j});
                }
            }
        }
        pair<int, int> move = moves[rand() % moves.size()];
        board[move.first][move.second] = ai;
    }

    void getHardAIMove(char ai, char pl) {
        srand(time(NULL));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ai && board[i][j] != pl) {
                    char c = board[i][j];
                    board[i][j] = ai;
                    if (win(ai) || tie()) return;
                    board[i][j] = pl;
                    if (win(pl) || tie()) {
                        board[i][j] = ai;
                        return;
                    }
                    board[i][j] = c;
                }
            }
        }
        getEasyAIMove(ai, pl);
    }

    void getInsaneAIMove(char ai, char pl) {
        srand(time(NULL));
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ai && board[i][j] != pl) {
                    char c = board[i][j];
                    board[i][j] = ai;
                    if (win(ai) || tie()) return;
                    board[i][j] = pl;
                    if (win(pl) || tie()) {
                        board[i][j] = ai;
                        return;
                    }
                    board[i][j] = c;
                }
            }
        }
        vector<pair<int, int>> moves;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ai && board[i][j] != pl) {
                    moves.push_back({i, j});
                }
            }
        }
        pair<int, int> move = moves[rand() % moves.size()];
        board[move.first][move.second] = ai;
    }

    void play(char p1, char p2) {
        int difficulty;
        cout << "Choose difficulty level (1: Easy, 2: Hard, 3: Insane): ";
        cin >> difficulty;

        printBoard();

        while (true) {
            getUserMove(p1);
            printBoard();
            if (win(p1)) {
                cout << "Player 1 wins!\n";
                break;
            }
            if (tie()) {
                cout << "It's a tie!\n";
                break;
            }

            if (difficulty == 1) {
                getEasyAIMove(p2, p1);
            } else if (difficulty == 2) {
                getHardAIMove(p2, p1);
            } else {
                getInsaneAIMove(p2, p1);
            }

            printBoard();
            if (win(p2)) {
                cout << "AI wins!\n";
                break;
            }
            if (tie()) {
                cout << "It's a tie!\n";
                break;
            }
        }
    }

    void startGame() {
        play('X', 'O');
    }
};

int main() {
    TicTacToe game;
    game.startGame();
    return 0;
}
