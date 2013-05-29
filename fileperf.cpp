

#include "fileperf.hh"

#define FAIL -1
#define SUCCESS 0

// Calculates the average number of accesses for the given file
//	and the space used by it
fsPerformance filePerformanceLinkedList (unsigned int fileSize,
		unsigned int blockSize,
		unsigned int pointerSize,
		unsigned int metaDataSize)
{
	unsigned int firstBlockDataSize = blockSize - pointerSize - metaDataSize;
	unsigned int dataSize = blockSize - pointerSize;
	int curFileSize = fileSize;
	unsigned int numBlock = 1;
	unsigned int accessSum = 0;
	fsPerformance filePerformance;

	for (; curFileSize > 0; numBlock++)
	{
		if (numBlock == 1)
		{
			curFileSize -= firstBlockDataSize;
			accessSum +=firstBlockDataSize;
		}

		else
		{
			curFileSize -= dataSize;
			accessSum += dataSize*numBlock;
		}
	}
	filePerformance.averageAccessNum = (double) accessSum / (double) fileSize;
	filePerformance.space = (numBlock-1)*blockSize;

	return filePerformance;
}

int calculateLinkedListPerformance ( unsigned int blockSize,
		unsigned int pointerSize,
		unsigned int metaDataSize,
		const std::vector<unsigned int> &fileSizes,
		fsPerformance &perf)
{
	 std::vector<unsigned int>::const_iterator it = fileSizes.begin();
	 unsigned int totalSpace = 0;
	 unsigned int totalAccess = 0;
	 unsigned int nonEmptyFileCounter;
	 fsPerformance filePerformance;

	 while (it != fileSizes.end())
	 {
		 if (*it > 0)
		 {
			 filePerformance = filePerformanceLinkedList (*it, blockSize, pointerSize, metaDataSize);
			 totalSpace += filePerformance.space;
			 totalAccess += filePerformance.averageAccessNum;
			 nonEmptyFileCounter++;
		 }
		 //empty files don't count in averaging the access number
		 else
		 {
			 totalSpace += blockSize;
		 }
	 }

	 perf.space = totalSpace;

	 if (nonEmptyFileCounter > 0)
	 {
		 perf.averageAccessNum = totalAccess / nonEmptyFileCounter;
	 }

	 else
	 {
		 perf.averageAccessNum = 0;
	 }

	 return SUCCESS;
}

bool isEnoughSpace (unsigned int blockSize,
		unsigned int numHeaderPointers,
		unsigned int* headerBlockPointers,
		unsigned int numBlockPointers,
		unsigned int maximalFileSize)
{
		unsigned int maxSpace = 0;

		for (int i =0; i < numHeaderPointers; i++)
		{
			if (headerBlockPointers[i] == 0)
			{
				break;
			}
			else
			{
				maxSpace += pow (numBlockPointers, i) * headerBlockPointers[i] * blockSize;
			}

		}

		if (maxSpace > maximalFileSize)
		{
			return true;
		}

		return false;

}

unsigned int* calculateHeaderBlockPointers (unsigned int blockSize,
		unsigned int pointerSize,
		unsigned int metaDataSize,
		unsigned int maximalFileSize)
{
		// The maximal number of pointers in header block
		unsigned int numHeaderPointers = (blockSize - metaDataSize)/pointerSize;
		// The maximal number of pointers in pointer block
		unsigned int numBlockPointers = blockSize / pointerSize;

		if (numHeaderPointers == 0)
		{
			return NULL;
		}

		unsigned int* headerBlockPointerLevels = new unsigned int [numHeaderPointers];
		headerBlockPointerLevels[0] = numHeaderPointers;
		for (int i = 1; i < numHeaderPointers; i++)
		{
			headerBlockPointerLevels = 0;
		}

		int currLevel = 0;
		while (!isEnoughSpace(blockSize, numHeaderPointers, headerBlockPointerLevels,numBlockPointers,maximalFileSize))
		{
			if (currLevel == numHeaderPointers - 1)
			{
				delete [] headerBlockPointerLevels;
				return NULL;
			}

			headerBlockPointerLevels[0] -= 1;
			currLevel++;
			headerBlockPointerLevels[currLevel] = 1;
		}

		return headerBlockPointerLevels;
}

fsPerformance filePerformanceInode( unsigned int fileSize,
				unsigned int blockSize,
				unsigned int pointerSize,
				unsigned int metaDataSize,
				unsigned int* headerBlockPointerLevels)
{

}

int calculateInodePerformance (  unsigned int blockSize,
		unsigned int pointerSize,
		unsigned int metaDataSize,
		unsigned int maximalFileSize,
		const std::vector<unsigned int> &fileSizes,
		fsPerformance &perf)
{
		unsigned int* headerBlockPointerLevels = calculateHeaderBlockPointers (blockSize,
						pointerSize,metaDataSize, maximalFileSize);

		if (headerBlockPointerLevels == NULL)
		{
			return FAIL;
		}

		int fileNum = fileSizes.size();
		std::vector<fsPerformance> =

}

int checkInput (unsigned int blockSize,
				unsigned int pointerSize,
				unsigned int maximalFileSize,
				unsigned int metaDataSize,
				const std::vector<unsigned int> &fileSizes)
{
	std::vector<unsigned int>::const_iterator it = fileSizes.begin();

	if (blockSize <= 0 || pointerSize <=0 || maximalFileSize <= 0 || pointerSize >= blockSize)
	{
		return FAIL;
	}

	for (;it != fileSizes.end(); it++)
	{
		if (*it > maximalFileSize)
		{
			return FAIL;
		}



	}

	return SUCCESS;

}

int fileSystemPerformance(
  unsigned int blockSize,
  unsigned int pointerSize,
  unsigned int metaDataSize,
  unsigned int maximalFileSize,
  fsType type,
  const std::vector<unsigned int> &fileSizes,
  fsPerformance &perf)
{
	if (checkInput (blockSize, pointerSize, maximalFileSize, metaDataSize, fileSizes))
	{
		return FAIL;
	}

	switch (type)
	{
	case FS_INODE:
		return calculateInodePerformance(blockSize, pointerSize, metaDataSize, maximalFileSize, fileSizes, perf);
	case FS_LINKED_LIST:
		return calculateLinkedListPerformance (blockSize, pointerSize, metaDataSize, fileSizes, perf);

	}


	return SUCCESS;
}
