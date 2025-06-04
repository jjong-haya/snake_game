#pragma once
#include <vector>

class MapManager;
class GateManager;

class Snake {
private:
  std::vector<std::pair<int, int>> body;
  int dir;
  int nextDir;
  const int dx[4] = {0, 0, -1, 1};
  const int dy[4] = {-1, 1, 0, 0};
  MapManager *map = nullptr;
  GateManager *gateManager = nullptr;
  double speedDelay = 0.2;
  bool reverseKey = false;

public:
  Snake();
  void reset();
  void render() const;
  void handleInput();
  bool move();
  void setMap(MapManager *m);
  void setGateManager(GateManager *g);
  int getLength() const;
  std::pair<int, int> getHead() const;
  bool isOnBody(int y, int x) const;
  void grow();
  bool shrink();
  void applySpeedUp();
  void applySpeedDown();
  void toggleReverse();
  bool isKeyReversed() const;
  double getSpeedDelay() const;
  void teleportGate();
};
