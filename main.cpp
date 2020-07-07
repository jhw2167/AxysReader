#include "main.h"

//Implementation file of main.h AxysReader c++ program

int main(int argc, const char* argv[])
{
	//Lets get a name for the dile and open it up via cmd line
	string fileName = "";

	cout << "Please move file to current directory " <<
		"and enter full filename, e.g.: db1.csv " << endl;

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

		// define ifstream object
		ifstream inFile(fileName);
		//defines stream and opens it at the same time

	}
	catch (missing_arguments& ma1) {
		cout << "Logic error caught in main: " << endl;
		cout << endl << ma1.what() << endl;
	}

	catch (file_open_error& foe1) {
		cout << "File open error caugh in main: " << endl;
		cout << endl << foe1.what() << endl;
	}
	catch (...) {
		cout << "Unkown exception caught in main";
	}

	return 0;
}


void loadData()
{




}
