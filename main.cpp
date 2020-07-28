#include "main.h"
#define _CRT_SECURE_NO_WARNINGS

//Implementation file of main.h AxysReader c++ program


/*
	The MainMenu function of our Menu object is responsible for running the program:
		- Defining the user's options and allowing them to navigate the code
			from the command line

		-The user stays in the program until the menu.exit() function is
			updated to true, either by the user or by the program



*/



int main(int argc, const char* argv[])
{
	Menu menu;
	int choice = 0;

	try
	{
		menu.mainMenu(choice);

		while (!menu.exit()) {
			
			bool opt = !(cin >> choice);

			if (opt) {
				menu.mainMenu(5);
				cout << endl;

				cin.clear();
				cin.ignore(10, '\n');
			}
			else {
				cin.ignore(1, '\n');
				menu.mainMenu(choice);
			}


			if (!menu.exit())
				menu.mainMenu(0);
		}
		
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in Main, program exiting";
		getchar();
	}
	
	if (!menu.exit()) 
	{
		cout << "Press Enter to Exit Program\n";
		getchar();
	}
	
	return 0;
}
