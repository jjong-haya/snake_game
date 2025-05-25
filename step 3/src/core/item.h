#pragma once
#include <ctime>
#include <vector>

using namespace std;

class MapManager;
class Snake;

class Item {
private:
  int y, x, type;
  time_t spawnTime;

public:
  Item(int y, int x, int type);
  int getY() const;
  int getX() const;
  int getType() const;
  time_t getTime() const;
};

class ItemManager {
private:
  vector<Item> items;
  MapManager *map;
  Snake *snake;

public:
  ItemManager();
  void setMap(MapManager *m);
  void setSnake(Snake *s);
  void spawn(int stage);      // 아이템 생성
  void removeItem();          // 시간 지난 아이템 제거
  bool getItem(Snake &snake); // snake 머리랑 충돌 확인
  void reset();
};
