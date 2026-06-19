#include "core/Launcher.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define _LIBCPP_DISABLE_AVAILABILITY  // For macOS 10.14+ to use std::filesystem
#ifdef __APPLE__
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

static bool fileExists(const std::string& path) {
  struct stat buffer;
  return (stat(path.c_str(), &buffer) == 0);
}

static void collectJars(const std::string& dir, std::string& classpath, const std::string& sep) {
  DIR* d = opendir(dir.c_str());
  if (!d)
    return;

  struct dirent* entry;
  while ((entry = readdir(d)) != nullptr) {
    std::string name = entry->d_name;
    if (name == "." || name == "..")
      continue;

    std::string fullPath = dir + "/" + name;
    struct stat st;
    if (stat(fullPath.c_str(), &st) == 0) {
      if (S_ISDIR(st.st_mode)) {
        collectJars(fullPath, classpath, sep);
      } else if (name.size() > 4 && name.substr(name.size() - 4) == ".jar") {
        classpath += sep + fullPath;
      }
    }
  }
  closedir(d);
}
#else
// Windows/Linux
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

std::pair<int, std::string> Launcher::buildClasspath(const std::string& minecraftPath,
                                                     const std::string& gameVersion) {
  std::string sep =
#ifdef _WIN32
    ";";
#else
    ":";
#endif

  std::string classpath;
  std::string versionJar = minecraftPath + "/versions/" + gameVersion + "/" + gameVersion + ".jar";
#ifdef __APPLE__
  if (fileExists(versionJar)) {
    classpath += versionJar;
  }
#else
  if (std::filesystem::exists(versionJar)) {
    classpath += versionJar;
  }
#endif

  std::string librariesPath = minecraftPath + "/libraries";
#ifdef __APPLE__
  collectJars(librariesPath, classpath, sep);
#else
  if (std::filesystem::exists(librariesPath)) {
    for (const auto& entry : std::filesystem::recursive_directory_iterator(librariesPath)) {
      if (entry.is_regular_file() && entry.path().extension() == ".jar") {
        classpath += sep + entry.path().string();
      }
    }
  }
#endif

  return std::make_pair(classpath.empty() ? 0 : 1, classpath);
}

Launcher::LaunchStatus Launcher::launchGame(Launcher::LaunchOptions options) {
  // Debug logs
  // std::cout << "[Launcher] MinecraftPath: " << options.MinecraftPath << std::endl;
  // std::cout << "[Launcher] GameVersion: " << options.GameVersion << std::endl;
  // std::cout << "[Launcher] Current path: " << std::filesystem::current_path() << std::endl;
  // std::cout << "[Launcher] Using Java: " << options.JavaPath << std::endl;
  std::vector<std::string> cmd;
  // JVM arguments
  cmd.push_back(options.JavaPath);
  cmd.push_back("-Xmx" + std::to_string(options.MaxMemory) + "M");
  cmd.push_back("-Djava.library.path=" + options.MinecraftPath + "/natives");

  // Class path
  cmd.push_back("-cp");
  auto [status, classpath] = buildClasspath(options.MinecraftPath, options.GameVersion);
  if (status != 1) {
    return Launcher::LaunchStatus::InvalidPath;
  }
  cmd.push_back(classpath);

  // Main class
  cmd.push_back("net.minecraft.client.main.Main");

  // Game arguments
  cmd.push_back("--username " + options.PlayerName);
  cmd.push_back("--version");
  cmd.push_back(options.GameVersion);
  cmd.push_back("--gameDir " + options.MinecraftPath);
  cmd.push_back("--assetsDir " + options.MinecraftPath + "/assets");
  cmd.push_back("--width " + std::to_string(options.WindowSize.first));
  cmd.push_back("--height " + std::to_string(options.WindowSize.second));
  for (const auto& arg : options.extraArgs) {
    cmd.push_back(arg);
  }

  if (!options.online) {
    cmd.push_back("--uuid");
    cmd.push_back("00000000-0000-0000-0000-000000000000");
    cmd.push_back("--accessToken");
    cmd.push_back("0");
    cmd.push_back("--userType");
    cmd.push_back("legacy");
  }

  std::string command;
  for (const auto& arg : cmd) {
    if (arg.find(' ') != std::string::npos) {
      command += "\"" + arg + "\" ";
    } else {
      command += arg + " ";
    }
  }

  // Debug log
  std::cout << "[Launcher] FULL COMMAND: " << command << std::endl;

  int result = std::system(command.c_str());

  if (result == 0) {
    return LaunchStatus::Success;
  } else {
    return LaunchStatus::Failed;
  }
  return Launcher::LaunchStatus::Success;
}
}  // namespace core
}  // namespace ETL