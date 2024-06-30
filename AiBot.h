#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <list>
#define MAP_SIZE 6
#define HIT 1000

class AiBot {
private:
	struct MoveData {
		int row;
		int col;
		bool isHit;
		bool isDestroyed;
	};
	enum AiState {
		SHIP_SEARCH, HIT_MODE, DESTROY_MODE, ORIENTATION_MODE
		};
	AiState state;
	int direction;
	std::list<MoveData> moves;
public:
	AiBot(void);
	void getMove(int shotsArray[MAP_SIZE][MAP_SIZE], int* row, int* col);
	void addMove(int row, int col, bool isHit, bool isDestroyed);
	void searchMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);
	void hitMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);
	void destroyMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);
	void orientationMode(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);
};


