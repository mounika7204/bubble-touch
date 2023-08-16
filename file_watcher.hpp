#pragma once

#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <chrono>
#include <shared_mutex>
#include <string>
#include <thread>

#include <openssl/md5.h>

class File_Watcher {
public:
  File_Watcher() = default;

  /// Whether this file watcher has been cancelled.
  [[nodiscard]] bool is_cancelled() noexcept;

  /// Stop this watcher.
  void cancel() noexcept;

  /// Start watching for changes to the provided file.
  template <typename Callback>
  void watch(const char* filename, Callback cb, int refresh_rate = 5) noexcept
  {
    auto fd = open(filename, O_RDONLY);
    if (fd == -1) {
      fprintf(stderr, "Failed to open file: %s: %s", filename, strerror(errno));
      return;
    }

    m_data = static_cast<const char*>(mmap( //
        nullptr,
        MAP_LENGTH,
        PROT_READ,
        MAP_PRIVATE | MAP_POPULATE,
        fd,
        0 //
        ) //
    );

    if (m_data == MAP_FAILED) {
      fprintf(stderr, "Failed to map config file: %s", strerror(errno));
      return;
    }

    close(fd);

    std::thread([this, cb, refresh_rate] {
      char last_hash[MD5_DIGEST_LENGTH]    = { 0 };
      char current_hash[MD5_DIGEST_LENGTH] = { 0 };

      while (!is_cancelled()) {
        MD5((unsigned char*)m_data, strlen(m_data), (unsigned char*)current_hash);
        if (memcmp(current_hash, last_hash, MD5_DIGEST_LENGTH) != 0) {
          memcpy(last_hash, current_hash, MD5_DIGEST_LENGTH);
          memset(current_hash, 0, MD5_DIGEST_LENGTH);
          cb();
        }
        std::this_thread::sleep_for(std::chrono::seconds(refresh_rate));
      }
    }).detach();
  }

private:
  bool m_cancelled   = false;
  const char* m_data = nullptr;
  std::shared_mutex m_mutex;

  static constexpr const int MAP_LENGTH = 4096;
};