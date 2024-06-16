/* ENGGEN131 C Project 2023 - Battleships */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 6
#define HIT 1000

// Prototype declarations of required functions
void PrintArray(int values[MAP_SIZE][MAP_SIZE]);
void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]);
void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]);
int CountValues(int value, int map[MAP_SIZE][MAP_SIZE]);
int TopLeftPosition(int size, int* row, int* col, int map[MAP_SIZE][MAP_SIZE]);
int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]);
void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]);
void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col);
int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]);

/*
* GodBot Class
*/

class GodBot {
private:
	int prevShots[MAP_SIZE][MAP_SIZE];
	int prevRow;
	int prevCol;

public:
	GodBot(int shots[MAP_SIZE][MAP_SIZE]) {
		for (int i = 0; i < MAP_SIZE; ++i) {
			for (int j = 0; j < MAP_SIZE; ++j) {
				prevShots[i][j] = shots[i][j];
			}
		}
	}

	void getMoveGodBot(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {

		int rand_row;
		int rand_col;

		if (prevMoveHit(shots) || ) {
			int rand_dir = rand() % 4;
			// check around prev move
			getNextShot(prevRow, prevCol, rand_dir, shots, row, col);
			return;

		}
		else {

			do {
				rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
				rand_row = rand() % MAP_SIZE;
				rand_col = rand() % MAP_SIZE;
			} while (shots[rand_row][rand_col] > 0);

			*row = rand_row;
			prevRow = rand_row;
			*col = rand_col;
			prevCol = rand_col;
		}
	}

	bool prevMoveHit(int shots[MAP_SIZE][MAP_SIZE]) {
		if (CountValues(HIT, prevShots) < CountValues(HIT, shots)) {
			return true;
		}

		return false;
	}

	void getNextShot(int prevRow, int prevCol, int rand_dir, int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {
		switch (rand_dir) {
		case 0: // up
			if (prevRow - 1 >= 0 && shots[prevRow - 1][prevCol] == 0) {
				*row = prevRow - 1;
				*col = prevCol;
				break;
			}
		case 1: // down
			if (prevRow + 1 < MAP_SIZE && shots[prevRow + 1][prevCol] == 0) {
				*row = prevRow + 1;
				*col = prevCol;
				break;
			}
		case 2: // left
			if (prevCol - 1 >= 0 && shots[prevRow][prevCol - 1] == 0) {
				*row = prevRow;
				*col = prevCol - 1;
				break;
			}
		case 3: // right
			if (prevCol + 1 < MAP_SIZE && shots[prevRow][prevCol + 1] == 0) {
				*row = prevRow;
				*col = prevCol + 1;
				break;
			}
		}

		return;
	}
};



/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go below
*******************************************************************************
*******************************************************************************
******************************************************************************/

void PrintArray(int values[MAP_SIZE][MAP_SIZE])
{
	// iterate across array
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			// print values
			printf("%d ", values[i][j]);
		}
		// new row
		printf("\n");
	}
}

void InitialiseMap(int map[MAP_SIZE][MAP_SIZE])
{
	// initalise map
	for (int r = 0; r < MAP_SIZE; r++) {
		for (int c = 0; c < MAP_SIZE; c++) {
			map[r][c] = 0;
		}
	}

	// initialise ship positions and lengths
	int shipSizes[4] = { 2, 3, 4, 5 };
	int row[4] = { 0, 4, 2, 1 };
	int col[4] = { 0, 2, 0, 5 };

	// Place horizontal ships (0, 2)
	for (int x = 0; x < 4; x += 2) {
		for (int i = col[x]; i < col[x] + shipSizes[x]; i++) {
			map[row[x]][i] = shipSizes[x];
		}
	}

	// Place vertical ships (1, 3)
	for (int x = 1; x < 4; x += 2) {
		for (int i = row[x]; i < row[x] + shipSizes[x]; i++) {
			map[i][col[x]] = shipSizes[x];
		}
	}
}

void AddShip(int size, int pos[2], int direction, int map[MAP_SIZE][MAP_SIZE])
{

	switch (direction) {
	case 1: // horizontal ship
		for (int i = pos[1]; i < pos[1] + size; i++) {
			map[pos[0]][i] = size;
		}
		break;
	case 2: // vertical ship
		for (int i = pos[0]; i < pos[0] + size; i++) {
			map[i][pos[1]] = size;
		}
		break;
	}
}

void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE])
{
	// 1 = hor, 2 = vert;
	int direction = (rand() % 2) + 1;
	int position[2] = { 0 };

	switch (direction) {
	case 1: // case of horizontal values
		position[0] = rand() % (MAP_SIZE);
		position[1] = rand() % (MAP_SIZE - size + 1); // all valid placements
		break;
	case 2: // case of vertical values
		position[0] = rand() % (MAP_SIZE - size + 1); // all valid placements
		position[1] = rand() % (MAP_SIZE);
		break;
	}

	AddShip(size, position, direction, map);
}

int CountValues(int value, int map[MAP_SIZE][MAP_SIZE])
{
	int count = 0;

	// iterate across array
	for (int r = 0; r < MAP_SIZE; r++) {
		for (int c = 0; c < MAP_SIZE; c++) {
			// count 1 everytume value is exactly the same as the map value
			count += (value == map[r][c]);
		}
	}

	return count;
}

int TopLeftPosition(int size, int* row, int* col, int map[MAP_SIZE][MAP_SIZE])
{
	int count = 0;
	// assume facing down
	int facing = 2;

	// iterate across array top left to bottom right
	for (int r = 0; r < MAP_SIZE; r++) {
		for (int c = 0; c < MAP_SIZE; c++) {
			if (size == map[r][c] && count < 1) {
				// set row and col
				*row = r;
				*col = c;
				// count 1 so only first element is detected
				count++;
			}
			else if (size == map[r][c] && c == *col + 1) {
				// if next col is = to size value: facing right
				facing = 1;
			}
		}
	}
	return facing;
}

int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE])
{
	// initise values
	int row = -1, col = -1;
	int i = 0;

	// get position of ship
	int direction = TopLeftPosition(size, &row, &col, map);

	// check for ship overlap
	if (direction == 2) {
		for (i = row; i < row + size; i++) {
			if (map[i][col] != size) {
				return 0;
			}
		}
	}
	else {
		for (i = col; i < col + size; i++) {
			if (map[row][i] != size) {
				return 0;
			}
		}
	}

	// check for close ships

	// check ends
	if (direction == 2) {
		if ((row - 1) >= 0) {
			if (map[row - 1][col] != 0) {
				return 0;
			}
		}

		if ((row + size) < MAP_SIZE) {
			if (map[row + size][col] != 0) {
				return 0;
			}
		}
	}
	else {
		if ((col - 1) >= 0) {
			if (map[row][col - 1] != 0) {
				return 0;
			}
		}

		if ((col + size) < MAP_SIZE) {
			if (map[row][col + size] != 0) {
				return 0;
			}
		}
	}

	// check sides
	if (direction == 2) {
		if ((col - 1) >= 0) {
			for (i = row; i < row + size; i++) {
				if (map[i][col - 1] != 0) {
					return 0;
				}
			}
		}

		if ((col + 1) < MAP_SIZE) {
			for (i = row; i < row + size; i++) {
				if (map[i][col + 1] != 0) {
					return 0;
				}
			}
		}
	}
	else {
		if ((row - 1) >= 0) {
			for (i = col; i < col + size; i++) {
				if (map[row - 1][i] != 0) {
					return 0;
				}
			}
		}

		if ((row + 1) < MAP_SIZE) {
			for (i = col; i < col + size; i++) {
				if (map[row + 1][i] != 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE])
{
	int valid = 0;

	// while map is not valid
	while (valid < 4) {
		// reset map to zeros
		for (int r = 0; r < MAP_SIZE; r++) {
			for (int c = 0; c < MAP_SIZE; c++) {
				map[r][c] = 0;
			}
		}

		// add 4 random ships
		AddRandomShip(2, map);
		AddRandomShip(3, map);
		AddRandomShip(4, map);
		AddRandomShip(5, map);

		int size = 2;
		valid = 0;

		while (size <= 5 && valid < 4) {
			valid += (IsShipValid(size, map));
			size++;
		}
	}
}

int HighestCount(int values[MAP_SIZE][MAP_SIZE])
{
	int max = 0;

	for (int j = 0; j < MAP_SIZE; j++) {
		for (int i = 0; i < MAP_SIZE; i++) {
			int thirdDigit = values[j][i] % 10;
			int secondDigit = (values[j][i] / 10) % 10;
			int firstDigit = (values[j][i] / 100) % 10;
			int value = (firstDigit * 100) + (secondDigit * 10) + (thirdDigit);

			if (value > max) {
				max = value;
			}
		}
	}

	return max;
}

int HowManyHits(int size, int map[MAP_SIZE][MAP_SIZE], int shots[MAP_SIZE][MAP_SIZE])
{
	// find ship position
	int row = -1, col = -1;
	int facing = TopLeftPosition(size, &row, &col, map);

	// initialise hitCount
	int hitCount = 0;

	// iterate across ship length
	int i = -1;
	switch (facing) {
	case 1:
		for (i = col; i < col + size; i++) {
			if (shots[row][i] >= 1000) {
				hitCount++;
			}
		}
		break;
	case 2:
		for (i = row; i < row + size; i++) {
			if (shots[i][col] >= 1000) {
				hitCount++;
			}
		}
		break;
	}

	return hitCount;
}

void DestroyShip(int size, int map[MAP_SIZE][MAP_SIZE], int shots[MAP_SIZE][MAP_SIZE])
{
	int row = -1, col = -1;
	int facing = TopLeftPosition(size, &row, &col, map);
	int i = 0;

	switch (facing) {
	case 1:
		for (i = col; i < col + size; i++) {
			while (shots[row][i] < size * HIT) {
				shots[row][i] += HIT;
			}
		}
		break;
	case 2:
		for (i = row; i < row + size; i++) {
			while (shots[i][col] < size * HIT) {
				shots[i][col] += HIT;
			}
		}
		break;
	}
}

void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col)
{
	// initialise values
	int size = map[row][col];
	int count = HighestCount(shots);
	int shotIndexValue = shots[row][col];

	// add 1 to shot count 
	count++;

	// check shot is valid 
	if (shotIndexValue == 0) {
		// record shot 
		shotIndexValue += count;
		// check if shot is hit
		if (size > 0) {
			// add HIT value
			shotIndexValue += HIT;
		}
	}

	// set shot index element to shot value
	shots[row][col] = shotIndexValue;
	int hitShip = HowManyHits(size, map, shots);

	if (hitShip == size) {
		DestroyShip(size, map, shots);
	}
}

int MaxValue(int values[MAP_SIZE][MAP_SIZE])
{
	int max = 0;

	for (int j = 0; j < MAP_SIZE; j++) {
		for (int i = 0; i < MAP_SIZE; i++) {
			if (values[j][i] > max) {
				max = values[j][i];
			}
		}
	}

	return max;
}

int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE])
{
	int LargestHit = MaxValue(shots);
	int LargestShip = MaxValue(map);
	int DestroyedTiles = 0;
	int IsOver = 0;
	int nTilesNeeded = 0;

	// if largest shot value is between 1000 and 2000 no ships destroyed
	if (LargestHit > HIT && LargestHit < 2 * HIT) {
		// games is not over
		return IsOver; // = 0
	}
	else {
		// every ship
		for (int ship = 2; ship <= LargestShip; ship++) {
			// calc number of tiles to be destroyed
			nTilesNeeded += ship;

			// look for in array: ship x HIT < shots value < (ship + 1) * HIT 
			for (int j = 0; j < MAP_SIZE; j++) {
				for (int i = 0; i < MAP_SIZE; i++) {
					if (ship * HIT < shots[j][i] && shots[j][i] < (ship + 1) * HIT) {
						// add 1 to detroyed tiles
						DestroyedTiles++;
					}
				}
			}
		}
	}

	if (DestroyedTiles == nTilesNeeded) {
		IsOver = 1;
	}

	return IsOver;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go above
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Provided functions needed for playing the Battleships game:

// First, prototype declarations for the bots:
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);
void GetMoveReasonableBot(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col);

// Gets the input for one move from the human player (an alphabetic row and a numeric column)
// This function converts both inputs to numeric values
void GetMoveHuman(int* row, int* col, int player)
{
	char a = ' ';
	int b = -1;
	printf("Player %d: enter move [row/col]: ", player);
	while (!(a >= 'A' && a <= 'Z')) {
		scanf("%c", &a);
	}
	while (!(b >= 0 && b <= 25)) {
		scanf("%d", &b);
	}
	*row = (int)(a - 'A');
	*col = b;
}

// Takes two "shots" arrays as input (which are the visible parts of the game shown to players) and formats them into a string for printing
// The player currently to move is highlighted
void GetDisplayMapString(int shots1[MAP_SIZE][MAP_SIZE], int shots2[MAP_SIZE][MAP_SIZE], int player, char* boardString)
{
	int i, j;
	char numbers[10];
	// Intialise board string to be empty:
	boardString[0] = '\0';
	strcat(boardString, "  ");

	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i % 10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		sprintf(numbers, "%d", i % 10);
		strcat(boardString, numbers);
	}
	strcat(boardString, "\n  ");

	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "|");
	for (i = 0; i < MAP_SIZE; i++) {
		strcat(boardString, "-");
	}
	strcat(boardString, "\n");

	for (i = 0; i < MAP_SIZE; i++) {
		int len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len + 1] = '\0';
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots1[i][j] / HIT > 0) {
				if (shots1[i][j] / HIT == 1) {
					strcat(boardString, "X");
				}
				else {
					sprintf(numbers, "%d", shots1[i][j] / HIT);
					strcat(boardString, numbers);
				}
			}
			else if (shots1[i][j] > 0) {
				strcat(boardString, ".");
			}
			else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		for (j = 0; j < MAP_SIZE; j++) {
			if (shots2[i][j] / HIT > 0) {
				if (shots2[i][j] / HIT == 1) {
					strcat(boardString, "X");
				}
				else {
					sprintf(numbers, "%d", shots2[i][j] / HIT);
					strcat(boardString, numbers);
				}
			}
			else if (shots2[i][j] > 0) {
				strcat(boardString, ".");
			}
			else {
				strcat(boardString, " ");
			}
		}
		strcat(boardString, "|");
		len = strlen(boardString);
		boardString[len] = (char)('A' + i);
		boardString[len + 1] = '\0';
		strcat(boardString, "\n");
	}
	if (player == 1) {
		strcat(boardString, "  P1");
		for (i = 0; i < MAP_SIZE - 2; i++) {
			strcat(boardString, "*");
		}
	}
	else {
		for (i = 0; i < MAP_SIZE; i++) {
			strcat(boardString, " ");
		}
		strcat(boardString, "   P2");
		for (i = 0; i < MAP_SIZE - 2; i++) {
			strcat(boardString, "*");
		}
	}
	strcat(boardString, "\n");
}

// Plays one game of Battleships, beginning with the specified starting player
// Game type = 1 (human vs human) or 2 (human vs bot) or 3 (bot vs bot)
int PlayOneGame(int startingPlayer, int gameType)
{
	int row, col, player, gameOver;

	// String to display the boards
	char displayBoardString[(2 * MAP_SIZE + 5) * (MAP_SIZE + 5)];

	// The maps containing the locations of the ships
	int mapPlayer1[MAP_SIZE][MAP_SIZE] = { 0 };
	int mapPlayer2[MAP_SIZE][MAP_SIZE] = { 0 };

	// The locations of the shots
	int shotsPlayer1[MAP_SIZE][MAP_SIZE] = { 0 };
	int shotsPlayer2[MAP_SIZE][MAP_SIZE] = { 0 };

	player = startingPlayer;
	gameOver = 0;

	// Intialise GodBot
	GodBot bot(shotsPlayer2);

	// Create random maps for each player
	InitialiseRandomMap(mapPlayer1);
	InitialiseRandomMap(mapPlayer2);

	// Display the board if a human is playing
	if (gameType != 3 && gameType != 4) {
		GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
		printf("%s", displayBoardString);
	}

	// Process one move for the current player
	while (!gameOver) {
		if (gameType == 1) {
			GetMoveHuman(&row, &col, player);
			if (player == 1) {
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			}
			else {
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}
		else if (gameType == 2) {
			if (player == 1) {
				GetMoveHuman(&row, &col, player);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			}
			else {
				GetMoveBot1(shotsPlayer2, &row, &col);
				printf("Player 2 (bot) moves: %c%d\n", (char)(row + 'A'), col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}
		else if (gameType == 3) {
			if (player == 1) {
				GetMoveBot1(shotsPlayer1, &row, &col);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			}
			else {
				GetMoveBot2(shotsPlayer2, &row, &col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}
		else {
			if (player == 1) {
				GetMoveReasonableBot(shotsPlayer1, &row, &col);
				FireShot(shotsPlayer1, mapPlayer2, row, col);
				gameOver = CheckGameOver(shotsPlayer1, mapPlayer2);
			}
			else {
				bot.getMoveGodBot(shotsPlayer2, &row, &col);
				FireShot(shotsPlayer2, mapPlayer1, row, col);
				gameOver = CheckGameOver(shotsPlayer2, mapPlayer1);
			}
		}

		// Swap players
		if (!gameOver) {
			player = 3 - player;
		}
		if (gameType != 3 && gameType != 4) {
			GetDisplayMapString(shotsPlayer1, shotsPlayer2, player, displayBoardString);
			printf("%s", displayBoardString);
		}
	}
	return player;
}

// Play a Battleships tournament
// If the tournament is between a human and a bot, only one game is played
// If the tournament is between two bots, the number of games is requested
void PlayBattleships(void)
{
	int gameType, numberOfGames, result;
	int i, wins1, wins2, player;

	// Get play options:
	printf("Options:\n");
	printf(" [1] = Human vs. Human\n");
	printf(" [2] = Human vs. Bot1\n");
	printf(" [3] = Bot1 vs. Bot2\n");
	printf(" [4] = ResonableBot vs. GodBot\n");
	printf("Choose game type: ");
	scanf("%d", &gameType);
	numberOfGames = 1;
	result = 0;

	// If two bots are playing a tournament, let the user choose how many games
	if (gameType == 3 || gameType == 4) {
		printf("Number of games: ");
		scanf("%d", &numberOfGames);
	}

	// Set win counts to zero
	wins1 = 0;
	wins2 = 0;

	// Player 1 will always start the first game
	// If a tournament is played (between two bots), the starting player alternates
	player = 1;

	for (i = 0; i < numberOfGames; i++) {
		result = PlayOneGame(player, gameType);
		if (result == 1) {
			wins1++;
		}
		else {
			wins2++;
		}
		// Switch the starting player for the next game
		player = 3 - player;
	}

	// Show the outcome (of a single game or tournament)
	if (numberOfGames == 1) {
		printf("\nGame over! Congratulations! Winner is Player %d\n\n", result);
	}
	else {
		printf("\nTournament over! Games played = %d\nPlayer 1 wins = %d / Player 2 wins = %d\n\n", numberOfGames, wins1, wins2);
	}
}

// The main function for the Battleships program
int main(void)
{
	// Initialise the seed for the random number generator
	srand((unsigned int)time(NULL));

	printf("ENGGEN131 - C Project 2023\n");
	printf("                          ... presents ...\n");
	printf(" ___    __   _____ _____  _     ____  __   _     _   ___   __  \n");
	printf("| |_)  / /\\   | |   | |  | |   | |_  ( (` | |_| | | | |_) ( (` \n");
	printf("|_|_) /_/--\\  |_|   |_|  |_|__ |_|__ _)_) |_| | |_| |_|   _)_) \n");
	printf("\n                __/___            \n");
	printf("          _____/______|           \n");
	printf("  _______/_____\\_______\\_____     \n");
	printf("  \\              < < <       |    \n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\n");

	PlayBattleships();

	return 0;
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the two bots can be provided below
// The existing implementations are naive:
// - Bot 1 simply takes random shots (without checking for repeated shots)
// - Bot 2 also takes random shots (but makes a single test on each turn for a repeated shot)
*******************************************************************************
*******************************************************************************
******************************************************************************/

// Strategy for Bot 1
void GetMoveBot1(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col)
{
	int rand_row;
	int rand_col;

	rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
	rand_row = rand() % MAP_SIZE;
	rand_col = rand() % MAP_SIZE;

	*row = rand_row;
	*col = rand_col;
}

// Strategy for Bot 2
void GetMoveBot2(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col)
{
	int rand_row;
	int rand_col;

	rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
	rand_row = rand() % MAP_SIZE;
	rand_col = rand() % MAP_SIZE;

	// If the shot has been tried before, try one more time
	if (shots[rand_row][rand_col] > 0) {
		rand_row = rand() % MAP_SIZE;
		rand_col = rand() % MAP_SIZE;
	}

	*row = rand_row;
	*col = rand_col;
}

void GetMoveReasonableBot(int shots[MAP_SIZE][MAP_SIZE], int* row, int* col) {
	int rand_row;
	int rand_col;

	do {
		rand_row = shots[0][0]; // to prevent compiler warning (because otherwise "shots" array is not accessed)
		rand_row = rand() % MAP_SIZE;
		rand_col = rand() % MAP_SIZE;
	} while (shots[rand_row][rand_col] > 0);

	*row = rand_row;
	*col = rand_col;
}













