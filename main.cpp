#import <iostream>
#import <vector>
#include <algorithm>

using namespace std;

class Move {
    public:
    Move() {}
    Move(double rate) : rate(rate) {}
    Move(int row, int col, int player = -1) : row(row), col(col), player(player) {}
    static Move GetBestMove(vector<Move>& list_of_moves) {
        for (auto move : list_of_moves) {
            if (move.rate > 0.99) {
                return move;
            }
        }
        auto best_move = *max_element(list_of_moves.begin(), list_of_moves.end(),
        [](Move a, Move b) {
            return a.rate < b.rate;
        });
        return best_move;
    }
public:
    int row;
    int col;
    int player;
    double rate;
};


class Position {
public:
    Position() {
        board.resize(3, vector<int>(3, 0));
    }
    void MakeMove(Move move) {
        history.push_back(move);
        board[move.row][move.col] = move.player;
    }
    void MakeMoveBack() {
        auto move = history.back();
        history.pop_back();
        board[move.row][move.col] = 0;
    }
    bool IsWinner(int player) {
        // check rows
        for (int row = 0; row < 3; ++row) {
            bool is_line = true;
            for (int col = 0; col < 3; ++col) {
                if (board[row][col] != player) {
                    is_line = false;
                    break;
                }
            }
            if (is_line) {
                return true;
            }
        }

        // check columns
        for (int col = 0; col < 3; ++col) {
            bool is_line = true;
            for (int row = 0; row < 3; ++row) {
                if (board[row][col] != player) {
                    is_line = false;
                    break;
                }
            }
            if (is_line) {
                return true;
            }
        }

        // check diagonals
        bool is_line = true;
        for (int i = 0; i < 3; ++i) {
            if (board[i][i] != player) {
                is_line = false;
                break;
            }
        }
        if (is_line) {
            return true;
        }

        is_line = true;
        for (int i = 0; i < 3; ++i) {
            if (board[2 - i][i] != player) {
                is_line = false;
                break;
            }
        }
        if (is_line) {
            return true;
        }
        
        return false;
    }
    bool IsDraw() {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (board[row][col] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
    vector<Move> GetPossibleMoves(int player) {
        vector<Move> list_of_moves;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (board[row][col] == 0) {
                    list_of_moves.push_back(Move(row, col, player));
                }
            }
        }
        return list_of_moves;
    }

    void Print() {
        vector<char> sign = {'.', 'x', 'o'};
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                cout << sign[board[row][col]];
            }
            cout << endl;
        }
        cout << endl;
    }

public:
    vector<vector<int>> board;
    vector<Move> history;
};


int OtherPlayer(int player) {
    return 3 - player;
}

double MakeRate(vector<Move>& moves_of_opponent) {
    double sum = 0;
    for (auto& move : moves_of_opponent) {
        if (move.rate > 0.99) {
            return 0;
        } else {
            sum += move.rate;
        }
    }
    return 1 - sum / moves_of_opponent.size();
}

vector<Move> ComputeMove(Position& pos, int player) {
    if (pos.IsWinner(OtherPlayer(player))) {
        return {Move(0)};
    } else if (pos.IsDraw()) {
        return {Move(0.5)};
    }

    auto list_of_moves = pos.GetPossibleMoves(player);
    for (auto& move : list_of_moves) {
        pos.MakeMove(move);

        auto moves_of_opponent = ComputeMove(pos, OtherPlayer(player));
        move.rate = MakeRate(moves_of_opponent);

        pos.MakeMoveBack();
    }
    return list_of_moves;
}


class Interface {
public:
    void Run() {
        ChooseColour();

        pos.Print();

        int turn = 1;
        do {
            Move move;
            if (turn != user) {
                auto list_of_moves = ComputeMove(pos, turn);
                for (auto move : list_of_moves) {
                    cout << move.rate << " ";
                }
                cout << endl;
                move = Move::GetBestMove(list_of_moves);
            } else {
                move = EnterMove();
            }
            pos.MakeMove(move);
            turn = OtherPlayer(turn);

            pos.Print();            

            auto checkEnd = CheckEnd();
            if (CheckEnd() != -1) {
                EndGame(checkEnd);
                return;
            }
        } while (true);
    }

    void ChooseColour() {
        user = 0;
        do {
            cout << "Выбере сторону: 1 - крестики, 2 - нолики." << endl;
            cin >> user;
        } while (user != 1 && user != 2);
        cout << "Вводите ход вида '#строки #столбца', где # имеет значение 1, 2 или 3. (Клетка 1,1 - левый верхний угол)" << endl;
    }
    Move EnterMove() {
        Move move(-1, -1, user);
        do {
            cin >> move.row >> move.col;
            if (!(1 <= move.row && move.row <= 3 && 1 <= move.col && move.col <= 3)) {
                cout << "Ход введен некорректно. Повторите попытку." << endl;
            } else {
                break;
            }
        } while (true);

        move.row--;
        move.col--;
        return move;
    }
    int CheckEnd() {
        if (pos.IsDraw()) {
            return 0;
        } else if (pos.IsWinner(user)) {
            return 1;
        } else if (pos.IsWinner(OtherPlayer(user))) {
            return 2;
        }
        return -1;
    }
    void EndGame(int checkEnd) {
        switch (checkEnd) {
        case 0:
            cout << "Ничья" << endl;
            break;
        case 1:
            cout << "Победили крестики" << endl;
            break;
        case 2:
            cout << "Победили нолики" << endl;
            break;
        }
    }

public:
    Position pos;
    int user;
};

int main() {
    Interface interface;
    interface.Run();

}