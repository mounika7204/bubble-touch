#pragma once

#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <chrono>
#include <functional>
#include <shared_mutex>
#include <string>
#include <thread>

#include <openssl/md5.h>

class File_Watcher {
public:
  using Callback = std::function<void()>;

  File_Watcher() = default;

  /// Whether this file watcher has been cancelled.
  [[nodiscard]] bool is_cancelled() noexcept;

  /// Stop this watcher.
  void cancel() noexcept;

  /// Start watching for changes to the provided file.
  void watch(const char* filename, Callback cb, int refresh_rate = 5) noexcept;

private:
  bool m_cancelled            = false;
  const unsigned char* m_data = nullptr;
  std::shared_mutex m_mutex;
};