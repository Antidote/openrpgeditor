#pragma once
#include "Utils/SignalSlot.hpp"

#include <condition_variable>
#include <filesystem>
#include <string_view>

class DirectoryMonitor {
public:
  ~DirectoryMonitor() { stop(); }
  void setWatchDirectory(const std::string_view watchDirectory) {
    std::unique_lock __(m_mutex);
    m_watchDirectory = watchDirectory;
  }

  void start() {
    m_running = true;
    m_watchDirectoryThread = std::thread(&DirectoryMonitor::watchDirectory, this);
  }

  void stop() {
    m_running = false;
    m_cv.notify_one();

    if (m_watchDirectoryThread.joinable()) {
      m_watchDirectoryThread.join();
    }
  }

  signal<void(const std::filesystem::path&)>& fileChanged() { return m_fileChanged; }
  signal<void(const std::filesystem::path&)>& fileAdded() { return m_fileAdded; }
  signal<void(const std::filesystem::path&)>& fileDeleted() { return m_fileDeleted; }
  signal<void(const std::filesystem::path&)>& directoryChanged() { return m_directoryChanged; }
  signal<void(const std::filesystem::path&)>& directoryAdded() { return m_directoryAdded; }
  signal<void(const std::filesystem::path&)>& directoryDeleted() { return m_directoryDeleted; }

private:
  void watchDirectory();
  std::string m_watchDirectory;
  signal<void(const std::filesystem::path&)> m_fileChanged;
  signal<void(const std::filesystem::path&)> m_fileAdded;
  signal<void(const std::filesystem::path&)> m_fileDeleted;
  signal<void(const std::filesystem::path&)> m_directoryChanged;
  signal<void(const std::filesystem::path&)> m_directoryAdded;
  signal<void(const std::filesystem::path&)> m_directoryDeleted;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_running = false;
  std::thread m_watchDirectoryThread;
};