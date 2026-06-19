#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "core/Launcher.h"

int main() {
  ETL::core::Launcher launcher;
  std::string javaPath = "java";
  bool result = launcher.checkJavaPath(javaPath);
  if (result) {
    std::cout << "Java path is valid." << std::endl;
  } else {
    std::cout << "Java path is invalid." << std::endl;
  }
  return 0;
}