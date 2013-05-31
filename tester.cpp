/*
 * OS File System Structure Tester 2013
 *
 * Written By: Victor Kats, Alon Ben-shimol
 */

#include "fileperf.hh"
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

#define FAILED_TEST_RET(x) { cout << "Failed Test: "<< x <<"\n\tFunction returned -1 where it shouldn't" << endl; }
#define PASSED_TEST(x) { cout << "Passed Test "<< x << endl; }
#define FAILED_TEST(x,ts,us,ta,ua) \
		{ \
			cout << "Failed Test "<< x << endl; \
			cout << "\tExpected Space: "   << ts << endl; \
			cout << "\tReturned Space: "   << us << endl; \
			cout << "\tExpected Average: " << ta << endl; \
			cout << "\tReturned Average: " << ua << endl << endl; \
		} \

#define EPSILON 0.001


vector<unsigned int> myVector(1);
fsPerformance fsp;


////////////LINKED_LIST TEST///////////////////////////
//////////////////////////////////////////////////////

// Illegal parameters test - Linked_List
void test_1()
{
	int ret;
	myVector.clear();

	ret = fileSystemPerformance(20,21,1,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 1;" << endl;
		cout << "\tFunction should have failed since pointer size is bigger than blockSize." << endl;
		return;
	}

	ret = fileSystemPerformance(20,20,1,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 1;" << endl;
		cout << "\tFunction should have failed since pointer size is equal blockSize." << endl;
		return;
	}


	ret = fileSystemPerformance(20,1,21,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 1;" << endl;
		cout << "\tFunction should have failed since metaData size is bigger than blockSize." << endl;
		return;
	}

	ret = fileSystemPerformance(20,2,19,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 1;" << endl;
		cout << "\tFunction should have failed since metaData size + pointer size is bigger than blockSize." << endl;
		return;
	}

	myVector.push_back(51);

	ret = fileSystemPerformance(20,1,21,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 1;" << endl;
		cout << "\tFunction should have failed since there's a file larger than maximal file size." << endl;
		return;
	}


	PASSED_TEST(1);
}

// Illegal parameters test - I-node
void test_2()
{
	int ret;
	myVector.clear();

	ret = fileSystemPerformance(20,21,1,50, FS_INODE, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 2;" << endl;
		cout << "\tFunction should have failed since pointer size is bigger than blockSize" << endl;
		return;
	}


	ret = fileSystemPerformance(20,1,21,50, FS_INODE, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 2;" << endl;
		cout << "\tFunction should have failed since metaData size is bigger than blockSize" << endl;
		return;
	}

	ret = fileSystemPerformance(20,2,19,50, FS_INODE, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 2;" << endl;
		cout << "\tFunction should have failed since metaData size + pointer size is bigger than blockSize" << endl;
		return;
	}

	myVector.push_back(51);

	ret = fileSystemPerformance(20,1,21,50, FS_INODE, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 2;" << endl;
		cout << "\tFunction should have failed since there's a file larger than maximal file size" << endl;
		return;
	}


	myVector.clear();
	myVector.push_back(2);

	ret = fileSystemPerformance(20,4,16,50, FS_INODE, myVector,fsp );
	if ( ret != -1 )
	{
		cout << "Failed Test 2;" << endl;
		cout << "\tFunction should have failed since no I-node can support this data." << endl;
		return;
	}

	PASSED_TEST(2);
}

/////////////////////////LINKED_LIST TESTS///////////////////////
/////////////////////////////////////////////////////////////////
/*
 * vector with one file
 * file = 50
 * pointer = 2
 * metaData = 10
 * blockSize = 20
 *
 * the perf values should be:
 * space = 80
 * acgAcc = 2.44
 */
void test_3()
{
	int ret;
	myVector.clear();

	myVector.push_back(50);

	ret = fileSystemPerformance(20,2,10,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(3);
		return;
	}

	if ( fsp.space != 80 || fabs(fsp.averageAccessNum - 2.44) > EPSILON )
	{
		FAILED_TEST(3,80, fsp.space, 2.44, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(3);
}


/* vector with one file
 * file = 100
 * pointer = 2
 * metaData = 0
 * blockSize = 20
 *
 * the perf values should be:
 * space = 120
 * acgAcc = 3.3
 */
void test_4()
{
	int ret;
	myVector.clear();

	myVector.push_back(100);

	ret = fileSystemPerformance(20,2,0,100, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(4);
		return;
	}

	if ( fsp.space != 120 || fabs(fsp.averageAccessNum - 3.3) > EPSILON)
	{
		FAILED_TEST(4, 120, fsp.space, 3.3 , fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(4);
	
}

/* vector with one file
 * file = 1001
 * pointer = 1
 * metaData = 50
 * blockSize = 51
 *
 * the perf values should be:
 * space = 1122
 * acgAcc = 11.510489
 */
void test_5()
{
	int ret;
	myVector.clear();

	myVector.push_back(1001);


	ret = fileSystemPerformance(51,1,50,1001, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(5);
		return;
	}

	if ( fsp.space != 1122 || fabs(fsp.averageAccessNum - 11.510489) > EPSILON )
	{
		FAILED_TEST(5, 1122, fsp.space, 11.510489, fsp.averageAccessNum);
		return;
	}
	
	PASSED_TEST(5);
}

/* vector with one file
 * file = 10000
 * pointer = 49
 * metaData = 1
 * blockSize = 51
 *
 * the perf values should be:
 * space = 255051
 * acgAcc = 2501
 */
void test_6()
{
	int ret;
	myVector.clear();

	myVector.push_back(10000);

	ret = fileSystemPerformance(51,49,1,10000, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(6);
		return;
	}

	if ( fsp.space != 255051 || fsp.averageAccessNum != 2501)
	{
		FAILED_TEST(6, 255051, fsp.space, 2501, fsp.averageAccessNum);
		return;
	}
	
	PASSED_TEST(6);
}

/*
 * vector with one file
 * file = 45
 * pointer = 2
 * metaData = 10
 * blockSize = 30
 *
 * the perf values should be:
 * space = 60
 * acgAcc = 1.6
 */
void test_7()
{
	int ret;
	myVector.clear();

	myVector.push_back(45);

	ret = fileSystemPerformance(30,2,10,45, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(7);
		return;
	}

	if ( fsp.space != 60 || fabs(fsp.averageAccessNum - 1.6) > EPSILON )
	{
		FAILED_TEST(7, 60, fsp.space, 1.6, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(7)
}

/*
 * vector with one file
 * file = 100
 * pointer = 1
 * metaData = 1
 * blockSize = 50
 *
 * the perf values should be:
 * space = 150
 * acgAcc = 1.55
 */
void test_8()
{
	int ret;
	myVector.clear();

	myVector.push_back(100);

	
	ret = fileSystemPerformance(50,1,1,100, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(8);
		return;
	}

	if ( fsp.space != 150 || fabs(fsp.averageAccessNum - 1.55) > EPSILON)
	{
		FAILED_TEST(8, 150,fsp.space, 1.55, fsp.averageAccessNum);
		return;
	}
	 
	 PASSED_TEST(8);
}

/*
 * vector with one file
 * file = 50
 * pointer = 20
 * metaData = 30
 * blockSize = 50
 *
 * the perf values should be:
 * space = 150
 * acgAcc = 2.4
 */
void test_9()
{
	int ret;	
	myVector.clear();

	myVector.push_back(50);

	ret = fileSystemPerformance(50,20,30,50, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(9);
		return;
	}

	if ( fsp.space != 150 || fabs(fsp.averageAccessNum - 2.4) > EPSILON )
	{
		FAILED_TEST(9, 150,fsp.space, 2.4, fsp.averageAccessNum);
		return;
	}
	
	PASSED_TEST(9);
}

/*
 * vector with one file
 * file = 20
 * pointer = 2
 * metaData = 10
 * blockSize = 30
 *
 * the perf values should be:
 * space = 60
 * acgAcc = 1.1
 */
void test_10()
{
	int ret;
	myVector.clear();

	myVector.push_back(20);

	ret = fileSystemPerformance(30,2,10,20, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(10);
		return;
	}

	if ( fsp.space != 60 || fabs(fsp.averageAccessNum - 1.1) > EPSILON )
	{
		FAILED_TEST(10, 60, fsp.space, 1.1, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(10);	
}

/*
 * vector with one file
 * file = 46
 * pointer = 2
 * metaData = 10
 * blockSize = 30
 *
 * the perf values should be:
 * space = 60
 * acgAcc = 1.6087
 */
void test_11()
{
	int ret;
	myVector.clear();

	myVector.push_back(46);

	ret = fileSystemPerformance(30,2,10,46, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(11);
		return;
	}

	if ( fsp.space != 60 || fabs(fsp.averageAccessNum - 1.608) > EPSILON )
	{
		FAILED_TEST(11,60, fsp.space, 1.6087, fsp.averageAccessNum);
		return;
	}
	
	PASSED_TEST(11);
}

/*
 * vector with one file
 * file = 2
 * pointer = 2
 * metaData = 10
 * blockSize = 30
 *
 * the perf values should be:
 * space = 30
 * acgAcc = 1
 */
void test_12()
{
	int ret;
	myVector.clear();

	myVector.push_back(2);

	ret = fileSystemPerformance(30,2,10,2, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(12);
		return;
	}

	if ( fsp.space != 30 || fsp.averageAccessNum != 1)
	{
		FAILED_TEST(12, 30, fsp.space, 1, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(12);
}
	
///////////////////////////////i-node test ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
 * vector with one file
 * header should have only one direct pointer
 * file = 20
 * pointer = 4
 * metaData = 16
 * blockSize = 20
 * maxFile = 20
 *
 * the perf values should be:
 * space = 40
 * acgAcc = 2
 */
void test_13()
{
	int ret;
	myVector.clear();

	myVector.push_back(20);

	ret = fileSystemPerformance(20,4,16,20, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(13);
		return;
	}

	if ( fsp.space != 40 || fsp.averageAccessNum != 2)
	{
		FAILED_TEST(13, 40, fsp.space, 2, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(13);
}

/*
 * vector with one file
 * header should have only one direct pointer and one indirect pointer
 * file = 40
 * pointer = 4
 * metaData = 12
 * blockSize = 20
 * maxFile = 60
 *
 * the perf values should be:
 * space = 4 block * blockSize = 80
 * acgAcc = (100/40) = 2.5
 */
void test_14()
{
	int ret;
	myVector.clear();

	myVector.push_back(40);

	ret = fileSystemPerformance(20,4,12,60, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(14);
		return;
	}

	if ( fsp.space != 80 || fabs(fsp.averageAccessNum - 2.5) > EPSILON )
	{

		FAILED_TEST(14, 80, fsp.space, 2.5, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(14);
}

/*
 * vector with one file
 * header should have only one direct pointer and one indirect pointer and one double indirect pointer
 * file = 500
 * pointer = 4
 * metaData = 8
 * blockSize = 20
 * maxFile = 600
 *
 * the perf values should be:
 * space = 32 blocks * blockSize = 640
 * acgAcc = 3.72 (1860/fileSize)
 */
void test_15()
{
	int ret;
	myVector.clear();

	myVector.push_back(500);

	ret = fileSystemPerformance(20,4,8,600, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(15);
		return;
	}

	if ( fsp.space != 640 || fabs(fsp.averageAccessNum - 3.72) > EPSILON )
	{
		FAILED_TEST(15, 640, fsp.space, 3.72, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(15);
}

/*
 * vector with one file
 * header should have only one direct pointer and one indirect pointer and one double indirect pointer and one triple Indirect pointer
 * file = 1000
 * pointer = 4
 * metaData = 4
 * blockSize = 20
 * maxFile = 1200
 *
 * the perf values should be:
 * space = 64 blocks * blockSize = 1280
 * acgAcc = 4.24 (4240/fileSize)
 */
void test_16()
{
	int ret;
	myVector.clear();

	myVector.push_back(1000);

	ret = fileSystemPerformance(20,4,4,1200, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(16);
		return;
	}

	if ( fsp.space != 1280 || fabs(fsp.averageAccessNum - 4.24) > EPSILON )
	{
		FAILED_TEST(16, 1280, fsp.space, 4.24, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(16);
}

/*
 * vector with one file
 * header should have only one direct pointer and 2 indirect pointers.
 * file = 120
 * pointer = 4
 * metaData = 8
 * blockSize = 20
 * maxFile = 140
 *
 * the perf values should be:
 * space = 8 blocks * blockSize = 160
 * acgAcc = 2.666 (320/fileSize)
 */
void test_17()
{
	int ret;
	myVector.clear();

	myVector.push_back(120);

	ret = fileSystemPerformance(20,4,8,140, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(17);
		return;
	}

	if ( fsp.space != 160 || fabs(fsp.averageAccessNum - 2.666) > EPSILON )
	{
		FAILED_TEST(17, 160, fsp.space, 2.666, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(17);
}

/*
 * vector with one file
 * header should have only one direct pointer and 1 indirect pointer and 2 double indirect pointers.
 * file = 1000
 * pointer = 4
 * metaData = 4
 * blockSize = 20
 * maxFile = 1120
 *
 * the perf values should be:
 * space = 63 blocks * blockSize = 1260
 * acgAcc = 3.86 (3860/fileSize)
 */
void test_18()
{
	int ret;
	myVector.clear();

	myVector.push_back(1000);

	ret = fileSystemPerformance(20,4,4,1120, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(18);
		return;
	}

	if ( fsp.space != 1280 || fabs(fsp.averageAccessNum - 4.24) > EPSILON)
	{
		FAILED_TEST(18, 1280, fsp.space, 4.24, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(18);
}

/*
 * vector with one file
 * header should have only one direct pointer and 1 indirect pointer and one double indirect pointer and 2 triple indirect pointers.
 * file = 5000
 * pointer = 4
 * metaData = 4
 * blockSize = 20
 * maxFile = 5620
 *
 * the perf values should be:
 * space = 313 blocks * blockSize = 6260
 * acgAcc = 4.848 (24240/fileSize)
 */
void test_19()
{
	int ret;
	myVector.clear();

	myVector.push_back(5000);

	ret = fileSystemPerformance(20,4,0,5620, FS_INODE, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(19);
		return;
	}

	if ( fsp.space != 6280 || fabs(fsp.averageAccessNum - 5.224) > EPSILON )
	{
		FAILED_TEST(19, 6280, fsp.space, 5.224, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(19);
}

/*
 * vector with 4 files - first file with 40, second with 120, third - 500, last is zero
 * header should have only one direct pointer and 1 indirect pointer and 1 double indirect pointer.
 * files = 40,120,500,0
 * pointer = 4
 * metaData = 8
 * blockSize = 20
 * maxFile = 620
 *
 * the perf values should be:
 * space_1 = 4 blocks * blockSize = 80 //space_2 = 8 blocks * blockSize = 160 // space_3 = 32 blocks * blockSize = 640
  //space_4 = 1 blocks * blockSize = 20
 * acgAcc_1 = 2.5 (100/fileSize) //acgAcc_2 = 2.8333 (340/fileSize) //acgAcc_3 = 3.72 (1860/fileSize) //acgAcc_4 = 0
 *
 * total space: 900
 * total avg = 3.0177
 */
void test_20()
{
	int ret;
	myVector.clear();

	myVector.push_back(40);
	myVector.push_back(120);
	myVector.push_back(500);
	myVector.push_back(0);

	ret = fileSystemPerformance(20,4,8,620, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(20);
		return;
	}

	if ( fsp.space != 900 || fabs(fsp.averageAccessNum - 3.0177)  > EPSILON )
	{
		FAILED_TEST(20, 900, fsp.space, 3.0177, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(20);
}

///////////////////////////////////////tests from last year/////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/*
 * vector with 4 files - first file with 500, second with 195, third - 430 , last - 16777219
 * files = 500,195,430,16777219
 * pointer = 4
 * metaData = 0
 * blockSize = 64
 * maxFile = 16777216
 *
 * should failed return -1 , one of the files is bigger than the maximal size
 */
void test_21()
{
	int ret;
	myVector.clear();

	myVector.push_back(500);
	myVector.push_back(195);
	myVector.push_back(430);
	myVector.push_back(16777219);

	ret = fileSystemPerformance(64,4,0,16777216, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		PASSED_TEST(21);
		return;
	}

	FAILED_TEST_RET(21);	
}

/*
 * vector with 1 files - 32 
 * files = 32
 * pointer = 8
 * metaData = 0
 * blockSize = 128
 * maxFile = 128
 *
 * total space Inode: 256
 * total avg Inode:  2
 * total space LL: 128
 * total avg LL: 1
 */
void test_22()
{
	int ret;
	myVector.clear();

	myVector.push_back(32);
	

	ret = fileSystemPerformance(128,8,0,128, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(22);
		return;
	}

	if ( fsp.space != 256 || fsp.averageAccessNum != 2)
	{
		FAILED_TEST(22, 256, fsp.space, 2, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(128,8,0,128, FS_LINKED_LIST, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(22);
		return;
	}
		if ( fsp.space != 128 || fsp.averageAccessNum != 1)
	{
		FAILED_TEST(22, 128, fsp.space, 1, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(22);
}



/*
 * vector with 1 files - 54
 * files = 54
 * pointer = 9
 * metaData = 0
 * blockSize = 18
 * maxFile = 54
 *
 * total space Inode: 90
 * total avg Inode: = 2.6666666666666666666666666666666666666666666666
 * total space LL: 108
 * total avg LL: 3.5
 */
void test_23()
{
	int ret;
	myVector.clear();
	myVector.push_back(54);

	ret = fileSystemPerformance(18,9,0,54, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(23);
		return;
	}

	if ( fsp.space != 90 || fabs(fsp.averageAccessNum - 2.666) > EPSILON)
	{
		FAILED_TEST(23, 90, fsp.space, 2.666, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(18,9,0,54, FS_LINKED_LIST, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(23);
		return;
	}

	if ( fsp.space != 108 || fabs(fsp.averageAccessNum - 3.5) > EPSILON)
	{
		FAILED_TEST(23, 108, fsp.space, 3.5, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(23);
}


/*
 * Figure out what the values are :)
 */
void test_24()
{
	int ret;
	myVector.clear();
	myVector.push_back(96);

	ret = fileSystemPerformance(24,8,0,120, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(24);
		return;
	}

	if ( fsp.space != 144 || fabs(fsp.averageAccessNum - 2.5) > EPSILON)
	{
		FAILED_TEST(24, 144, fsp.space, 2.5, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(24,8,0,120, FS_LINKED_LIST, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(24);
		return;
	}

	if ( fsp.space != 144 || fabs(fsp.averageAccessNum - 3.5) > EPSILON)
	{
		FAILED_TEST(24, 144, fsp.space, 3.5, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(24);
}




/*
 * Morad megamadov rulez!!!
 */
void test_25()
{
	int ret;
	myVector.clear();
	myVector.push_back(96);
	myVector.push_back(0);

	ret = fileSystemPerformance(24,8,0,120, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(25);
		return;
	}

	if ( fsp.space != 168 || fabs(fsp.averageAccessNum - 2.5) > EPSILON)
	{
		FAILED_TEST(25, 168, fsp.space, 2.5, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(24,8,0,120, FS_LINKED_LIST, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(25);
		return;
	}

	if ( fsp.space != 168 || fabs(fsp.averageAccessNum - 3.5) > EPSILON)
	{
		FAILED_TEST(25, 168, fsp.space, 3.5, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(25);
}





/*
 * "If there are no stupid questions,
 *  then what kind of questions do stupid people ask?
 *  Do they get smart just in time to ask questions? "
 *  Scott Adams
 */
void test_26()
{
	int ret;
	myVector.clear();
	myVector.push_back(500);

	ret = fileSystemPerformance(500,4,0,1000, FS_INODE, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(26);
		return;
	}

	if ( fsp.space != 1000 || fabs(fsp.averageAccessNum - 2) > EPSILON)
	{
		FAILED_TEST(26, 1000, fsp.space, 2, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(500,4,0,1000, FS_LINKED_LIST, myVector,fsp ); // first file
	if ( ret == -1 )
	{
		FAILED_TEST_RET(26);
		return;
	}

	if ( fsp.space != 1000 || fabs(fsp.averageAccessNum - 1.008) > EPSILON)
	{
		FAILED_TEST(26, 1000, fsp.space, 1.008, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(26);
}


/*
 * "Only two things are infinite, the universe and human stupidity,
 *  and I'm not sure about the former."
 *
 *  Bar Refaelli.
 */
void test_27()
{
	int ret;
	myVector.clear();
	myVector.push_back(80);

	ret = fileSystemPerformance(32,4,0,36536226, FS_INODE, myVector,fsp);
	if ( ret == -1 )
	{
		FAILED_TEST_RET(27);
		return;
	}

	if ( fsp.space != 160 || fabs(fsp.averageAccessNum - 2.6) > EPSILON)
	{
		FAILED_TEST(27, 160, fsp.space, 2.6, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(32,4,0,36536226, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(27);
		return;
	}

	if ( fsp.space != 96 || fabs(fsp.averageAccessNum - 1.95) > EPSILON)
	{
		FAILED_TEST(27, 96, fsp.space, 1.95, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(27);
}


/*
 * "5 of every 4 people have problems with math",
 *
 *  Alon Mizrahi shares his wisdom once again.
 */
void test_28()
{
	int ret;
	myVector.clear();
	myVector.push_back(320);

	ret = fileSystemPerformance(32,4,0,36536226, FS_INODE, myVector,fsp);
	if ( ret == -1 )
	{
		FAILED_TEST_RET(28);
		return;
	}

	if ( fsp.space != 448 || fabs(fsp.averageAccessNum - 3) > EPSILON)
	{
		FAILED_TEST(28, 448, fsp.space, 3, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(32,4,0,36536226, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(28);
		return;
	}

	if ( fsp.space != 384 || fabs(fsp.averageAccessNum - 6.225) > EPSILON)
	{
		FAILED_TEST(28, 384, fsp.space, 6.225, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(28);
}

/*
 * "I have nothing to prove, and I proved it today on the court",
 *
 * Alon Mizrahi blows our mind!
 */
void test_29()
{
	int ret;
	myVector.clear();
	myVector.push_back(123468);

	ret = fileSystemPerformance(32,4,0,36536226, FS_INODE, myVector,fsp);
	if ( ret == -1 )
	{
		FAILED_TEST_RET(29);
		return;
	}

	if ( fsp.space != 141216 || fabs(fsp.averageAccessNum - 5.826870) > EPSILON)
	{
		FAILED_TEST(29, 141216, fsp.space, 5.826870, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(32,4,0,36536226, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(29);
		return;
	}

	if ( fsp.space != 141120 || fabs(fsp.averageAccessNum - 2205.286) > EPSILON)
	{
		FAILED_TEST(29, 141120, fsp.space, 2.205286, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(29);
}

/*
 * "Have a good night dear viewers, and may you rest in peace.",
 *
 * Moshe Gertel, the man and the legend, kills us all.
 */
void test_30()
{
	int ret;
	myVector.clear();
	myVector.push_back(9);

	ret = fileSystemPerformance(12,4,0,15155126, FS_INODE, myVector,fsp);
	if ( ret != -1 )
	{
		cout << "Failed Test: 30" << endl;
		cout << "/tFunction should have return -1" << endl;
		return;
	}

	ret = fileSystemPerformance(12,4,0,15155126, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(30);
		return;
	}

	if ( fsp.space != 24 || fabs(fsp.averageAccessNum - 1.11111) > EPSILON)
	{
		FAILED_TEST(30, 24, fsp.space, 1.11111, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(30);
}

/*
 * Did you know?
 * In Alabama it is illegal to wear a fake mustache
 * that causes laughter in church.
 */
void test_31()
{
	int ret;
	myVector.clear();
	myVector.push_back(12);
	myVector.push_back(150);
	myVector.push_back(800);
	myVector.push_back(10000);
	myVector.push_back(50000);
	myVector.push_back(1000000);
	myVector.push_back(10000000);
	myVector.push_back(100000000);
	myVector.push_back(123456);
	myVector.push_back(9876);

	ret = fileSystemPerformance(36,4,0,116634634, FS_INODE, myVector,fsp);

	if ( ret == -1 )
	{
		FAILED_TEST_RET(31);
		return;
	}

	if ( fsp.space != 125094636 || fabs(fsp.averageAccessNum - 5.142722) > EPSILON)
	{
		FAILED_TEST(31, 125094636, fsp.space, 5.142722, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(36,4,0,116634634, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(31);
		return;
	}

	if ( fsp.space != 125093664 || fabs(fsp.averageAccessNum - 1.737416e05) > 0.01901)
	{
		FAILED_TEST(31, 125093664, fsp.space, 1.737416e05, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(31);
}


/*
 * Did you know?
 * The human eye can see only 1% of its surrounding.
 */
void test_32()
{
	int ret;
	myVector.clear();
	myVector.push_back(12);
	myVector.push_back(68);
	myVector.push_back(1000);
	myVector.push_back(0);
	myVector.push_back(0);
	myVector.push_back(5000);
	myVector.push_back(10000);
	myVector.push_back(1234);
	myVector.push_back(0);
	myVector.push_back(5678);

	ret = fileSystemPerformance(60,12,0,25000, FS_INODE, myVector,fsp);

	if ( ret == -1 )
	{
		FAILED_TEST_RET(32);
		return;
	}

	if ( fsp.space != 29880 || fabs(fsp.averageAccessNum - 3.619392) > EPSILON)
	{
		FAILED_TEST(32, 29880, fsp.space, 3.619392, fsp.averageAccessNum);
		return;
	}

	ret = fileSystemPerformance(60,12,0,25000, FS_LINKED_LIST, myVector,fsp );
	if ( ret == -1 )
	{
		FAILED_TEST_RET(32);
		return;
	}

	if ( fsp.space != 29160 || fabs(fsp.averageAccessNum - 34.78146) > EPSILON)
	{
		FAILED_TEST(32, 29160, fsp.space, 34.78146, fsp.averageAccessNum);
		return;
	}

	PASSED_TEST(32);
}



/*
 * Did you know?
 * Writing tests of OS can make one go mental.
 */
void test_33()
{
	int ret;
	myVector.clear();
	myVector.push_back(7);

	ret = fileSystemPerformance(12,6,0,48, FS_INODE, myVector,fsp);
	if ( ret != -1 )
	{
		cout << "Failed Test: 33" << endl;
		cout << "/tFunction should have return -1" << endl;
		return;
	}

	PASSED_TEST(33);
}



int main()
{
	//Linked List structure
	/*test_1();
	test_2();
	test_3();
	test_4();
	test_5();
	test_6();
	test_7();
	test_8();
	test_9();*/
	test_10();
   	test_11();
    	test_12();

	//I Node structure
	test_13();
	test_14();
	test_15();
	test_16();
	test_17();
	test_18();
	test_19();
	test_20();

	//Advance, both
	test_21();
	test_22();
	test_23();
	test_24();
	test_25();
	test_26();
	test_27();
	test_28();
	test_29();
	test_30();
	test_31();
	test_32();
	test_33();

	return 0;
}



