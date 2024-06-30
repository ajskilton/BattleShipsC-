#include "AiBot.h"

struct MoveData {
		int row;
		int col;
		bool isHit;
		bool isDestroyed;
	};

AiBot::AiBot(void) {
    // create a list of moves
  std::list<MoveData> moves;
  // inital state is ship search  
  state = SHIP_SEARCH;
  direction = 0;
}

void AiBot::getMove(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {
  // do state transitions
  if(moves.empty()) {
    state = SHIP_SEARCH;
  } else if(state == SHIP_SEARCH && moves.back().isHit) {
    state = HIT_MODE;
  } else if(state == HIT_MODE && moves.back().isHit && !moves.back().isDestroyed) {
    state = DESTROY_MODE;
  }

  if (!moves.empty()) {
    if (moves.back().isDestroyed) {
      state = SHIP_SEARCH;
    }
  }

  if(state == SHIP_SEARCH) {
    searchMode(shots, row, col);
    return;
  } else if (state == HIT_MODE) {
    hitMode(shots, row, col);
    return;
  } else if (state == DESTROY_MODE) {
    destroyMode(shots, row, col);
    return;
  } else if (state == ORIENTATION_MODE) {
    orientationMode(shots, row, col);
    return;
  }
}

void AiBot::addMove(int row, int col, bool isHit, bool isDestroyed) {
  MoveData move;
  move.row = row;
  move.col = col;
  move.isHit = isHit;
  move.isDestroyed = isDestroyed;
  moves.push_back(move);
  return;
}

void AiBot::searchMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {
  // generate random move
  int rand_row, rand_col;

  do {
    rand_row = rand() % MAP_SIZE;
    rand_col = rand() % (MAP_SIZE/2);

    if (rand_row % 2 == 0) {
      rand_col *= 2;
    } else {
      rand_col = rand_col * 2 + 1;
    }
  } while (shots[rand_row][rand_col] > 0);

  *row = rand_row;
  *col = rand_col;
  return;
}

void AiBot::hitMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {

  // get position of most recent hit
  // Declare the reverse iterator of the list
  std::list<MoveData>::reverse_iterator it;
  // Traverse the  elements of the list using  the
  // reverse_iterator
  for (it = moves.rbegin(); it != moves.rend(); it++) {

    if(it->isHit) {
      break;
    }
  }

  int prev_row = it->row;
  int prev_col = it->col;

  for(int dir = 0; dir < 4; dir++) {
    if(dir == 0) {
      if(prev_row - 1 >= 0) {
        if(shots[prev_row - 1][prev_col] == 0) {
          *row = prev_row - 1;
          *col = prev_col;

          direction = dir;
          return;
        }
      }
    } else if(dir == 1) {
      if(prev_col + 1 < MAP_SIZE) {
        if(shots[prev_row][prev_col + 1] == 0) {
          *row = prev_row;
          *col = prev_col + 1;

          direction = dir;
          return;
        }
      }
    } else if(dir == 2) {
      if(prev_row + 1 < MAP_SIZE) {
        if(shots[prev_row + 1][prev_col] == 0) {
          *row = prev_row + 1;
          *col = prev_col;

          direction = dir;
          return;
        }
      }
    } else if(dir == 3) {
      if(prev_col - 1 >= 0) {
        if(shots[prev_row][prev_col - 1] == 0) {
          *row = prev_row;
          *col = prev_col - 1;

          direction = dir;
          return;
        }
      }
    } else {
      *row = prev_row;
      *col = prev_col;

      direction = dir;
      std::cout << "Error: No valid direction found";
      return;
    }
  }
}

void AiBot::destroyMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {
  // if last shot sunk the ship, go back to search mode
  if(moves.back().isDestroyed) {
    state = SHIP_SEARCH;
    searchMode(shots, row, col);
    return;
  } else if (!moves.back().isHit) {
    // last shot missed, re orrient and try again
    orientationMode(shots, row, col);
    return;
  }

  int prev_row = moves.back().row;
  int prev_col = moves.back().col;

  // shoot in state direction until miss or out of bounds
  if(direction == 0) {
    if(prev_row - 1 >= 0) {
      if(shots[prev_row - 1][prev_col] == 0) {
        *row = prev_row - 1;
        *col = prev_col;
        return;
      } else {
        orientationMode(shots, row, col);
        return;
      }
    } else {
      orientationMode(shots, row, col);
      return;
    }
  } else if(direction == 1) {
    if(prev_col + 1 < MAP_SIZE) {
      if(shots[prev_row][prev_col + 1] == 0) {
        *row = prev_row;
        *col = prev_col + 1;
        return;
      } else {
        orientationMode(shots, row, col);
        return;
      }
    } else {
      orientationMode(shots, row, col);
      return;
    }
  } else if(direction == 2) {
    
    if(prev_row + 1 < MAP_SIZE) {
      if(shots[prev_row + 1][prev_col] == 0) {
        *row = prev_row + 1;
        *col = prev_col;
        return;
      } else {
        orientationMode(shots, row, col);
        return;
      }
    } else {
      orientationMode(shots, row, col);
      return;
    }
  } else if(direction == 3) {
    
    if(prev_col - 1 >= 0) {
      if(shots[prev_row][prev_col - 1] == 0) {
        
        *row = prev_row;
        *col = prev_col - 1;
        return;
      } else {
        orientationMode(shots, row, col);
        return;
      }
    } else {
      orientationMode(shots, row, col);
      return;
    }
  } else {
      std::cout << "Error: No valid direction found";
    return;
  }
}

void AiBot::orientationMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {
  state = ORIENTATION_MODE;

  // reverse direction
  direction = (direction + 2) % 4;

  // shift in new direction until available shot is found
  int prev_row = moves.back().row;
  int prev_col = moves.back().col;

  while (shots[prev_row][prev_col] > 0) {
    if(direction == 0) {
      prev_row--;
    } else if(direction == 1) {
      prev_col++;
    } else if(direction == 2) {
      prev_row++;
    } else if(direction == 3) {
      prev_col--;
    } else {
      std::cout << "Error: No valid direction found";
      return;
    }
  }

  // switch to destroy mode
  state = DESTROY_MODE;
  *row = prev_row;
  *col = prev_col;
  return;
}
