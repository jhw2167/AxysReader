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
	try
	{
		Menu menu;
		static int count = 0;
		int choice = 0;

		while (!menu.exit()) {
			std::cin >> choice;
			menu.mainMenu(choice);
		}
		
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in Main, program exiting";
	}
	

	getchar();

	return 0;
}
