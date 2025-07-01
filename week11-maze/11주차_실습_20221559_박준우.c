#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL_NONE 0b0000
#define WALL_UP 0b1000
#define WALL_DOWN 0b0100
#define WALL_LEFT 0b0010
#define WALL_RIGHT 0b0001
#define WALL_ALL 0b1111

#define MAZE_VERT "|"
#define MAZE_HORIZ "-"
#define MAZE_CORNER "+"
#define MAZE_BLANK " "

#define MAX(X, Y) ((X > Y)? X : Y)
#define MIN(X, Y) ((X < Y)? X : Y)

int get0or1(){
  return rand() & 1;
}

void createMaze(int i, int N, int M, int **maze, int **group){
  // M - 1개의 벽에 대해
  for(int j = 0; j < M - 1; j++){
    // 그룹이 같으면 뚫으면 안된다.
    if(group[i][j] == group[i][j+1]) continue;

    // 확률에 의해
    if(get0or1()){

      int changing = MAX(group[i][j], group[i][j+1]);
      int changeto = MIN(group[i][j], group[i][j+1]);
      // 벽을 뚫을것인데, 뒤에서 그룹이 같은 곳 찾기
      for(int k = 0; k < M; k++){
        if(group[i][k] != changing) continue;
        // 지금 바뀔 것과 같은 그룹이면 다 바꿔라.
        group[i][k] = changeto;
      }

      maze[i][j] ^= WALL_RIGHT;
      maze[i][j+1] ^= WALL_LEFT;
    }
  }

  for(int j = 0; j < M; j++){
    // 이미 한 그룹인 경우 스킵
    if(group[i][j] < 0) continue;

    // 현재 그룹 저장, 이 그룹의 개수 셀 변수, 필수로 뚫고 내려갈 순서
    int curr = group[i][j], count = 1, mand;

    // j보다 오른쪽의 같은 그룹의 개수 세기
    for(int k = j + 1; k < M; k++){
      if(group[i][k] == curr) count++;
    }

    // 그룹 개수 내의 필수 순서,,
    mand = rand() % count;

    // printf("i : %d, j : %d, mand : %d \n", i, j, mand);

    // j부터, cnt는 몇번째의 같은 그룹인지 보기.
    for(int k = j, cnt = 0; k < M; k++){
      // 같은 그룹이 아니면 스킵
      if(group[i][k] != curr) continue;
      
      // 의무 순서거나, 랜덤하게 부시기로 결정했으면, 그룹을 확장하고, 벽을 허물자
      if(cnt++ == mand || get0or1()){
        group[i + 1][k] = curr;

        maze[i][k] ^= WALL_DOWN;
        maze[i + 1][k] ^= WALL_UP;
      }

      // 이미 끝난 곳 체크.
      group[i][k] *= -1;
    }
  }
}

void printMaze(int N, int M, int **maze ,int **group){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      printf(MAZE_CORNER);
      printf((maze[i][j] & WALL_UP)? MAZE_HORIZ : MAZE_BLANK);
      if(j == M - 1) printf(MAZE_CORNER);
    }
    printf("\n");
    for(int j = 0; j < M; j++){
      printf((maze[i][j] & WALL_LEFT)? MAZE_VERT : MAZE_BLANK);
      printf(MAZE_BLANK);
      if(j == M - 1) printf(MAZE_VERT);
    }
    printf("\n");

    if(i == N - 1){
      for(int j = 0 ; j < M; j++){
        printf(MAZE_CORNER);
        printf(MAZE_HORIZ);

        if(j == M - 1) printf(MAZE_CORNER);
      }
    }
  }
}

int main(void){
  // freopen("input.txt", "r", stdin);
  freopen("output.maz", "w", stdout);
  srand(time(NULL));

  int N, M, i, j, cnt = 0;

  scanf("%d %d", &N, &M);

  int **maze, **cell_group;

  maze = (int**)malloc(sizeof(int*) * N);
  for(i = 0; i < N; i++){
    maze[i] = (int*) malloc(sizeof(int) * M);

    for(j = 0; j < M; j++){
      maze[i][j] = WALL_ALL;
    }
  }

  cell_group = (int**)malloc(sizeof(int*) * N);
  for(i = 0; i < N; i++){
    cell_group[i] = (int*) malloc(sizeof(int) * M);

    for(j = 0; j < M; j++){
      cell_group[i][j] = 0;
    }
  }


  // 윗 줄들 만들기, 아래층 뚫기까지 포함
  for(i = 0; i < N - 1; i++) {
    for(j = 0; j < M; j++){
      //이미 그룹이 있으면 패스
      if(cell_group[i][j] > 0) continue;
      // 아니면 새로 생성해주기.
      cell_group[i][j] = ++cnt;
    }

    //i 번 줄 넣기.
    createMaze(i, N, M, maze, cell_group);
  }

  // 마지막 줄
  i = N - 1;
  for(j = 0; j < M; j++){
    if(cell_group[i][j] > 0) continue;

    cell_group[i][j] = ++cnt;
  }

  // 맨 마지막 줄의 M - 1개의 벽 뚫기
  for(j = 0; j < M - 1; j++){
    // 이 벽 좌우로 그룹이 같으면 스킵
    if(cell_group[i][j] == cell_group[i][j + 1]) continue;

    // 이 벽 좌우로 그룹이 같지 않아 부수려 함,,
    // 바뀔 그룹은 미리 바꿔주고, 그룹 바꾼 후 벽 허물기

    int changing = MAX(cell_group[i][j], cell_group[i][j+1]);
    int changeto = MIN(cell_group[i][j], cell_group[i][j+1]);
    for(int k = 0; k < M; k++){
      if (cell_group[i][k] == changing)
        cell_group[i][k] = changeto;
    }

    maze[i][j] ^= WALL_RIGHT;
    maze[i][j+1] ^= WALL_LEFT;
  }

  printMaze(N, M, maze, cell_group);

  // printf("\n");
  // for(i = 0; i < N; i++){
  //   for(j = 0; j < M; j++){
  //     printf("%d ", cell_group[i][j]);
  //   }
  //   printf("\n");
  // }

  // for(i = 0; i < N; i++){
  //   for(j = 0; j < M; j++){
  //     printf("%d%d%d%d ", !!(maze[i][j] & WALL_UP), !!(maze[i][j] & WALL_DOWN), !!(maze[i][j] & WALL_LEFT), !!(maze[i][j] & WALL_RIGHT));
  //   }
  //   printf("\n");
  // }


  // free dynamically allocated memory
  for(i = 0; i < N; i++){
    free(maze[i]);
  }
  free(maze);

  for(i = 0; i < N; i++){
    free(cell_group[i]);
  }
  free(cell_group);

  return 0;
}