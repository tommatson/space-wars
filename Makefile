include .env

CXX = g++
CXXFLAGS = -std=c++17 -I$(THINENG_PATH) -I$(VULKAN_SDK_PATH)/include -I$(STB_INCLUDE_PATH) -I$(TINYOBJ_INCLUDE_PATH)
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib -Wl,-rpath,$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan


BUILD_DIR = build
TARGET = $(BUILD_DIR)/space-wars
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

VERT_SRCS = $(wildcard shaders/*.vert)
FRAG_SRCS = $(wildcard shaders/*.frag)
SPV_FILES = $(VERT_SRCS:%.vert=%.vert.spv) $(FRAG_SRCS:%.frag=%.frag.spv)


all: shaders $(TARGET)

shaders: $(SPV_FILES)

shaders/%.vert.spv: shaders/%.vert
	$(GLSLC) $< -o $@

shaders/%.frag.spv: shaders/%.frag
	$(GLSLC) $< -o $@

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
