#pragma once
#include <ctime>
#include <vector>

class MapManager;
class Snake;

enum class ItemType {
  Growth = 5,
  Poison = 6,
  Random = 8,
  SpeedUp = 9,
  SpeedDown = 10,
  RandomGate = 11,
  KeyReverse = 12
};

class Item {
private:
  int y, x;
  ItemType type;
  time_t spawnTime;

public:
  Item(int y, int x, ItemType type);
  int getY() const;
  int getX() const;
  ItemType getType() const;
  time_t getTime() const;
};

class ItemManager {
private:
  std::vector<Item> items;
  MapManager *map;
  Snake *snake;

public:
  ItemManager();
  void setMap(MapManager *m);
  void setSnake(Snake *s);
  void spawn(int stage);
  void removeItem();
  bool getItem(Snake &snake);
  void reset();
};
