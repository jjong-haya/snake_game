#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "item.h"
#include "map.h"
#include "snake.h"

using namespace std;

int main() {
  initscr();             // 창 만들기
  noecho();              // 입력 키 출력 무시
  curs_set(0);           // 커서 무시
  keypad(stdscr, TRUE);  // 특수키 사용(getch() 대용)
  nodelay(stdscr, TRUE); // 비동기화 게임

  //========= 색깔 기능 ====================
  start_color();
  use_default_colors();
  init_color(100, 500, 250, 0);    // 베이지색
  init_color(101, 1000, 950, 800); // 갈색
  init_color(102, 0, 0, 900);      // 하늘색
  init_pair(1, 100, 101);          // 베이지 배경 갈색 글자
  init_pair(2, COLOR_WHITE, 102);  // 하늘색 배경 흰색 글자
  init_pair(3, 101, 100);          // 1번 반대
  bkgd(COLOR_PAIR(1));
  //========================================
  // 맵 파일 불러오기
  vector<string> Maps = {"src/map/map1.txt", "src/map/map2.txt",
                         "src/map/map3.txt", "src/map/map4.txt"};
  Snake snake;
  MapManager map; // map 선언
  ItemManager item;

  map.printIntro(); // 인트로 출력

  for (size_t stage = 0; stage < Maps.size(); stage++) {
    //===============초기 설정=============
    clear();                 // 초기화
    map.load(Maps[stage]);   // 맵 불러오기
    map.setStage(stage + 1); // 스테이지 세팅
    snake.reset();           // 뱀 초기화
    snake.setMap(&map);      // 뱀과 맵 연결
    item.setMap(&map);       // 아이템과 맵 연결
    item.setSnake(&snake);
    map.printStageIntro();
    //==============게임 시작===============
    while (true) {
      clear(); // 초기화
      item.spawn(map.getStage());
      item.removeItem();
      map.print(); // 출력
      snake.render();
      refresh();
      snake.handleInput();
      if (!snake.move() || !item.getItem(snake)) {
        if (!map.printGameOver()) { // q 누르면 종료
          endwin();
          return 0;
        }
        stage--; // r 누르면 현재 stage 다시
        break;
      }
      usleep(snake.getSpeedDelay() * 1000000);
    }
  }
  endwin();
  return 0;
}
