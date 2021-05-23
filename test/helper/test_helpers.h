#ifndef MONITOR_TEST_HELPERS_H
#define MONITOR_TEST_HELPERS_H

#include <experimental/filesystem>

/*
 * source: https://stackoverflow.com/questions/25211110/find-external-test-file-for-unit-test-by-relative-path-c-cmake-guest
 */
inline std::string resolvePath(const std::string &relPath)
{
  namespace fs = std::experimental::filesystem;
  auto baseDir = fs::current_path();
  while (baseDir.has_parent_path())
  {
    auto combinePath = baseDir / relPath;
    if (fs::exists(combinePath))
    {
      return combinePath.string();
    }
    baseDir = baseDir.parent_path();
  }
  throw std::runtime_error("File (relative path: " + relPath + ") not found!");
}

#endif  // MONITOR_TEST_HELPERS_H
