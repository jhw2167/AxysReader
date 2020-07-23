#include "hf_config.h"

/*
	Implementation file for hf_config class
*/

/*  Initiate Static variables - shared by all instances of this class  */
const std::string hf_config::breaker = "&&&&&";

	
	/*  Constructor  */

//Init from hf_config object
hf_config::hf_config(hf_config& hf, int lvl) {
	setMembers(hf);
	level = lvl;
}

//Init from file
hf_config::hf_config(std::ifstream& toRead, int lvl,
	const bool readOut) {
	level = lvl;
	config(toRead, readOut);
}

//End constructors


/*  Initializers  */
void hf_config::config(std::ifstream& toRead, const bool readOut)
{
	std::string breaks[3] = { "Section Header:", "KEYWORDS:",
		"Section Footer:" };
	//Set filename and "breaks" where the program reads
	//its init information from the program

	//read through document until we get to proper level... 1, 2...
	std::string lvlString = "Level: " + std::to_string(level);
	readThrough(toRead, lvlString);

	if (readOut) {
		std::cout << "Reading section header/footer in for level: "
			<< level << "\n\n";
	}
		
	//Read through file until first "break" 
	//indicates we have arrived at our header
	readThrough(toRead, breaks[0]);
	initHeader(toRead, readOut);
	cout << endl;

	//Read through file until we arrive at 
	//second break
	readThrough(toRead, breaks[1]);
	initKeywords(toRead);


	//And third break, our footer section 
	readThrough(toRead, breaks[2]);
	initFooter(toRead, readOut);
	cout << endl;

	if (readOut) {
		std::cout << "Finished level: "
			<< level << "\n\n";
		std::cout << std::string(30, '-') << endl;
	}

}

void hf_config::initHeader(std::ifstream& is, const bool readOut)
{

	//First read in bool that inidicates if we HAVE a header
	is >> hasHeader;

	if (readOut)
		std::cout << "Section has header: " << hasHeader << endl;

	if (!hasHeader)
		return;

	std::string val;
	std::getline(is, val);

	try
	{
		//Try and catch blocks help handle errors

		std::getline(is, val);
		//store leading block of header in val, should be "&&&&&"

		if (val != breaker) {
			std::string err = "Critial error in hf_config::initHeader(top), ";
			throw missing_arguments(err);
		}

		//now get next line and read in header from the file
		//until we hit the breaker
		while (std::getline(is, val) && val != breaker) {

			if (readOut)
				cout << val << endl;

			headLength++;
		}

		if (val != breaker) {
			std::string err = "Critial error in hf_config::initHeader (bottom), ";
			throw missing_arguments(err);
		}
	}
	catch (missing_arguments& ma)
	{
		std::string err = ma.what();
		err += fileName;
		err += "file lacking critical \n breaker symbols: ";
		err += breaker;
		err += "  That surround the header body, please correct or configure ";
		err += "with built in configurer.";

		throw missing_arguments(err);
	}

	return;
}

void hf_config::initFooter(std::ifstream& is, const bool readOut)
{
	//First read in bool that inidicates if we HAVE a header
	is >> hasFooter;

	if (readOut)
		std::cout << "Section has footer:" << hasFooter << endl;

	if (!hasFooter)
		return;

	std::string val;
	std::getline(is, val);

	try
	{

		std::getline(is, val);
		//store leading block of footer in val, should be "&&&&&"

		if (val != breaker) {
			std::string err = "Critial error in Section::initFooter(top), ";
			throw missing_arguments(err);
		}

		//now get next line and read in footer from the file
		//until we hit the breaker ( "&&&&&" )
		std::getline(is, val);
		stopper = val;
		footLength++;
		//stopper must be equal to first line of footer

		if (readOut)
			cout << val << endl;

		while (std::getline(is, val) && val != breaker) {

			if (readOut)
				cout << val << endl;

			footLength++;
		}

		if (val != breaker) {
			std::string err = "Critial error in Section::initFooter(bottom), ";
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

		throw missing_arguments(err);
	}

}

void hf_config::initKeywords(std::ifstream& is)
{
	int numLines = 1;
	//Number of lines of keywords we have to read, by default 1,
	//this number will be read in from "SectionsConfig.txt" if 
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
void hf_config::setMembers(hf_config& hf)
{
	fileName = hf.fileName;
	keywords = hf.keywords;
	stopper = hf.stopper;

	headLength = hf.headLength;
	footLength = hf.footLength;

	hasHeader = hf.hasHeader;
	hasFooter = hf.hasFooter;

}
//end Modifiers


/*  Othe Functions  */
std::ifstream& hf_config::readThrough(std::ifstream& is, std::string& brk)
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
