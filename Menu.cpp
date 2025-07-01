#include "Menu.h"
#define _CRT_SECURE_NO_WARNINGS

/*
	Menu.cpp : Implementation file of Minu object declared
	in Menu.h
*/

/*  Constructors  */
Menu::Menu() {
	exMenu = false;
	setFiles();
	levels = 0;

	includeHeaderLine = false;

	//mainMenu(0);
	//called from main instead
}


/*  Other Functions  */
void Menu::mainMenu(int opt)
{
	std::cout << "\nOpt is: " << opt << endl;

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
				<< "2 - Config Header/Footer" << std::string(5, ' ')
				<< "3 - Config Debugger" << std::string(5, ' ')
				<< "4 - Exit" << endl;
			break;

		case 1:
			//Read Files
			initConfigs();
			readFiles();

			try {
				aggregate();
			}
			catch (const std::exception& e1) {
				cout << "Unkown Exception caught in aggregate:   " << e1.what() << endl;
				cout << "Proceeding to write files to " << writeFile << endl;
			}
			
			writeFiles();

			exMenu = true;
			cout << "Finished Writing Files\n Press any Key to exit program...\n";
			cin.clear();
			char a; cin.ignore(10, '\n');  cin.get(a);

			break;

		case 2:
			//Config Header/Footer
			initConfigs();
			configHF();
			break;

		case 3:
			//Config debugger
			configOutputs();
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
		std::cout << "could not open file: " << foe1.what() <<
			" program ending, please ensure file exists and restart program" << endl;
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

void Menu::initConfigs(const bool readOut)
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


		std::string brker = "Output File Header:";
		readThrough(inStream, brker);
		if (!std::getline(inStream, headerLine))
		{
			cout << "Designator line for file header: '" << brker << 
				"' not found -- no header will be printed,\n closing and reopening file: " << SectionsFile;
			inStream.close();

			inStream.open(SectionsFile);
		};
		//Read in our output file header line

		cout << "Number of levels: " << levels << endl;

		for (size_t i = 0; i != levels; i++) {
			hf_config newConfig(inStream, i+1, readOut);
			configs.push_back(newConfig);
		}
		//configures the header/footer information
		//and saves it in our configs vector
		//For AxysReader there are TWO (2) levels

		//cout << "stream failed: " << inStream.fail() << endl;
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

void Menu::configOutputs()
{
	std::cout << "Debugger output levels are organized into levels of verbosity" << endl;
	std::cout << "Please select an option: " << endl
		<< "0 - Minimum output" << std::string(5, ' ')
		<< "1 - Some output" << std::string(5, ' ')
		<< "2 - Verbose output" << endl;

	int choice = 0;
	bool exit = false;

	while (!exit) {

		if (!(cin >> choice) || choice > 2) {
			cin.clear();
			cout << "Not an option, please choose between 0 and 2" << endl;
			choice = 0;
		}
		else {
			cin.ignore(1);
			exit = true;
		}

	}
	cout << "Output Configured\n\n\n\n\n\n";

	bool arr[3] = { false, false, false };

	for (size_t i = 0; i != 3; i++) {
		if (i == choice)
			arr[i] = true;
	} 

	AR::output.lvl_0 = arr[0];
	AR::output.lvl_1 = arr[1] || arr[2];
	AR::output.lvl_2 = arr[2];
}

void Menu::configHF()
{
	std::cout << "\n\nThis section of the program will take you through the "
		"steps to configure the header/footer manually." << endl << endl;


	std::cout << "Please open and view the 'Sections.txt' file located at: " 
		<< SectionsFile << endl << endl;

	std::cout << "The number at the top indicates the total number of 'levels'"
		<< ", or Section/subsections, there are surrounding " << endl
		<< "the datarows. \n";
		
	std::cout << "You can scroll down to see the section header/footers "
		<< "between the '&&&&&' signs in the file" << endl;

	std::cout << "Level 1 - the innermost layer \n";
	std::cout << "Level 2 - the outer layer, and so on \n";

	std::cout << "---------------------------------------------------------\n";
	std::cout << std::string(16, ' ') << "Press Enter to continue\n";
	std::cout << "---------------------------------------------------------\n";

	getchar();

	std::cout << "To alter an existing header/footer, you need to save a file with"
		<< " your new header footer as a \n .CSV (comma delimited) file and "
		<< "copy/paste the entire header/footer between the existing \n ampersand signs,"
		<< "be careful not to delete the ampersand signs, they are NOT part\n"
		<< "of the header and merely exist as guidlines for the code\n\n";

	std::cout << "NOTE: It is important that any BLANK lines between sections be "
		<< "included in your header/footer,\n but please ensure blank lines never"
		<< "START a header section, and only start a footer\n section IF it is only "
		<< "blank lines that delineate one section from the next, otherwise they\n"
		<< "should always be trailing elements of header/footer section\n\n";

	std::cout << "NOTE 2: The first line of the footer is known as the STOPPER "
		<< "and is what tells the program when to stop \n reading subsections or "
		<< "datarows.  This first line MUST BE GENERAL TO THE FIRST LINE OF\n ALL "
		<< "FOOTERS so that the program always knows where to stop reading datarows\n\n";

	std::cout << "---------------------------------------------------------\n";
	std::cout << std::string(16, ' ') << "Press Enter to continue\n";
	std::cout << "---------------------------------------------------------\n";

	getchar();

	std::cout << "To disable or enable a header/footer, simply switch the single"
		<< " digit above the header/footer\n section (should be a 0 or 1)"
		<< "to a 0 (program reads no header/footer), or 1 respectively. \n"
		<< "Once again, do NOT delete the '&&&&&' symbols\n\n";


	std::cout << "The program will now read out and configure the new header/footers\n\n";

	std::cout << "---------------------------------------------------------\n";
	std::cout << std::string(16, ' ') << "Press Enter to continue\n";
	std::cout << "---------------------------------------------------------\n";

	initConfigs(true);

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

		//readFile = "db1c_6.csv";

		if (readFile == "")
		{
			string noArg = "/t /t No filename submitted ";
			noArg += "please enter full filename of type csv, e.g.: db1.csv";
			throw missing_arguments(noArg);
		}

		cout << "Entered Read File name: " << readFile << endl << endl;

		ifstream inStream(readFile);
		std::string line;
		std::istringstream ssLine;
		/*
			defines stream object and
			opens file at the same time
		*/


		if (AR::output.lvl_2) {
			cout << "\n\nEntering read data while loop with debugger output level 2:\n\n"
				<< "You will have to parse through sections with the enter key" << endl <<
				"Alternatively, spam a number of charcters into the cmdLine and press <Enter> "
				<< endl << "to run many lines at once     " << "Press enter to continue" << endl;

			getchar();
			getchar();
			cout << "\n\n--------------------------------------------------------\n\n";
		}

		while (inStream.good())
		{
			//generally its bad practice to use eof, 
			//but it suits our needs here
			
			try
			{
				Section s = Section(&configs, levels);
				inStream >> s;

				wrapper.push_back(s);
			}
			catch (const std::out_of_range& ia1) {
				cout << "invalid argument caught in Menu::Reads, while loop:  " <<
					ia1.what() << endl;
			}
			catch (const std::exception& e1) {
				//Error handling
				cout << "Unkown exception caught in Menu::ReadFile, while loop:  " <<
					e1.what() << endl;

				throw e1;
			}
			
		}

		inStream.close();

		if (wrapper.size() == 0) {
			std::string err = "No data read from " + readFile;
			throw file_open_error(err);
		}
	}
	catch (missing_arguments& ma1) {
		//Error handling
		cout << "Logic error caught in Menu::ReadFile: " << endl;
		cout << endl << ma1.what() << endl;
	}
	catch (file_open_error& foe1) {
		//Error handling
		cout << "File open error caugh in Menu::ReadFile, PLEASE ENSURE the file is saved\n" << 
			" as a .csv file and you include the file extension with the file name" << endl;
		cout << endl << foe1.what() << endl;

		throw file_open_error(readFile);
	}
	catch (std::bad_alloc& ba1) {
		std::cout << "Caught bad alloc exception in Menu::readFiles "
			<< "Proceeding to write rows out and exit ";
		throw ba1;
	}
	catch (const std::exception& e1) {
		//Error handling
		cout << "Unkown exception caught in Menu::ReadFile  " <<
			e1.what() << endl;;
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
	//gets user sp500 value

	char c = 'n';
	std::cout << "Include header line? (y/n): ";
	std::cin >> c;

	if (c == 'y')
		includeHeaderLine = true;
	
	
	//Establish Date
	std::time_t now_c = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &now_c);

	std::stringstream time;
	time << std::put_time(&localTime, "%m/%d/%Y");

	/*
	std::time_t const now_c = std::time(NULL);
	auto localTime = std::localtime(&now_c);
	std::stringstream time;

	time << std::put_time(localTime, "%m/%d/%Y");
	*/

	/*
		We instantiate use of threads to help the program run faster,
		this shouldnt be modified
	*/

	int threadNumTBD = 1;
	/*
	if (wrapper.size() < 18 || AR::output.lvl_1)
		threadNumTBD = 1;
	else
		threadNumTBD = 9;
	*/

	int totThreads = threadNumTBD; // threadNumTBD;
	std::vector<std::thread> threads;

	std::vector<Section>::iterator start;
	std::vector<Section>::iterator end;

	std::vector<int> startNums;
	std::vector<int> endNums;
	startNums.push_back(0);

	for (size_t i = 1; i != totThreads; i++) {
		int num = (wrapper.size() * i) / totThreads;
		endNums.push_back(num);

		startNums.push_back(endNums.at(i - 1));
	}

	endNums.push_back(wrapper.size());
	
	if (AR::output.lvl_1) {
		cout << "Entering " << totThreads << " threads for aggregate function"
			<< endl << endl;
	}

	for (size_t i = 0; i != totThreads; i++) {

		start = wrapper.begin() + startNums[i];
		end = wrapper.begin() + endNums[i];
					
		std::thread t1{ &Menu::aggregateTask, this,
			start, end, std::ref(time), std::ref(sp500) };

		threads.push_back(std::move(t1));
	}

	for (size_t i = 0; i != totThreads; i++) {
		threads.at(i).join();
	} 
}

void Menu::aggregateTask(const std::vector<Section>::iterator start,
	const std::vector<Section>::iterator end,
	const std::stringstream& time, const double& sp500)
{
	//cout << "Entering thread ID: " << std::this_thread::get_id() << endl;


	for (auto iter = start; iter != end; ++iter)
	{
		SectionVals sv;
		sv.nowDate = time.str();
		sv.sp_current = sp500;

		iter->aggregateSecs(lookupFiles, sv);
	}
}


void Menu::writeFiles()
{
	std::ofstream output(writeFile);

	if (includeHeaderLine)
		output << headerLine << endl;
	

	for (auto i : wrapper) {
		output << i;
	}

	DataRow row;
	cout << "Total writes: " << row.getTotalWrites() << endl;
}

std::ifstream& Menu::readThrough(std::ifstream& is, const std::string& brk)
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


