#include <algorithm>
#include <random>
#include <vector>

class Move {
public:
  Move() {}
  Move(double rate) : rate(rate) {}
  Move(int row, int col, int player = -1)
      : row(row), col(col), player(player) {}

  static Move GetBestMove(std::vector<Move> &list_of_moves) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(list_of_moves.begin(), list_of_moves.end(), gen);

    auto best_move =
        *max_element(list_of_moves.begin(), list_of_moves.end(),
                     [](Move a, Move b) { return a.rate < b.rate; });
    return best_move;
  }

public:
  int row;
  int col;
  int player;
  double rate;
};