# 컴파일러와 옵션
CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lncurses

# 디렉토리
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
BUILD_DIR = $(SRC_DIR)/build

# 소스 파일
SRCS = $(CORE_DIR)/main.cpp \
		$(CORE_DIR)/snake.cpp \
		$(CORE_DIR)/item.cpp \
       $(CORE_DIR)/map.cpp

# 오브젝트 파일은 build 폴더에 저장
OBJS = $(patsubst $(CORE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# 실행 파일 이름
TARGET = snake

# 기본 규칙: 자동 clean → 빌드 → 실행
all: clean $(TARGET)
	@echo "=== 프로그램 실행 ==="
	@./$(TARGET)

# 최종 실행 파일 빌드
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# .cpp → .o 빌드 규칙
$(BUILD_DIR)/%.o: $(CORE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 정리
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
