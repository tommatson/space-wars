include .env

CXX = g++
CXXFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib -Wl,-rpath,$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

ENGINE_DIR = engine
RENDERER_DIR = $(ENGINE_DIR)/renderer
SYSTEMS_DIR = $(RENDERER_DIR)/systems
BUILD_DIR = build
RENDERER_BUILD_DIR = $(BUILD_DIR)/engine/renderer
SYSTEMS_BUILD_DIR = $(BUILD_DIR)/engine/renderer/systems
TARGET = $(BUILD_DIR)/space-wars

GAME_SRCS = $(wildcard *.cpp)
RENDERER_SRCS = $(wildcard $(RENDERER_DIR)/*.cpp)
SYSTEMS_SRCS = $(wildcard $(SYSTEMS_DIR)/*.cpp)

GAME_OBJS = $(GAME_SRCS:%.cpp=$(BUILD_DIR)/%.o)
RENDERER_OBJS = $(RENDERER_SRCS:$(RENDERER_DIR)/%.cpp=$(RENDERER_BUILD_DIR)/%.o)
SYSTEMS_OBJS = $(SYSTEMS_SRCS:$(SYSTEMS_DIR)/%.cpp=$(SYSTEMS_BUILD_DIR)/%.o)
OBJS = $(GAME_OBJS) $(RENDERER_OBJS) $(SYSTEMS_OBJS)

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

$(RENDERER_BUILD_DIR)/%.o: $(RENDERER_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SYSTEMS_BUILD_DIR)/%.o: $(SYSTEMS_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	mkdir -p $(BUILD_DIR) $(RENDERER_BUILD_DIR) $(SYSTEMS_BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
