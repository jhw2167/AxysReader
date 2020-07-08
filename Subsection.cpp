#include "Subsection.h"

/*
	Implementation file for Class Subsection
*/


/*  Initiate Static variables - shared by all instances of this class  */
const std::string Subsection::breaker = "&&&&&";
const std::string Subsection::fileName = "subsectionConfig.txt";

int Subsection::headLength = 0;
int Subsection::footLength = 0;

bool Subsection::hasHeader = false;
bool Subsection::hasFooter = false;

std::vector<std::string> Subsection::keywords;

/*  Constructor  */
Subsection::Subsection() {
	numRows = 0;
}


/*  Initializers  */

void Subsection::initSubs()
{
	std::string breaks[3] = { "Subsection Header:", "KEYWORDS:",
		"Subsection Footer:" };
	//Set filename and "breaks" where the program reads
	//its init information from the program

	std::ifstream toRead(fileName);
	//Declare and open file to read from


	
	std::cout << "Initializing Subsections from: " <<
		fileName << std::endl;
	


	//Read through file until first "break" 
	//indicates we have arrived at our header
	readThrough(toRead, breaks[0]);
	initHeader(toRead);


	//Read through file until we arrive at 
	//second break
	readThrough(toRead, breaks[1]);
	initKeywords(toRead);


	//And third break, our footer section 
	readThrough(toRead, breaks[2]);
	initFooter(toRead);

}

void Subsection::initHeader(std::ifstream& is)
{
	//First read in bool that inidicates if we HAVE a header
	is >> hasHeader;

	if (!hasHeader)
		return;

	std::string val;
	std::getline(is, val);

	try
	{
		//Try and catch blocks help handle errors

		if (val != breaker) {
			std::string err = "Critial error in Subsection::initHeader, ";
			throw missing_arguments(err);
		}

		//now get next line and read in header from the file
		//until we hit the breaker
		std::getline(is, val);
		while ((val != breaker)  && std::getline(is, val)) {
			headLength++;
		}

		if (val != breaker) {
			std::string err = "Critial error in Subsection::initHeader, ";
			throw missing_arguments(err);
		}
	}
	catch (missing_arguments& ma)
	{
		std::string err = ma.what();
		err += fileName;
		err += "file lacking critical \n breaker symbols: ";
		err += breaker;
		err += "  That surround the header and footer bodies, please correct or configure ";
		err += "with built in configurer.";

		throw ma;
	}
}

void Subsection::initFooter(std::ifstream& is)
{
	//First read in bool that inidicates if we HAVE a header
	is >> hasFooter;

	if (!hasFooter)
		return;

	std::string val;
	std::getline(is, val);

	try
	{
		if (val != breaker) {
			std::string err = "Critial error in Subsection::initHeader, ";
			throw missing_arguments(err);
		}

		//now get next line and read in footer from the file
		//until we hit the breaker ( "&&&&&"
		std::getline(is, val);
		while (val != breaker && std::getline(is, val)) {
			footLength++;
		}

		if (val != breaker) {
			std::string err = "Critial error in Subsection::initHeader, ";
			throw missing_arguments(err);
		}
	}
	catch (missing_arguments& ma)
	{
		std::string err = ma.what();
		err += fileName;
		err += "file lacking critical \n breaker symbols: ";
		err += breaker;
		err += "  That surround the header and footer bodies, please correct or configure ";
		err += "with built in configurer.";

		throw ma;
	}

}

void Subsection::initKeywords(std::ifstream& is)
{
	int numLines = 1;
	//Number of lines of keywords we have to read, by default 1,
	//this number will be read in from "subsectionsConfig.txt" if 
	//there are multiple lines of keywords
	is >> numLines;
	
	std::string discard;
	std::getline(is, discard);
	//discard the rest of the instructions on the line under KEYWORD

	for (size_t i = 0; i != numLines; i++)
	{
		std::string val;
		std::getline(is, val);
		//Line contains all of our section keywords
		std::stringstream ssLine(val);

		while (std::getline(ssLine, val, ',')) {
			keywords.push_back(val);
		}
	}
	
}



/*  Modifiers  */
/*  Accessors  */

/*  Other Functions  */
void Subsection::addRow(const DataRow &r) {
	rows.push_back(r);
}

const bool Subsection::containsKeyword(const std::string &sample) const
{
	/*
		We check string "sample" for any of the subsection 
		keywords in our "keywords" vector (stored in
		subsectConfig.txt)
	*/

	bool found = false;

	for (auto i : keywords) {
		found = (sample.find(i) != std::string::npos);
	}

	return found;
}

std::ifstream& Subsection::readThrough(std::ifstream& is, std::string& brk)
{
	/*
		Method simply reads through the file until it hits
		and established breakpoint (string) I expect to
		find within the file
	*/

	std::string comp;
	while (getline(is, comp)) {
		if (comp == brk)
			break;
	}

	return is;
}

/*  Destructor  */