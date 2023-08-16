#include "file_watcher.hpp"

#define MAP_LENGTH 4096

bool File_Watcher::is_cancelled() noexcept
{
  std::shared_lock lock { m_mutex };
  return m_cancelled;
}

void File_Watcher::cancel() noexcept
{
  std::unique_lock lock { m_mutex };
  munmap((void*)m_data, MAP_LENGTH);
  m_cancelled = true;
}

void File_Watcher::watch(const char* filename, Callback cb, int refresh_rate) noexcept
{
  auto fd = open(filename, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Failed to open file: %s: %s", filename, strerror(errno));
    return;
  }

  m_data = static_cast<const unsigned char*>(mmap( //
      nullptr,
      MAP_LENGTH,
      PROT_READ,
      MAP_PRIVATE | MAP_POPULATE,
      fd,
      0 //
      ));

  close(fd);

  if (m_data == MAP_FAILED) {
    fprintf(stderr, "Failed to map config file: %s", strerror(errno));
    return;
  }

  std::thread([this, cb, refresh_rate] {
    unsigned char last_hash[MD5_DIGEST_LENGTH]    = { 0 };
    unsigned char current_hash[MD5_DIGEST_LENGTH] = { 0 };

    while (!is_cancelled()) {
      MD5(m_data, strlen((char*)m_data), current_hash);
      if (memcmp(current_hash, last_hash, MD5_DIGEST_LENGTH) != 0) {
        memcpy(last_hash, current_hash, MD5_DIGEST_LENGTH);
        memset(current_hash, 0, MD5_DIGEST_LENGTH);
        cb();
      }
      std::this_thread::sleep_for(std::chrono::seconds(refresh_rate));
    }
  }).detach();
}
