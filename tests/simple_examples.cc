#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sqlite_modern_cpp.h>
using namespace  sqlite;
using namespace std;

struct TmpFile
{
	string fname;

	TmpFile()
	{
		char f[]="/tmp/sqlite_modern_cpp_test_XXXXXX";
		int fid = mkstemp(f);
		close(fid);

		fname  = f;
	}

	~TmpFile()
	{
		unlink(fname.c_str());
	}
};

int main()
{
	try
	{
		TmpFile file;
		database db(file.fname);

		db << "CREATE TABLE foo (a integer, b string);";
		db << "INSERT INTO foo VALUES (?, ?)" << 1 << "hello";
		db << "INSERT INTO foo VALUES (?, ?)" << 2 << "world";

		string str;
		db << "SELECT b from FOO where a=?;" << 2 >> str;

		if(str != "world")
		{
			cout << "Bad result on line " << __LINE__ << endl;
			exit(EXIT_FAILURE);
		}

		std::string sql("select 1+1");
		int test = 0;
		db << sql >> test;
		
		if(test != 2) exit(EXIT_FAILURE);
		
	}
	catch(sqlite_exception e)
	{
		cout << "Unexpected error " << e.what() << endl;
		exit(EXIT_FAILURE);
	}
	catch(...)
	{
		cout << "Unknown error\n";
		exit(EXIT_FAILURE);
	}

	cout << "OK\n";
	exit(EXIT_SUCCESS);
}
