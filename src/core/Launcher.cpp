#include "core/Launcher.h"
#include <cstdlib>
#include <string>
#include <vector>

#ifdef __APPLE__  // macOS
#include <sys/stat.h>
#include <unistd.h>

static bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}
#else  // Windows/Linux
#include <filesystem>
namespace fs = std::filesystem;
#endif

namespace ETL {
namespace core {

bool Launcher::checkJavaPath(const std::string& path) {
  if (path.empty()) {
    return false;
  }

  // macOS
#ifdef __APPLE__
  if (!fileExists(path)) {
    return false;
  }
#else  // Windows/Linux
  if (!fs::exists(path)) {
    return false;
  }
#endif

  int result = system((path + " -version").c_str());
  return result == 0;
}

}  // namespace core
}  // namespace ETL