#include "pipeline.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>


Pipeline::Pipeline(const std::string& vertFilePath, const std::string& fragFilePath){
  createGraphicsPipeline(vertFilePath, fragFilePath);
}

std::vector<char> Pipeline::readFile(const std::string& filepath){


  // Ate flag means seek to end immediately, binary assures we read in as a binary 
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()){
    throw std::runtime_error("Failed to open file: " + filepath);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();
  return buffer;

}



void Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath){
  auto vertCode = readFile(vertFilePath);
  auto fragCode = readFile(fragFilePath);

  std::cout << "Vertex Shader size: " << vertCode.size() << "\n";
  std::cout << "Fragment Shader size: " << fragCode.size() << "\n"; 
}

