#include "main.h"

//Implementation file of main.h AxysReader c++ program

int main(int argc, const char* argv[])
{
	//Lets get a name for the dile and open it up via cmd line
	std::string fileName = "";

	try
	{
		//initialize all static member variables in our objects from files
		initObjects();
		
		

		if (inStream)
		{
			std::vector<Section> dRows;
			
			for (int i = 0; i < 3; i++) {
				Section row;

				inStream >> row;
				dRows.push_back(row);
			}

			

			//Read Data Back
			cout << endl << endl << "READ DATA BACK: " << endl << endl;
			
			std::ofstream output("HOLDNGS.csv");
			//declare output File Stream

			for (auto i : dRows) {
				output << i << endl;
			}

		}
		else
			cout << "Not in stream" << endl;
		
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in Main, program exiting";
	}


	getchar();

	return 0;
}



void initObjects()
{
	try
	{
		Section sub;
		std::cout << "Initing subs now: " << std::endl << endl;
		sub.initSubs();

	}
	catch (missing_arguments& ma)
	{
		std::cout << "Missing arguments exception caught in " 
			<< "initObjects, rethrowing: " << endl;
		throw ma;
	}
}

void loadData()
{




}
