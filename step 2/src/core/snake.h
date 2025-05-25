#pragma once
#include <vector>

class MapManager;

class Snake {
private:
  std::vector<std::pair<int, int>> body;
  int dir;     // 현재 방향
  int nextDir; // 다음 틱 방향
  const int dx[4] = {0, 0, -1, 1};
  const int dy[4] = {-1, 1, 0, 0};
  MapManager *map = nullptr;

public:
  Snake();

  void reset(); // 기본 세팅
  void render() const;
  bool handleInput();         // 입력만 저장
  bool move();                // 실제 이동
  void setMap(MapManager *m); // 외부에서 map 연결
  int getLength() const;
};
