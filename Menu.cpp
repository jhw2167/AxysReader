#include "Menu.h"

/*
	Menu.cpp : Implementation file of Minu object declared
	in Menu.h
*/

/*  Constructors  */
Menu::Menu() {
	exMenu = false;
	configFile = "configs/Sections.txt";
	writeFile = "outputs/HOLDNGS.CSV";
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
		cout << "Please select another menu option: " << endl;
	}
	catch (...)
	{
		cout << "Unknown exceoption caught in main menu" << endl;
		throw;
	}
	

}

const bool Menu::exit() const {
	return exMenu;
}


/*  Private functions  */
void Menu::initConfigs()
{
	try
	{
		//std::cout << "Opening config file: " << configFile << endl;
		std::ifstream inStream(configFile);
		//configFile is initialized to filename "Sections.txt" in
		//the constructor of this class, if that file is lost or
		//the file name is changed, the program will malfuntion

		if (inStream.fail()) {
			std::string err = "unable to find config file: " + configFile +
				" in program directory" + '\n';
			throw file_open_error(err);
		}

		inStream >> levels;
		//reads in the number of "levels" we have, that is the number
		//of distinct header/footer pairs to deal with

		cout << "Number of levels: " << levels << endl;
		std::string breaker = "LOOKUP FILES:";

		readThrough(inStream, breaker);
		lookupFiles.readFileNames(inStream);

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

		readFile = "db4c.csv";

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

			/*
						NON-COPY CONSTRUCTOR
			Section* newSec = new Section(&configs, levels);
			inStream >> *newSec;
			
			*/
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
	


}

void Menu::aggregate()
{
	/*
		Here we direct methods that aggregate the "reads" vector columns
		into the "writes" vector
	*/

	//Establish Lookups
	
	
	//Establish Date
	std::time_t const now_c = std::time(NULL);
	auto localTime = std::localtime(&now_c);
	std::stringstream time;

	time << std::put_time(localTime, "%m/%d/%Y");

	for (auto& section : wrapper)
	{
		SectionVals sv;
		sv.nowDate = time.str();

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


