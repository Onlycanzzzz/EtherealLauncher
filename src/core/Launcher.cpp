#include "core/Launcher.h"
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace ETL {
namespace core {

namespace fs = std::filesystem;

bool Launcher::checkJavaPath(const std::string& path) {
  if (path.empty()) {
    return false;
  }
  int result = system((path + " -version").c_str());
  return result == 0;
}
}  // namespace core
}  // namespace ETL