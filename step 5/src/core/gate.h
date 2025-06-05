#pragma once
#include <chrono>
#include <ctime>
#include <utility>
#include <vector>

class MapManager;

class GateManager {
private:
  std::pair<int, int> gateA;
  std::pair<int, int> gateB;
  bool active;
  bool used;
  std::chrono::steady_clock::time_point spawnTime;
  MapManager *map;
  int maxGates;

public:
  GateManager();
  std::pair<int, int> getGateA() const { return gateA; }
  std::pair<int, int> getGateB() const { return gateB; }
  void setGateA(std::pair<int, int> pos);
  void setGateB(std::pair<int, int> pos);
  void setMap(MapManager *m);
  void setMaxGates(int count);
  void spawn();
  void clear();
  void reset();
  bool isGate(int y, int x) const;
  std::pair<int, int> getOtherGate(int y, int x) const;
  int getExitDirection(int gateY, int gateX, int entryDir) const;
  bool isActive() const;
  void setActive(bool a);
  bool isUsed() const;
  void setUsed(bool u);
  bool isExpired() const;
  std::chrono::steady_clock::time_point getSpawnClock() const;
};
