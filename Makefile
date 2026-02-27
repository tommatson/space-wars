include .env

CXX = g++
CXXFLAGS = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib -Wl,-rpath,$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

ENGINE_DIR = engine
RENDERER_DIR = $(ENGINE_DIR)/renderer
SYSTEMS_DIR = $(RENDERER_DIR)/systems
NETWORK_DIR = $(ENGINE_DIR)/network
SOCKETS_DIR = $(NETWORK_DIR)/sockets
CORE_DIR = $(ENGINE_DIR)/core
GAME_SRC_DIR = game/src
GAME_SCENES_DIR = game/scenes
BUILD_DIR = build
RENDERER_BUILD_DIR = $(BUILD_DIR)/engine/renderer
SYSTEMS_BUILD_DIR = $(BUILD_DIR)/engine/renderer/systems
NETWORK_BUILD_DIR = $(BUILD_DIR)/engine/network
SOCKETS_BUILD_DIR = $(BUILD_DIR)/engine/network/sockets
CORE_BUILD_DIR = $(BUILD_DIR)/engine/core
GAME_SRC_BUILD_DIR = $(BUILD_DIR)/game/src
GAME_SCENES_BUILD_DIR = $(BUILD_DIR)/game/scenes
TARGET = $(BUILD_DIR)/space-wars

GAME_SRC_SRCS = $(wildcard $(GAME_SRC_DIR)/*.cpp)
GAME_SCENES_SRCS = $(wildcard $(GAME_SCENES_DIR)/*.cpp)
RENDERER_SRCS = $(wildcard $(RENDERER_DIR)/*.cpp)
SYSTEMS_SRCS = $(wildcard $(SYSTEMS_DIR)/*.cpp)
NETWORK_SRCS = $(wildcard $(NETWORK_DIR)/*.cpp)
SOCKETS_SRCS = $(wildcard $(SOCKETS_DIR)/*.cpp)
CORE_SRCS = $(wildcard $(CORE_DIR)/*.cpp)

GAME_SRC_OBJS = $(GAME_SRC_SRCS:$(GAME_SRC_DIR)/%.cpp=$(GAME_SRC_BUILD_DIR)/%.o)
GAME_SCENES_OBJS = $(GAME_SCENES_SRCS:$(GAME_SCENES_DIR)/%.cpp=$(GAME_SCENES_BUILD_DIR)/%.o)
RENDERER_OBJS = $(RENDERER_SRCS:$(RENDERER_DIR)/%.cpp=$(RENDERER_BUILD_DIR)/%.o)
SYSTEMS_OBJS = $(SYSTEMS_SRCS:$(SYSTEMS_DIR)/%.cpp=$(SYSTEMS_BUILD_DIR)/%.o)
NETWORK_OBJS = $(NETWORK_SRCS:$(NETWORK_DIR)/%.cpp=$(NETWORK_BUILD_DIR)/%.o)
SOCKETS_OBJS = $(SOCKETS_SRCS:$(SOCKETS_DIR)/%.cpp=$(SOCKETS_BUILD_DIR)/%.o)
CORE_OBJS = $(CORE_SRCS:$(CORE_DIR)/%.cpp=$(CORE_BUILD_DIR)/%.o)
OBJS = $(GAME_SRC_OBJS) $(GAME_SCENES_OBJS) $(RENDERER_OBJS) $(SYSTEMS_OBJS) $(NETWORK_OBJS) $(SOCKETS_OBJS) $(CORE_OBJS)

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

$(GAME_SRC_BUILD_DIR)/%.o: $(GAME_SRC_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GAME_SCENES_BUILD_DIR)/%.o: $(GAME_SCENES_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RENDERER_BUILD_DIR)/%.o: $(RENDERER_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SYSTEMS_BUILD_DIR)/%.o: $(SYSTEMS_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NETWORK_BUILD_DIR)/%.o: $(NETWORK_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SOCKETS_BUILD_DIR)/%.o: $(SOCKETS_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CORE_BUILD_DIR)/%.o: $(CORE_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	mkdir -p $(BUILD_DIR) $(RENDERER_BUILD_DIR) $(SYSTEMS_BUILD_DIR) $(NETWORK_BUILD_DIR) $(SOCKETS_BUILD_DIR) $(CORE_BUILD_DIR) $(GAME_SRC_BUILD_DIR) $(GAME_SCENES_BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
