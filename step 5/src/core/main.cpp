#include <chrono>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "gate.h"
#include "item.h"
#include "map.h"
#include "score.h"
#include "snake.h"

int main() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  start_color();
  use_default_colors();
  init_color(100, 500, 250, 0);
  init_color(101, 1000, 950, 800);
  init_color(102, 0, 0, 900);
  init_color(110, 200, 1000, 200);
  init_color(111, 1000, 200, 200);

  init_pair(1, 100, 101);
  init_pair(2, COLOR_WHITE, 102);
  init_pair(3, 101, 101);
  init_pair(4, 110, 101);
  init_pair(5, 111, 101);
  init_pair(6, COLOR_YELLOW, 101);
  init_pair(7, COLOR_CYAN, 101);
  init_pair(8, COLOR_MAGENTA, 101);
  init_pair(9, COLOR_WHITE, 101);
  init_pair(10, COLOR_GREEN, 101);
  bkgd(COLOR_PAIR(1));

  std::vector<std::string> Maps = {"src/map/map1.txt", "src/map/map2.txt",
                                   "src/map/map3.txt", "src/map/map4.txt"};

  Snake snake;
  MapManager map;
  ItemManager item;
  GateManager gate;
  ScoreManager score;

  map.printIntro();
  static auto lastMove = std::chrono::steady_clock::now();
  static auto lastShrink = std::chrono::steady_clock::now();
  auto gameStart = std::chrono::steady_clock::now();

  for (size_t stage = 0; stage < Maps.size(); stage++) {
    clear();
    map.load(Maps[stage]);
    map.setStage(stage + 1);

    gate.setMap(&map);
    gate.setMaxGates(1);
    gate.reset();

    snake.reset();
    snake.setMap(&map);
    snake.setGateManager(&gate);

    item.setMap(&map);
    item.setSnake(&snake);
    item.reset();

    score.reset(stage + 1);
    item.setScoreManager(&score);

    map.printStageIntro();
    int wallDir = rand() % 4;
    std::string dirStr;
    if (wallDir == 0)
      dirStr = "Up";
    else if (wallDir == 1)
      dirStr = "Down";
    else if (wallDir == 2)
      dirStr = "Left";
    else if (wallDir == 3)
      dirStr = "Right";
    score.setShrinkTime(lastShrink, dirStr);
    lastMove = std::chrono::steady_clock::now();
    lastShrink = std::chrono::steady_clock::now();
    auto prev = std::chrono::steady_clock::now();

    while (true) {
      if (score.isMissionComplete()) {
        break;
      }
      auto gameNow = std::chrono::steady_clock::now();

      double gameTime =
          std::chrono::duration<double>(gameNow - gameStart).count();
      score.setGameTime(gameTime);

      snake.handleInput();
      auto now = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsed = now - lastMove;
      std::chrono::duration<double> deltaReverse = now - prev;
      auto Wallnow = std::chrono::steady_clock::now();
      std::chrono::duration<double> shrinkElapsed = Wallnow - lastShrink;
      if (shrinkElapsed.count() >= 30.0 && !gate.isActive()) {
        lastShrink = Wallnow;
        map.WallRandom(&gate, wallDir);
        wallDir = rand() % 4;
        if (wallDir == 0)
          dirStr = "Up";
        else if (wallDir == 1)
          dirStr = "Down";
        else if (wallDir == 2)
          dirStr = "Left";
        else if (wallDir == 3)
          dirStr = "Right";
        score.setShrinkTime(lastShrink, dirStr);
        clear();
        map.print();
        snake.render();

        score.render(&gate, &snake);
        wallDir = rand() % 4;
        refresh();
      }
      if (snake.isKeyReversed()) {
        score.updateKeyReverse(deltaReverse.count());
      }

      if (elapsed.count() >= snake.getSpeedDelay()) {
        lastMove = now;
        clear();
        item.spawn(map.getStage());
        item.removeItem();
        if (!gate.isActive()) {
          gate.spawn();
          score.setGateSpawnTime(std::chrono::steady_clock::now());
        }

        if ((gate.isUsed() || gate.isExpired()) && !gate.isActive())
          gate.clear();

        if (!snake.move() || !item.getItem(snake)) {
          if (!map.printGameOver()) {
            endwin();
            return 0;
          }
          stage--;
          break;
        }
        score.updateSpeed(snake.getSpeedDelay(), 0.5);
        map.print();
        snake.render();
        score.render(&gate, &snake);
        refresh();
      }
      usleep(3000);
      prev = now;
    }
  }
  endwin();
  return 0;
}
