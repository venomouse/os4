#ifndef FILEPERF_H
#define FILEPERF_H

/*
 * File-System Performance Library (fileperf)
 * Author: OS, os@cs.huji.ac.il
 */

#include <vector>

struct fsPerformance {
  long unsigned int space; 
  double averageAccessNum;
};

enum fsType {
  FS_INODE,
  FS_LINKED_LIST
};

/* Returns 0 on success, -1 on failure. On success fills perf. */

int fileSystemPerformance(
  unsigned int blockSize, 
  unsigned int pointerSize,
  unsigned int metaDataSize,
  unsigned int maximalFileSize, 
  fsType type, 
  const std::vector<unsigned int> &fileSizes,
  fsPerformance &perf);

#endif
