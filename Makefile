include .env

CXX = g++
CXXFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib -Wl,-rpath,$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

ENGINE_DIR = engine
BUILD_DIR = build
ENGINE_BUILD_DIR = $(BUILD_DIR)/engine
TARGET = $(BUILD_DIR)/space-wars

GAME_SRCS = $(wildcard *.cpp)
ENGINE_SRCS = $(wildcard $(ENGINE_DIR)/*.cpp)

GAME_OBJS = $(GAME_SRCS:%.cpp=$(BUILD_DIR)/%.o)
ENGINE_OBJS = $(ENGINE_SRCS:$(ENGINE_DIR)/%.cpp=$(ENGINE_BUILD_DIR)/%.o)
OBJS = $(GAME_OBJS) $(ENGINE_OBJS)

VERT_SRCS = $(wildcard shaders/*.vert)
FRAG_SRCS = $(wildcard shaders/*.frag)
SPV_FILES = $(VERT_SRCS:%.vert=%.vert.spv) $(FRAG_SRCS:%.frag=%.frag.spv)


all: shaders $(TARGET)

shaders: $(SPV_FILES)

shaders/%.vert.spv: shaders/%.vert
	$(GLSLC) $< -o $@

shaders/%.frag.spv: shaders/%.frag
	$(GLSLC) $< -o $@

$(TARGET): $(OBJS) | dirs
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ENGINE_BUILD_DIR)/%.o: $(ENGINE_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	mkdir -p $(BUILD_DIR) $(ENGINE_BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
