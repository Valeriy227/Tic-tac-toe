#include <iostream>
#include <random>
#include <vector>

#include "move.h"

class Position {
public:
  Position() { board.resize(3, std::vector<int>(3, 0)); }

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
  
  std::vector<Move> GetPossibleMoves(int player) {
    std::vector<Move> list_of_moves;
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
    std::vector<char> sign = {'.', 'x', 'o'};
    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 3; col++) {
        std::cout << sign[board[row][col]];
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

public:
  std::vector<std::vector<int>> board;
  std::vector<Move> history;
};