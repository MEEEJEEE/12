#define BOARDSTATUS_OK       1
#define BOARDSTATUS_NOK      0
#define N_BOARD 15

int board_initBoard(void);

int board_getBoardStatus(int pos);
int board_getShartPosition(void);

int board_stepShark(void);
int board_getBoardCoin(int pos);

void board_printBoardStatus(void);