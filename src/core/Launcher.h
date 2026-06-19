#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace ETL {
namespace core {
class Launcher {
public:
  enum class LaunchStatus {
    Success,
    Failed,
    AlreadyRunning,
    JavaNotFound,
    InvalidPath,
    UnknownError
  };
  struct LaunchOptions {
    bool online = false;
    std::string JavaPath = "";
    std::string MinecraftPath = "";
    int MaxMemory = 2048;
    std::string PlayerName = "";
    std::string VersionType = "EtherealLauncher";
    std::pair<int, int> WindowSize = {800, 680};
    std::vector<std::string> extraArgs;
  };
  Launcher() = default;
  ~Launcher() = default;

  LaunchStatus launchGame(LaunchOptions options);
  bool stopGame();

  bool checkJavaPath(const std::string& path);

private:
  LaunchStatus offlineLaunch(LaunchOptions options);
  LaunchStatus onlineLaunch(LaunchOptions options);
};

}  // namespace core
}  // namespace ETL