#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 22
#define NOTHING 0
#define QUIT 'q'
#define NUM_OF_SHAPE 7
#define NUM_OF_ROTATE 4
#define BLOCK_HEIGHT 4
#define BLOCK_WIDTH 4
#define BLOCK_NUM 2

// menu number
#define MENU_PLAY '1'
#define MENU_EXIT '4'

// 사용자 이름의 길이
#define NAMELEN 16

#define CHILDREN_MAX 36

typedef struct _RecNode {
  int lv, score;
  char (*f)[WIDTH];
  struct _RecNode *c[CHILDREN_MAX];
} RecNode;

/* [blockShapeID][# of rotate][][]*/
const char block[NUM_OF_SHAPE][NUM_OF_ROTATE][BLOCK_HEIGHT][BLOCK_WIDTH] = {
    { /*[0][][][]					▩▩▩▩*/
     {/*[][0][][]*/
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     {/*[][1][][]*/
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     {/*[][3][][]*/
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0}}},
    { /*[1][][][];					  ▩▩▩*/
     {/*[][0][][]				      ▩*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 1},
      {0, 0, 0, 1}},
     {/*[][1][][]*/
      {0, 0, 0, 0},
      {0, 0, 1, 1},
      {0, 0, 1, 0},
      {0, 0, 1, 0}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 1},
      {0, 0, 0, 0}},
     {/*[][3][][]*/
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 1, 1, 0}}},
    { /*[2][][][];					  ▩▩▩*/
     {/*[][0][][]				  ▩*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 1},
      {0, 1, 0, 0}},
     {/*[][1][][]*/
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 1}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {0, 0, 0, 1},
      {0, 1, 1, 1},
      {0, 0, 0, 0}},
     {/*[][3][][]*/
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0}}},
    { /*[3][][][];					  ▩▩▩*/
     {/*[][0][][]				    ▩*/
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0}},
     {/*[][1][][]*/
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 1, 0, 0}},
     {/*[][3][][]*/
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0}}},
    { /*[4][][][];					  ▩▩*/
     {/*[][0][][]				  ▩▩*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0}},
     {/*[][1][][]*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0}},
     {/*[][3][][]*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0}}},
    { /*[5][][][];					  ▩▩*/
     {/*[][0][][]				▩▩*/
      {0, 0, 0, 0},
      {0, 0, 1, 1},
      {0, 1, 1, 0},
      {0, 0, 0, 0}},
     {/*[][1][][]*/
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {0, 0, 1, 1},
      {0, 1, 1, 0},
      {0, 0, 0, 0}},
     {/*[][3][][]*/
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0}}},
    { /*[6][][][];					▩▩*/
     {/*[][0][][]				  ▩▩*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 1}},
     {/*[][1][][]*/
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0}},
     {/*[][2][][]*/
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 1}},
     {/*[][3][][]*/
      {0, 0, 0, 0},
      {0, 0, 1, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 0}}}};

char field[HEIGHT][WIDTH]; /* 테트리스의 메인 게임 화면 */
int nextBlock[BLOCK_NUM]; /* 현재 블럭의 ID와 다음 블럭의 ID들을 저장; [0]: 현재
                             블럭; [1]: 다음 블럭 */
int blockRotate, blockY,
    blockX;       /* 현재 블럭의 회전, 블럭의 Y 좌표, 블럭의 X 좌표*/
int score;        /* 점수가 저장*/
int gameOver = 0; /* 게임이 종료되면 1로 setting된다.*/
int timed_out;
int recommendR, recommendY,
    recommedX;  // 추천 블럭 배치 정보. 차례대로 회전, Y 좌표, X 좌표
RecNode *recRoot;

/***********************************************************
 *	테트리스의 모든  global 변수를 초기화 해준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void InitTetris();

/***********************************************************
 *	테트리스의 모든  interface를 그려준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void DrawOutline();

/***********************************************************
 *	테트리스와 관련된 키입력을 받는다.
 *	input	: none
 *	return	: (int) 입력받은 command
 *		  KEY_DOWN : 방향키 아래
 *		  KEY_UP   : 방향키 위
 *		  KEY_RIGHT: 방향키 오른쪽
 *		  KEY_LEFT : 방향키 왼쪽
 *		  ' '	   : Space bar
 *		  'q'/'Q'  : quit
 ***********************************************************/
int GetCommand();

/***********************************************************
 *	GetCommand로 입력받은 command에 대한 동작을 수행한다.
 *	input	: (int) GetCommand로 받은 command
 *	return	: (int) quit에 대한 입력을 받을 경우 0,
 * 		  그외의 경우 1을 return한다.
 ***********************************************************/
int ProcessCommand(int command);

/***********************************************************
 *	블럭이 일정 시간(1초)마다 내려가도록 호출되는 함수
 *	더이상 내릴수 없을 경우,
 *		블럭을 field에 합친다.
 *		완전이 채워진 line을 지운다.
 *		next block을 current block으로 바꿔주고
 *		block의 좌표를 초기화 한다.
 *		다음 블럭을 화면에 그리고 갱신된 score를
 *		화면에 display한다.
 *	input	: (int) sig
 *	return	: none
 ***********************************************************/
void BlockDown(int sig);

/***********************************************************
 *	입력된 움직임이 가능한지를 판단해주는 함수.
 *	input	: (char[][]) 블럭의 움직임을 확인할 필드
 *		  (int) 현재 블럭의 모양 ID
 *		  (int) 블럭의 회전 횟수
 *		  (int) 블럭의 Y좌표
 *		  (int) 블럭의 X좌표
 *	return	: (int) 입력에 대한 블럭 움직임이 가능하면 1
 *		  가능하지 않으면 0을 return 한다.
 ***********************************************************/
int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate,
                int blockY, int blockX);

/***********************************************************
 *	테트리스에서 command에 의해 바뀐 부분만 다시 그려준다.
 *	input	: (char[][]) command의해 바뀐 블럭을 확인할 필드
 *		  (int) 바뀌기 전 모양을 알기 위해 command를 입력으로 받는다.
 *		  (int) 현재 블럭의 모양 ID
 *		  (int) 블럭의 회전 횟수
 *		  (int) 블럭의 Y좌표
 *		  (int) 블럭의 X좌표
 *	return	: none
 ***********************************************************/
void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock,
                int blockRotate, int blockY, int blockX);

/***********************************************************
 *	테트리스의 블럭이 쌓이는 field를 그려준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void DrawField();

/***********************************************************
 *	떨어지는 블럭을 field에 더해준다.
 *	input	: (char[][]) 블럭을 쌓을 필드
 *		  (int) 현재 블럭의 모양 ID
 *		  (int) 블럭의 회전 횟수
 *		  (int) 블럭의 Y좌표
 *		  (int) 블럭의 X좌표
 *	return	: none
 ***********************************************************/
void AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate,
                     int blockY, int blockX);

/***********************************************************
 *	완전히 채워진 Line을 삭제하고 점수를 매겨준다.
 *	input	: (char[][]) 완전히 채워진 line을 확인할 필드
 *	return	: (int) 삭제된 라인의 갯수에 대한 점수
 ***********************************************************/
int DeleteLine(char f[HEIGHT][WIDTH]);

/***********************************************************
 *	커서의 위치를 입력된 x, y의 위치로 옮겨주는 역할을 한다.
 *	input	: (int) 커서의 이동할 y 좌표
 *		  (int) 커서의 이동할 x 좌표
 *	return	: none
 ***********************************************************/
void gotoyx(int y, int x);

/***********************************************************
 *	테트리스의 화면 오른쪽상단에 다음 나올 블럭을 그려준다..
 *	input	: (int*) 블럭의 모양에 대한 ID 배열
 *	return	: none
 ***********************************************************/
void DrawNextBlock(int *nextBlock);

/***********************************************************
 *	테트리스의 화면 오른쪽 하단에 Score를 출력한다.
 *	input	: (int) 출력할 점수
 *	return	: none
 ***********************************************************/
void PrintScore(int score);

/***********************************************************
 *	해당 좌표(y,x)에 원하는 크기(height,width)의 box를 그린다.
 *	input	: (int) 그리고자 하는 박스의 왼쪽 상단모서리의 y 좌표
 *		  (int) 왼쪽 상단 모서리의 x 좌표
 *		  (int) 박스의 높이
 *		  (int) 박스의 넓이
 *	return	: none
 ***********************************************************/
void DrawBox(int y, int x, int height, int width);

/***********************************************************
 *	해당 좌표(y,x)에 원하는 모양의 블록을 그린다.
 *	input	: (int) 그리고자 하는 박스의 왼쪽 상단모서리의 y 좌표
 *		  (int) 왼쪽 상단 모서리의 x 좌표
 *		  (int) 블록의 모양
 *		  (int) 블록의 회전 횟수
 *		  (char) 블록을 그릴 패턴 모양
 *	return	: none
 ***********************************************************/
void DrawBlock(int y, int x, int blockID, int blockRotate, char tile);

/***********************************************************
 *	블록이 떨어질 위치를 미리 보여준다.
 *	input	: (int) 그림자를 보여줄 블록의 왼쪽 상단모서리의 y 좌표
 *		  (int) 왼쪽 상단 모서리의 x 좌표
 *		  (int) 블록의 모양
 *		  (int) 블록의 회전 횟수
 *	return	: none
 ***********************************************************/
void DrawShadow(int y, int x, int blockID, int blockRotate);

/***********************************************************
 *	테트리스 게임을 시작한다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void play();

/***********************************************************
 *	메뉴를 보여준다.
 *	input	: none
 *	return	: 사용자가 입력한 메뉴 번호
 ***********************************************************/
char menu();

/***********************************************************
 *	rank file로부터 랭킹 정보를 읽어와 랭킹 목록을 구성한다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void createRankList();

/***********************************************************
 *	화면에 랭킹 기록들을 보여준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void rank();

/***********************************************************
 *	rank file을 생성한다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void writeRankFile();

/***********************************************************
 *	새로운 랭킹 정보를 추가한다.
 *	input	: (int) 새로운 랭킹의 점수
 *	return	: none
 ***********************************************************/
void newRank(int score);

/***********************************************************
 *	추천 블럭 배치를 구한다.
 *	input	: (RecNode*) 추천 트리의 루트
 *	return	: (int) 추천 블럭 배치를 따를 때 얻어지는 예상 스코어
 ***********************************************************/
int recommend(RecNode *root);

/***********************************************************
 *	추천 기능에 따라 블럭을 배치하여 진행하는 게임을 시작한다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void recommendedPlay();

#endif
