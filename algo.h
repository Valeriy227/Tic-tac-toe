#include <iostream>
#include <random>
#include <vector>

#include "position.h"

int OtherPlayer(int player) { return 3 - player; }

double MakeRate(std::vector<Move> &moves_of_opponent) {
  double sum = 0;
  for (auto &move : moves_of_opponent) {
    if (move.rate == 0.5) {
      sum += 1.5 * move.rate;
    } else {
      sum += move.rate;
    }
  }
  return 1 - sum / moves_of_opponent.size();
}

std::vector<Move> ComputeMove(Position &pos, int player) {
  if (pos.IsWinner(OtherPlayer(player))) {
    return {Move(0)};
  } else if (pos.IsDraw()) {
    return {Move(0.5)};
  }

  auto list_of_moves = pos.GetPossibleMoves(player);
  for (auto &move : list_of_moves) {
    pos.MakeMove(move);

    auto moves_of_opponent = ComputeMove(pos, OtherPlayer(player));
    move.rate = MakeRate(moves_of_opponent);

    pos.MakeMoveBack();
  }

  return list_of_moves;
}