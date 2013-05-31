

#include "fileperf.hh"

#define FAIL -1
#define SUCCESS 0
#define EMPTY_FILE_ACCESS 0
#define INODE_ONE_POINTER_ACCESS 2
#define INODE_ONE_POINTER_BLOCK_NUMBER 2

// Calculates the average number of accesses for the given file
//	and the space used by it
fsPerformance filePerformanceLinkedList (unsigned int fileSize,
		unsigned int blockSize,
		unsigned int pointerSize,
		unsigned int metaDataSize)
{
	long unsigned int firstBlockDataSize = blockSize - pointerSize - metaDataSize;
	long unsigned int dataSize = blockSize - pointerSize;
	long int curFileSize = fileSize;
	long unsigned int numBlock = 1;
	long long accessSum = 0;
	fsPerformance filePerformance;

	for (; curFileSize > 0; numBlock++)
	{
		if (numBlock == 1)
		{
			if (firstBlockDataSize > curFileSize)
			{
				accessSum += curFileSize;
				curFileSize = 0;
			}
			else
			{
				curFileSize -= firstBlockDataSize;
				accessSum +=firstBlockDataSize;
			}
		}

		else
		{
			if (dataSize > curFileSize)
			{
				accessSum += curFileSize*numBlock;
				curFileSize = 0;
			}
			else
			{
				curFileSize -= dataSize;
				accessSum +=  dataSize*numBlock;
			}
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
	 double totalAccess = 0;
	 unsigned int nonEmptyFileCounter = 0;
	 fsPerformance filePerformance;

	 for (; it != fileSizes.end(); it++)
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
		 perf.averageAccessNum = (double) totalAccess / (double) nonEmptyFileCounter;
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

		for (unsigned int i =0; i < numHeaderPointers; i++)
		{
			if (headerBlockPointers[i] == 0)
			{
				break;
			}
			else
			{
				//TODO - remove
				maxSpace += pow ((double)numBlockPointers, (int)i) * headerBlockPointers[i] * blockSize;
			}

		}

		if (maxSpace >= maximalFileSize)
		{
			return true;
		}

		return false;

}

unsigned int* calculateHeaderBlockPointers (unsigned int blockSize,
		unsigned int numHeaderPointers,
		unsigned int numBlockPointers,
		unsigned int maximalFileSize)
{

		if ((numHeaderPointers == 1 && maximalFileSize > blockSize))
		{
			return NULL;
		}

		unsigned int* headerBlockPointerLevels = new unsigned int [numHeaderPointers];
		if (headerBlockPointerLevels == NULL)
		{
			return NULL;
		}

		headerBlockPointerLevels[0] = numHeaderPointers;
		for (unsigned int i = 1; i < numHeaderPointers; i++)
		{
			headerBlockPointerLevels[i] = 0;
		}

		unsigned int currLevel = 1;
		for ( ; !isEnoughSpace(blockSize, numHeaderPointers, headerBlockPointerLevels,numBlockPointers,maximalFileSize); currLevel++)
		{
			if (currLevel == numHeaderPointers)
			{
				delete [] headerBlockPointerLevels;
				return NULL;
			}

			headerBlockPointerLevels[0] -= 1;
			headerBlockPointerLevels[currLevel] = 1;
		}

		return headerBlockPointerLevels;
}

fsPerformance filePerformanceInode( unsigned int fileSize,
				unsigned int blockSize,
				unsigned int numHeaderPointers,
				unsigned int numBlockPointers,
				unsigned int* headerBlockPointerLevels)
{
		unsigned int currSize = fileSize;
		unsigned int level = 0;
		fsPerformance filePerformance;
		//any file has at least one block
		unsigned int numBlocks= 1;
		double accessNum = 0;

		//assuming 1 direct pointer is enough for maximal size files
		if (numHeaderPointers == 1)
		{
			filePerformance.space = blockSize * INODE_ONE_POINTER_BLOCK_NUMBER;
			filePerformance.averageAccessNum = INODE_ONE_POINTER_ACCESS;
			return filePerformance;
		}

		for (; level < numHeaderPointers; level++)
		{
			//TODO - remove
			unsigned int fullLevelCapacity = blockSize * pow ((double)numBlockPointers,(int) level) * headerBlockPointerLevels[level];
			if (fullLevelCapacity > currSize )
			{
				break;
			}
			//the whole level of data is filled

			currSize -= fullLevelCapacity;
			//using geometrical series sum formula
			//TODO remove
			numBlocks += headerBlockPointerLevels[level] * (pow ((double)numBlockPointers,(int)level + 1) - 1)/ (numBlockPointers -1);
			accessNum += pow ((double)numBlockPointers, (int) level) * (level+2) * blockSize * headerBlockPointerLevels[level];

		}

		if (currSize > 0)
		{
			//there is a part-filled level
			//updating the access number
			accessNum += currSize * (level +2 );
			unsigned int lastLevelBlocks = (int)ceil((double) currSize / (double) blockSize);
			//adding the number of data blocks in the last level
			numBlocks += lastLevelBlocks;
			//adding the pointer blocks for the last level (no need to add again the header block)
			for (; level  > 0; level --)
			{
				lastLevelBlocks = ceil((double)lastLevelBlocks/(double)numBlockPointers);
				numBlocks +=lastLevelBlocks;
			}
		}

		filePerformance.space = numBlocks*blockSize;
		filePerformance.averageAccessNum = accessNum / (double) fileSize;
		return filePerformance;
  }

int calculateInodePerformance (  unsigned int blockSize,
		unsigned int pointerSize,
		unsigned int metaDataSize,
		unsigned int maximalFileSize,
		const std::vector<unsigned int> &fileSizes,
		fsPerformance &perf)
{

		unsigned int numHeaderPointers = (blockSize - metaDataSize) / pointerSize;
		if (numHeaderPointers == 0)
		{
			return FAIL;
		}

		if ((numHeaderPointers == 1 && maximalFileSize > blockSize))
		{
			return FAIL;
		}


		unsigned int numBlockPointers = blockSize / pointerSize;

		unsigned int* headerBlockPointerLevels = calculateHeaderBlockPointers (blockSize,
						numHeaderPointers,numBlockPointers, maximalFileSize);


		if (headerBlockPointerLevels == NULL)
		{
			return FAIL;
		}

		//for debugging purposes, we save the file performances until the
		//end of function run;
		std::vector<fsPerformance>  allFiles = std::vector<fsPerformance>();
		unsigned int totalSpace =0;
		double totalAccessNum = 0;
		unsigned int nonEmptyFileCounter = 0;
		std::vector<unsigned int>::const_iterator it = fileSizes.begin();

		for ( ; it != fileSizes.end(); it++)
		{
			fsPerformance filePerformance;
			if (*it >0)
			{
				nonEmptyFileCounter++;
				filePerformance = filePerformanceInode (*it,blockSize,numHeaderPointers,numBlockPointers,headerBlockPointerLevels);
			}

			else
			{
				filePerformance.space = blockSize;
				filePerformance.averageAccessNum = EMPTY_FILE_ACCESS;
			}

			totalSpace += filePerformance.space;
			totalAccessNum += filePerformance.averageAccessNum;
			allFiles.push_back(filePerformance);

		}

		perf.space = totalSpace;
		perf.averageAccessNum = (double) totalAccessNum / (double) nonEmptyFileCounter;

		return SUCCESS;
}

int checkInput (unsigned int blockSize,
				unsigned int pointerSize,
				unsigned int maximalFileSize,
				unsigned int metaDataSize,
				const std::vector<unsigned int> &fileSizes)
{
	std::vector<unsigned int>::const_iterator it = fileSizes.begin();

	if (blockSize <= 0 || pointerSize <=0 || maximalFileSize <= 0 || pointerSize + metaDataSize > blockSize)
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
