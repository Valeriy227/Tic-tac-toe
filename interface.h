#include <iostream>
#include <random>
#include <vector>

#include "algo.h"

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
      std::cout << "Выбере сторону: 1 - крестики, 2 - нолики." << std::endl;
      std::cin >> user;
    } while (user != 1 && user != 2);
    std::cout << "Вводите ход вида '#строки #столбца', где # имеет значение 1, "
                 "2 или 3. (Клетка 1,1 - левый верхний угол)"
              << std::endl;
  }

  Move EnterMove() {
    Move move(-1, -1, user);
    do {
      std::cin >> move.row >> move.col;
      move.row--;
      move.col--;
      if (!(0 <= move.row && move.row <= 2 && 0 <= move.col && move.col <= 2)) {
        std::cout << "Ход введен некорректно. Повторите попытку." << std::endl;
      } else if (pos.board[move.row][move.col] != 0) {
        std::cout << "Клетка занята. Повторите попытку." << std::endl;
      } else {
        break;
      }
    } while (true);

    return move;
  }

  int CheckEnd() {
    if (pos.IsWinner(1)) {
      return 1;
    } else if (pos.IsWinner(2)) {
      return 2;
    } else if (pos.IsDraw()) {
      return 0;
    }
    return -1;
  }

  void EndGame(int checkEnd) {
    switch (checkEnd) {
    case 0:
      std::cout << "Ничья" << std::endl;
      break;
    case 1:
      std::cout << "Победили крестики" << std::endl;
      break;
    case 2:
      std::cout << "Победили нолики" << std::endl;
      break;
    }
  }

public:
  Position pos;
  int user;
};