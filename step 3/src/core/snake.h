#pragma once
#include <vector>
using namespace std;
class MapManager;

class Snake {
private:
  vector<std::pair<int, int>> body;
  int dir;     // 현재 방향
  int nextDir; // 다음 틱 방향
  const int dx[4] = {0, 0, -1, 1};
  const int dy[4] = {-1, 1, 0, 0};
  MapManager *map = nullptr;
  double speedDelay = 0.2; // 초기 속도 (초)
  bool reverseKey = false; // 키 반전 상태
public:
  Snake();

  void reset(); // 기본 세팅
  void render() const;
  void handleInput();         // 입력만 저장
  bool move();                // 실제 이동
  void setMap(MapManager *m); // 외부에서 map 연결
  int getLength() const;
  pair<int, int> getHead() const;
  void grow();
  bool shrink();
  void applySpeedUp();   // 속도 빠르게
  void applySpeedDown(); // 속도 느리게
  void toggleReverse();  // 키 반전 토글
  double getSpeedDelay() const;
  bool isKeyReversed() const;
  bool isOnBody(int y, int x) const;
};
