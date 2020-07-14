#include "Section.h"

/*
	Implementation file for Class Section
*/


/*  Constructor  */
Section::Section() {
	//cout << "dummy constructor " << endl;
	details = nullptr;
	allDetails = nullptr;

	level = 1;
	numRows = 0;
	numSubs = 0;
}

Section::Section(const hf_config* dts, int lvl) {
	initVars(lvl);
	setDetails(dts);
}

Section::Section(const std::vector<hf_config>* aDts, int lvl)
{
	initVars(lvl);
	setAllDetails(aDts);
	setDetails(&aDts->at(lvl - 1));
}



/*  Initializers  */
void Section::initVars(int lvl)
{
	level = lvl;
	//All sections must have a level

	secName = "";
	//not all sections will have a name

	numRows = 0;
	numSubs = 0;
	//initial number of rows and subsections per 
	//section is 0

}



/*  Modifiers  */
void Section::setDetails(const hf_config* dts) {
	details = dts;
}

void Section::setAllDetails(const std::vector<hf_config>* aDts) {
	allDetails = aDts;
}



/*  Accessors  */
const hf_config& Section::getDetails() {
	return *details;
}

std::string Section::getClientName() {
	return clientName;
}



/*  Other Functions  */
void Section::addRow(const DataRow& r) {
	rows.push_back(r);
}

const bool Section::containsKeyword(const std::string& sample) const
{
	/*
		We check string "sample" for any of the Section
		keywords in our "keywords" vector (stored in
		subsectConfig.txt)
	*/

	bool found = false;

	for (auto& i : details->keywords) {
		found = (sample.find(i) != std::string::npos);
	}

	return found;
}

void Section::aggregateSecs(const Lookups& lks, SectionVals& sv) 
{
	if (level > 1)
	{
		//aggregate section values then rows

		//Hold Number
		addHoldNumber(lks.holdNum,sv);
		
		//Total_Port Value
		sv.total_port = summaryVals.at(0);

		//delete Value
		addDeleteValue(lks.holdNum, sv);

		//for subsections
		for (auto& sub : subs) {
			sub.aggregateSecs(lks, sv);
		}

	}
	else {
		aggregateRows(lks, sv);
	}
		
		

	//Build Major Section values

	

	//Call aggRows function

}


void Section::aggregateRows(const Lookups& lks, SectionVals& sv)
{
	//Build Minor Section values
	
	//Cash

	//Fixed Income

	//Equities



	//Call aggregateData for DataRows
	for (auto row : rows) {
		row.aggregate(lks, sv);
	}

}


std::ifstream& Section::readThrough(std::ifstream& is, std::string& brk, char c)
{
	/*
		Method simply reads through the file until it hits
		and established breakpoint (string) I expect to
		find within the file
	*/

	std::string comp;
	while (getline(is, comp, c)) {

		if (comp == brk) {
			break;
		}
	}

	return is;
}


/*  Other Private Functions  */

void Section::readRows(std::istream& is)
{
	std::string line;

	bool failed = false;
	bool readNext = true;
	//initialize variables simply

	numRows = -1;
	//offsets for one extra numRows++ command in 
	//the while loop
	

	while (readNext && !failed) {

		/*
			Read our data into the DataRow Object then
			call addRow function to add it to
			our rows vector for this section.
		*/
		failed = !std::getline(is, line);
		readNext = !(line == details->stopper ||
			containsKeyword(line));
		//If next Line = report footer or header, loop terminates as
		//there are no more rows to read in this section

		if (readNext)
		{
			//don't run this in first loop
			DataRow row;

			std::stringstream ssLine(line);
			ssLine >> row;
			addRow(row);
			numRows++;
		}
	}

}

void Section::readSubsections(std::istream& is)
{
	/*
		While there is a next SECTION to read and while 
		getLine doesnt "fail", reads a bad line, we continue
		to read subsections into this large section.
	*/
	std::string line;
	std::string fullLine;
	//full Line represents full section line

	const int newLevel = level - 1;
	bool readNext = true;
	bool failed = !std::getline(is, line);
	numSubs = -1;


	while (readNext && !failed)
	{

		fullLine = "";
		bool hitStopper = false;
		while (!hitStopper && !failed)
		{
			fullLine = fullLine + line + '\n';
			failed = !std::getline(is, line);
			hitStopper = (line == allDetails->at(newLevel - 1).stopper);
			//this stopper must be a section stopper, must come
			//from one level lower on alldetails vector

			if (hitStopper && !failed)
			{
				//Read inner section footer into the full line
				for (size_t i = 0; i != allDetails->at(newLevel - 1).footLength; i++) {
					fullLine = fullLine + line + '\n';
					failed = !std::getline(is, line);
				}
			}
		}
		//Read entire section into stream

		
		Section ns;		//default dummy constructor, should always be overriden
		if (newLevel == 1) {
			ns = Section(&allDetails->at(newLevel - 1), newLevel);
		}
		else {
			ns = Section(allDetails, newLevel);
		}

		std::stringstream ssLine(fullLine);

		ssLine >> ns;
		subs.push_back(ns);
		numSubs++;
		//Read full section into ns and push it onto vector
		
		//cout << "Testing section stopper output with line: " << line << endl;
		readNext = !(line == details->stopper ||
			containsKeyword(line));
		//We read NEXT line, to test for
		//outer section stopper
	}


	
}

void Section::readSummaryvals()
{
	/*
		Read Summary values for each section for Axys reader, appear 1 or 2
		lines into the footer of each section
	*/

	/*
		gets next char until there is a number, adds the number
		to the summar vals vector for later access
		as of 7/14/20 Axys value format:
		 -First Number will be mrket value summary (Cash & Eqvs,
		 Equities etc. total) or Total_port if you are on level 2
		 -second number will be "total cost" summary val
	*/

	//read footer into string, then string stream
	std::string footerLine = "";
	for (auto& line : footer) {
		footerLine += line;
	}

	std::stringstream ssFooter(footerLine);
	char c;

	while (ssFooter.get(c)) {
		if (isdigit(c)) {
			std::string num;
			std::getline(ssFooter, num, ',');
			summaryVals.push_back(c + num);
		}
	}

	//setClientname
	if (level > 1) {
		clientName = subs.at(0).getClientName();
	}
	else {
		clientName = rows.at(0).getClientName();
	}
	
}



void Section::addHoldNumber(const std::string& fileName, SectionVals& sv)
{
	/*
		Adds hold (internal) acount number corresponding to account name by looking up
		unique "Client Name" value in configs/hold_numL.csv file
	*/

	cout << "our filename is : " << fileName << endl;
	cout << "our clientName is : " << clientName << endl;

	std::ifstream inFile(fileName);

	readThrough(inFile, clientName, ',');

	std::string discard;
	std::getline(inFile, discard, ',');
	std::getline(inFile, sv.holdNum, ',');
	//should put the hold num right in there

	cout << "our holdNum is : " << sv.holdNum << endl;

	inFile.close();
}


void Section::addDeleteValue(const std::string& fileName, SectionVals& sv)
{
	/*
		Adds delete number corresponding to account name by looking up
		unique "holdNumber" value in configs/deleteL.csv file
	*/




}
/*  Destructor  */
