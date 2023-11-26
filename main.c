#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_CHARNAME		 200
#define N_PLAYER			 4
#define MAX_DIE				 6

#define PLAYERSTATUS_LIVE    0
#define PLAYERSTATUS_DIE	 1
#define PLAYERSTATUS_END     2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = { "LIVE", "DIE","END" };


//0. 시작
void opening(void)
{
	printf("=================================\n");
	printf("            SHARK GAME           \n");
	printf("=================================\n");
}
//주사위 굴리기

int rolldie(void)
{
	return rand() % MAX_DIE + 1;
}

//플레이어 위치 출력
void printPlayerPosition(int player)
{
	int i;
	for (i = 0; i< N_BOARD; i++)
	{
		printf("|");
		if (i == player_position[player])
			printf("%c", player_name[player][0]);
		else
		{
			if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
				printf("X");
			else
				printf(" ");
		}
	}
	printf("|\n");
}

//플레이어 상태 출력
void printPlayerStatus(void)
{
	int i;
	printf("player status ---\n");
	for (i=0; i < N_PLAYER;i++)
	{
		printf("%s : pos %i, coin %i, status : %s\n",
			player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
	}
	printf("--------------------------\n");
}

//보드 위에서 플레이어가 상어에게 잡힌 경우
void checkDie(void)
{
	int i;
	for (i = 0; i < N_PLAYER; i++)
		if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
			player_status[i] = PLAYERSTATUS_DIE;
}
// 게임 종료 여부 확인
int game_end(void) {
	int i;
	int flag_end = 1;

	// 모든 플레이어가 죽었는지 확인
	for (i = 0; i < N_PLAYER; i++) {
		if (player_status[i] == PLAYERSTATUS_LIVE) {
			flag_end = 0;
			break;
		}
	}

	return flag_end;
}

// 살아있는 플레이어 수 반환
int getAlivePlayer(void) {
	int i;
	int cnt = 0;
	for (i = 0; i < N_PLAYER; i++) {
		if (player_status[i] == PLAYERSTATUS_LIVE)
			cnt++;
	}

	return cnt;
}

// 승자 결정
int getWinner(void) {
	int i;
	int winner = 0;
	int max_coin = -1;

	for (i = 0; i < N_PLAYER; i++) {
		if (player_coin[i] > max_coin) {
			max_coin = player_coin[i];
			winner = i;
		}
	}
	return winner;
}
int main(int argc, char* argv[])
{
	int pos = 0;
	int i;
	int turn = 0;

	srand((unsigned)time(NULL));
//1-1.플레리어 초기화 및 이름 결정
for (i=0; i < N_PLAYER; i++)
{
	player_position[i] = 0;
	player_coin[i] = 0;
	player_status[i] = PLAYERSTATUS_LIVE;
	printf("Player %i's name: ", i);
}

opening();

//1-2.보드 초기화 & 플레이어 이름 결정
board_initBoard();

//2. 게임 루프 반복문 (플레이어 턴)
do {
	int step;
	int coinResult;
	char c;

	if (player_status[turn] != PLAYERSTATUS_LIVE)
	{
		turn = (turn + 1) % N_PLAYER;
		continue;
	}

	//2-1. 보드&플레이어의 상태 출력
	board_printBoardStatus();
	for (i = 0; i < N_PLAYER; i++)
		printPlayerPosition(i);
	printPlayerStatus();


	//2-2. 주사위 던지기
	printf("%s turn!! ", player_name[turn]);
	printf("press any key to roll a die!\n");
	scanf("%d", &c);
	fflush(stdin);
	step = rolldie();

//2-3. 이동
	player_position[turn] += step;
	if (player_position[turn] >= N_BOARD)
		player_position[turn] = N_BOARD - 1;

	if (player_position[turn] == N_BOARD - 1)
		player_status[turn] = PLAYERSTATUS_END;

//2-4. 동전 줍기
	coinResult = board_getBoardCoin(pos);
	player_coin[turn] += coinResult;

	//2-5. 다음턴
	turn = (turn + 1) % N_PLAYER; //wrap around

 //2-6. if if (조건:모든 플레이어가 한번씩 턴을 돔)
	if (turn == 0)
	{
		//상어 동작
		int shark_pos = board_stepShark();
		//printf();
		checkDie();
	}
} while (game_end() == 0);
//3. 정리 (승자 계산, 출력 등) 
int game_end(void)
{
	int i;
	int flag_end = 1;

	//if all the players are died?
	for (i = 0;i < N_PLAYER; i++)
	{
		if (player_status[i] == PLAYERSTATUS_LIVE)
		{
			flag_end = 0;
			break;
		}
	}

	return flag_end;
}

int getAlivePlayer(void)
{
	int i; 
	int cnt = 0; 
	for (i = 0; i < N_PLAYER; i++)
	{
		if (player_status[i] == PLAYERSTATUS_END)
			cnt++;
	}
	
	return cnt;
}
int getWinner(void)
{
	int i; 
	int winner = 0; 
	int max_coin = -1;

	for (i = 0; i < N_PLAYER; i++)
	{
		if (player_coin[i] > max_coin)
		{
			max_coin = player_coin[i];
			winner = i;
		}
	}
	return winner;
}
}