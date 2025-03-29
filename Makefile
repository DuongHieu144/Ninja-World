# Compiler
CXX = g++

# Cờ biên dịch (thêm -g để debug)
CXXFLAGS = -g

# Thư mục chứa file .h và thư viện SDL
INCLUDE = -I src/include
LIBS = -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Các file nguồn
SRCS = main.cpp graphic.cpp character.cpp map.cpp enemy.cpp villagechief.cpp item.cpp
OBJS = $(SRCS:.cpp=.o)

# Tên file output
TARGET = game.exe

# Rule build
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

# Rule clean (xóa file .o và binary)
clean:
	rm -f $(OBJS) $(TARGET)
