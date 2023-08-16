#include "file_watcher.hpp"

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