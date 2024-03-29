#include "console.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#define BOARD_SIZE 15
#define MOVE_DELAY 24
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"
using namespace console;

void map(){
  draw(0,0,"┏");
  for (int x1=1; x1<BOARD_SIZE-1; x1++)
    draw(x1,0,"━");
  draw(BOARD_SIZE-1,0,"┓");
  
  for(int y1=1; y1<BOARD_SIZE-1; y1++){
    draw(0,y1,"┃");
    draw(BOARD_SIZE-1,y1,"┃");
  }

  draw(0,BOARD_SIZE-1,"┗");
  for (int x1=1; x1<BOARD_SIZE-1; x1++)
    draw(x1,BOARD_SIZE-1,"━");
  draw(BOARD_SIZE-1,BOARD_SIZE-1,"┛");
}

int main(){
  srand((unsigned int)time(NULL));
  int count=0,score=0,timer=0,xa=rand()%(BOARD_SIZE-2)+1,ya=rand()%(BOARD_SIZE-2)+1,a=0;
  std::string last="r";
  int snake_place[2][(BOARD_SIZE-2)*(BOARD_SIZE-2)];//몸톰의 좌표 저장
  snake_place[0][0]=BOARD_SIZE/2;//[0][0]은 머리의 y좌표
  snake_place[1][0]=BOARD_SIZE/2;//[1][0]은 머리의 x좌표

  init();

  while(true){
    clear();
    map();
    
    if (key(K_LEFT))
      last="l";
    if (key(K_RIGHT))
      last="r";
    if (key(K_UP))
      last="u";
    if (key(K_DOWN))
      last="d";
    if (key(K_ESC)) 
      break;
    if(key(K_OTHER))
      last=last;
    if(key(K_NONE))
      last=last;

    for(int i=count-1; i>=0; i--){//이전 몸의 좌표를 다음 몸에 넣기
      snake_place[0][i+1]=snake_place[0][i];
      snake_place[1][i+1]=snake_place[1][i];
    }

    switch(last[0]){//머리 좌표 바꾸기
      case 'l': snake_place[1][0]--; break;
      case 'r': snake_place[1][0]++; break;
      case 'u': snake_place[0][0]--; break;
      case 'd': snake_place[0][0]++; break;
    }

    draw(snake_place[1][0],snake_place[0][0],"■");
    for(int i=1; i<=count; i++)//뱀 출력
      draw(snake_place[1][i],snake_place[0][i],"■");

    for(int i=0,k=0; i<=count; i++){//죽었을때1
      for(int j=0; j<=count; j++){
        if(i==j)
          continue;
        if(count!=0 && (snake_place[0][i]==snake_place[0][j]) && (snake_place[1][i]==snake_place[1][j])){
          clear();
          map();
          draw(BOARD_SIZE/2-4,BOARD_SIZE/2,"YOU LOSE!");
          if(BOARD_SIZE>=20)
            draw(BOARD_SIZE/2-9,BOARD_SIZE/2+1,"Try again? (Enter)");
          else
            draw(0,BOARD_SIZE/2+1,"Try again? (Enter)");
          if (key(K_ESC)) {
            k++;
            break;
          }
          if (key(K_ENTER)) {
            xa=rand()%(BOARD_SIZE-2)+1;
            ya=rand()%(BOARD_SIZE-2)+1;
            for(int i=0; i<=count; i++){
              snake_place[0][i]=0;
              snake_place[1][i]=0;
            }
            snake_place[0][0]=BOARD_SIZE/2;
            snake_place[1][0]=BOARD_SIZE/2;
            count=0;
            score=0;
            a=0;
            last="r";
            init();
            continue;
          }
        }
      }
      if(k==1){
        a=1;
        break;
      }
    }
    if(a==1)
      break;
    //죽었을때2
    if(snake_place[1][0]<=0 || snake_place[1][0]>=BOARD_SIZE-1 || snake_place[0][0]<=0 || snake_place[0][0]>=BOARD_SIZE-1){
      clear();
      map();
      draw(BOARD_SIZE/2-4,BOARD_SIZE/2,"YOU LOSE!");
      if(BOARD_SIZE>=20)
        draw(BOARD_SIZE/2-9,BOARD_SIZE/2+1,"Try again? (Enter)");
      else
        draw(0,BOARD_SIZE/2+1,"Try again? (Enter)");
      if (key(K_ESC)) 
        break;
      if (key(K_ENTER)) {
        xa=rand()%(BOARD_SIZE-2)+1;
        ya=rand()%(BOARD_SIZE-2)+1;
        for(int i=0; i<=count; i++){
          snake_place[0][i]=0;
          snake_place[1][i]=0;
        }
        snake_place[0][0]=BOARD_SIZE/2;
        snake_place[1][0]=BOARD_SIZE/2;
        count=0;
        score=0;
        last="r";
        a=0;
        init();
        continue;
      }
    }

    if(snake_place[1][0]==xa && snake_place[0][0]==ya){//사과 먹을때
      count++;
      score+=10;
      xa=rand()%(BOARD_SIZE-2)+1;
      ya=rand()%(BOARD_SIZE-2)+1;
      
      for(int i=0; i<count; i++){//사과와 뱀의 위치를 다르게
        if(xa==snake_place[1][i] && ya==snake_place[0][i]){
          xa=rand()%(BOARD_SIZE-2)+1;
          ya=rand()%(BOARD_SIZE-2)+1;
          i=-1;
        }
      } 
    }

    draw(xa,ya,APPLE_STRING);//사과

    if(count+1==(BOARD_SIZE-2)*(BOARD_SIZE-2)){//다 채웠을 때
      clear();
      map();
      draw(BOARD_SIZE/2-3,BOARD_SIZE/2,"YOU WIN!");
      if(BOARD_SIZE>=20)
        draw(BOARD_SIZE/2-9,BOARD_SIZE/2+1,"Try again? (Enter)");
      else
        draw(0,BOARD_SIZE/2+1,"Try again? (Enter)");
      if (key(K_ESC)) 
        break;
      if (key(K_ENTER)) {
        xa=rand()%(BOARD_SIZE-2)+1;
        ya=rand()%(BOARD_SIZE-2)+1;
        for(int i=0; i<=count; i++){
          snake_place[0][i]=0;
          snake_place[1][i]=0;
        }
        snake_place[0][0]=BOARD_SIZE/2;
        snake_place[1][0]=BOARD_SIZE/2;
        count=0;
        score=0;
        last="r";
        a=0;
        init();
        continue;
      }
    }
    
    if(score<100)
      draw(BOARD_SIZE/2-5,BOARD_SIZE,"Score : "+std::to_string(score));
    else
      draw(BOARD_SIZE/2-6,BOARD_SIZE,"Score : "+std::to_string(score));
    
    for(int i=0; i<MOVE_DELAY; i++)
      wait();
  }
  return 0;
}