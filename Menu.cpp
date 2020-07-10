#include "Menu.h"

/*
	Menu.cpp : Implementation file of Minu object declared
	in Menu.h
*/

/*  Constructors  */
Menu::Menu() {
	exMenu = false;
	configFile = "Sections.txt";
	levels = 0;

	mainMenu(0);
}


/*  Other Functions  */
void Menu::mainMenu(short opt)
{
	switch (opt)
	{
	case 1:
		//Read Files
		initConfigs();
		readFiles();
		break;

	case 2:
		//Config Header

		break;

	case 3:
		//Config Footer

		break;

	case 4:
		//exit
		exMenu = true;
		break;

	default:
		std::cout << "Welcome to Axys Reader, this program reads provided " <<
			"csv files into a desirable format \n" << "What would you like " <<
			" to do: "
			<< "1 - Read Files" << std::setw(5)
			<< "2 - Config Header" << std::setw(5)
			<< "3 - Config Footer" << std::setw(5)
			<< "4 - Exit" << std::setw(5);

		break;
	}

}


/*  Private functions  */
void Menu::initConfigs()
{
	try
	{
		std::cout << "Opening config file: " << configFile << endl;
		ifstream inStream(configFile);

		inStream >> levels;

		for (size_t i = 0; i < levels; i++) {
			hf_config newConfig(inStream);
		}
	}
	catch (file_open_error& foe1) {
		//Error handling
		cout << "File open error caugh in Menu::initConfigs: " << endl;
		cout << endl << foe1.what() << endl;
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in Menu::initConfigs";
	}


}


void Menu::readFiles()
{
	std::cout << "Please move file to current directory " <<
		"and enter full filename, e.g.: newData.csv " << endl;

	try
	{
		//Try block is for error handling

		std::cin >> readFile;
		//reads infile name entered by the user

		readFile = "db3c.csv";

		if (readFile == "")
		{
			string noArg = "/t /t No filename submitted ";
			noArg += "please enter full filename of type csv, e.g.: db1.csv";
			throw missing_arguments(noArg);
		}

		cout << "File name: " << readFile << endl << endl;

		ifstream inStream(readFile);
		/*
			defines stream object and
			opens file at the same time
		*/
	}
	catch (missing_arguments& ma1) {
		//Error handling
		cout << "Logic error caught in Menu::ReadFile: " << endl;
		cout << endl << ma1.what() << endl;
	}
	catch (file_open_error& foe1) {
		//Error handling
		cout << "File open error caugh in Menu::ReadFile: " << endl;
		cout << endl << foe1.what() << endl;
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in Menu::ReadFile";
	}
	


}


