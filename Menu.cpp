#include "Menu.h"

/*
	Menu.cpp : Implementation file of Minu object declared
	in Menu.h
*/

/*  Constructors  */
Menu::Menu() {
	exMenu = false;
	setFiles();
	levels = 0;

	mainMenu(0);
}


/*  Other Functions  */
void Menu::mainMenu(int opt)
{
	std::cout << "Opt is: " << opt << endl;

	try
	{
		switch (opt)
		{
		case 0:
			//Read Menu
			std::cout << "Welcome to Axys Reader, this program reads provided " <<
				"csv files into a desirable format \n" << "What would you like " <<
				" to do: " << endl
				<< "1 - Read Files" << std::string(5, ' ')
				<< "2 - Config Header" << std::string(5, ' ')
				<< "3 - Config Footer" << std::string(5, ' ')
				<< "4 - Exit" << endl;
			break;

		case 1:
			//Read Files
			initConfigs();
			readFiles();

			aggregate();
			writeFiles();
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
			cout << endl << "That is not an option, please select a number 1-4" << endl;

			break;
		}
	}
	catch (file_open_error &foe1) {
		std::cout << "could not open file, " << foe1.what() <<
			" exception rethrowing and program ending " << endl;
		throw foe1;
	}
	catch (std::out_of_range& or1) {
		std::cout << "Out of range eror: " << endl;
		std::cout << or1.what() << endl;
	}
	catch (...)
	{
		cout << "Unknown exception caught in main menu" << endl;
		throw;
	}
	
}

const bool Menu::exit() const {
	return exMenu;
}


/*  Private functions  */
void Menu::setFiles()
{
	/*
		One core file "configs/lookups.txt" holds all the core file names
		for the rest of the program, this method will setUp all those files
	*/
	std::string coreFile = "configs/Lookups.txt";
	std::cout << "Scanning for file:  " << coreFile << "  to setUp program" << endl;

	std::ifstream inFile;

	do
	{
		bool runMenu = false;
		try {
			inFile.open(coreFile);
		}
		catch (const file_open_error& foe1)
		{
			setFileHelp(coreFile);
			runMenu = true;
		}

		if (!runMenu && !inFile)
			setFileHelp(coreFile);

	} while (!inFile && !exMenu);
	
	
	if (!inFile) {
		return;
	}
	
	std::string line;
	std::string discard;

	std::string breaker = "LOOKUP FILES (from directory with AxysReader.exe):";
	readThrough(inFile, breaker);
	lookupFiles.readFileNames(inFile);


	std::getline(inFile, discard, ':');
	inFile >> SectionsFile;
	//get Section header_footer config file

	cout << "sections File: " << SectionsFile << endl << endl;

	std::getline(inFile, discard, ':');
	inFile >> readFile;
	//get read file

	std::getline(inFile, discard, ':');
	inFile >> writeFile;
	//get write file

	std::getline(inFile, discard, ':');
	inFile >> flagsFile;
	//get error flag config file

	//Close File
	inFile.close();

}

void Menu::setFileHelp(std::string& coreFile)
{
	/*
		Presents menu options for setFiles() method to assist with file errors
		in opening the main loading file
	*/

	//Start with our menu;

	std::cout << "Problem finding or opening file: " << coreFile << endl;
	std::cout << "Please create the file, or enter a new filename";
	std::cout << "Please note this is NOT the Axys report File" << endl << endl;
	std::cout << "Please enter: "
		<< "0 - Try again with same file" << std::string(5, ' ')
		<< "1 - Change File Name" << std::string(5, ' ')
		<< "2 - Exit program" << endl;

	//get option and run case:
	char c;
	std::cin >> c;

	std::cout << endl;
	switch (c)
	{
	case 0:
		break;

	case 1:
		std::cout << "Please enter new file name: ";
		std::cin >> coreFile;
		std::cout << endl << endl;
		break;

	default:
		exMenu = true;
		break;
	}

	return;
}

void Menu::initConfigs()
{
	try
	{
		//std::cout << "Opening config file: " << configFile << endl;
		std::ifstream inStream(SectionsFile);
		//configFile is initialized to filename "Sections.txt" in
		//the constructor of this class, if that file is lost or
		//the file name is changed, the program will malfuntion

		if (inStream.fail()) {
			std::string err = "unable to find config file: " + SectionsFile +
				" in program directory" + '\n';
			throw file_open_error(err);
		}

		inStream >> levels;
		//reads in the number of "levels" we have, that is the number
		//of distinct header/footer pairs to deal with

		cout << "Number of levels: " << levels << endl;

		for (size_t i = 0; i != levels; i++) {
			hf_config newConfig(inStream, i+1);
			configs.push_back(newConfig);
		}
		//configures the header/footer information
		//and saves it in our configs vector
		//For AxysReader there are TWO (2) levels

		cout << "stream failed: " << inStream.fail() << endl;
	}
	catch (file_open_error& foe1) {
		//Error handling
		cout << "File open error caugh in Menu::initConfigs: " << endl;
		cout << endl << foe1.what() << endl;
		throw foe1;
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

		readFile = "db1c_6.csv";

		if (readFile == "")
		{
			string noArg = "/t /t No filename submitted ";
			noArg += "please enter full filename of type csv, e.g.: db1.csv";
			throw missing_arguments(noArg);
		}

		cout << "File name: " << readFile << endl << endl;

		ifstream inStream(readFile);
		std::string line;
		std::istringstream ssLine;
		/*
			defines stream object and
			opens file at the same time
		*/

		while (!inStream.eof())
		{
			//cout << "Reading Top Stream section from readFiles() " << endl << endl;

			//generally its bad practice to use eof, 
			//but it suits our needs here

			Section s = Section(&configs, levels);
			inStream >> s;

			wrapper.push_back(s);
		}

		inStream.close();
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
	catch (std::bad_alloc& ba1) {
		std::cout << "Caught bad alloc exception in Menu::readFiles "
			<< "Proceeding to write rows our and exit ";

		throw ba1;
	}
	catch (...) {
		//Error handling
		cout << "Unkown exception caught in Menu::ReadFile";
	}
	
	DataRow row;
	cout << "Total reads: " << row.getTotalReads() << endl;
}

void Menu::aggregate()
{
	/*
		Here we direct methods that aggregate the "reads" vector columns
		into the "writes" vector
	*/

	//Get current S&P Value from user
	double sp500 = -1;

	std::cout << "Please enter current S&P 500 value: ";
	std::cin >> sp500;
	
	
	//Establish Date
	std::time_t const now_c = std::time(NULL);
	auto localTime = std::localtime(&now_c);
	std::stringstream time;

	time << std::put_time(localTime, "%m/%d/%Y");

	cout << "Lookup files val_sp: " << lookupFiles.val_SP << endl;

	for (auto& section : wrapper)
	{
		SectionVals sv;
		sv.nowDate = time.str();
		sv.sp_current = sp500;

		section.aggregateSecs(lookupFiles, sv);
	}

	
}

void Menu::writeFiles()
{
	std::ofstream output(writeFile);

	for (auto i : wrapper) {
		output << i;
	}
	exMenu = true;

	DataRow row;
	cout << "Total writes: " << row.getTotalWrites() << endl;

}

std::ifstream& Menu::readThrough(std::ifstream& is, std::string& brk)
{
	/*
		Method simply reads through the file until it hits
		and established breakpoint (string) I expect to
		find within the file
	*/

	std::string comp;
	while (getline(is, comp)) {
		if (comp == brk) {
			break;
		}
	}
	return is;
}


