#include "main.h"

//Implementation file of main.h AxysReader c++ program

int main(int argc, const char* argv[])
{
	//Lets get a name for the dile and open it up via cmd line
	string fileName = "";

	cout << "Please move file to current directory " <<
		"and enter full filename, e.g.: newData.csv " << endl;

	cin >> fileName;

	try
	{
		if (fileName == "")
		{
			string noArg = "/t /t No filename submitted ";
			noArg += "please enter full filename of type csv, e.g.: db1.csv";
			throw missing_arguments(noArg);
		}

		cout << "File name: " << fileName << endl << endl;

		ifstream inStream(fileName);
		/*
			defines stream object we can read from and
			opens it at the same time
		*/
		if (inStream)
		{
			DataRow dRow;
			inStream >> dRow;
		}
		else
			cout << "Not in stream" << endl;
		
	}
	catch (missing_arguments& ma1) {
		//Error handling
		cout << "Logic error caught in main: " << endl;
		cout << endl << ma1.what() << endl;
	}

	catch (file_open_error& foe1) {
		//Error handling
		cout << "File open error caugh in main: " << endl;
		cout << endl << foe1.what() << endl;
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in main";
	}


	getchar();

	return 0;
}


void loadData()
{




}
