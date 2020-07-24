#include "Section.h"

/*
	Implementation file for Class Section
*/

/*  Initialize static variables */
int Section::equityReads = 0;

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

std::string Section::getSecName() {
	return secName;
}

const std::vector<double>& Section::getSummaryVals() {
	return summaryVals;
}

const double Section::getSectionMrktVal() {
	return summaryVals.at(0);
}

const double Section::getSectionTotCost() {
	return summaryVals.at(0);;
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
	if (AR::output.lvl_1) {
		std::stringstream ss;
		ss << endl << "------------------------" << endl;
		ss << "Begining aggregation for client: " << clientName << endl;
		cout << ss.str();
	}


	if (level > 1)
	{
		//aggregate section values then rows

		//Hold Number
		addHoldNumber(lks.holdNum,sv);
		
		//Total_Port Value
		try {
			sv.total_port = summaryVals.at(0);
		}
		catch (const std::out_of_range& e1)
		{ 
			if (AR::output.lvl_1)
			{
				std::stringstream errStream;
				errStream << "Out of range error caught in summaryVals.at(0), aggregateSecs, " <<
					"section level: " << level << " footer not properly exported, error message:  "
					<< endl << e1.what() << endl;

				cout << errStream.str();
			}
			
			sv.total_port = 0;
		}

		//delete Value
		addDeleteValue(lks.deleteFile, sv);

		//for subsection Cash, Equity and Fixed Income Values 
		try
		{
			for (auto& sub : subs) {
				size_t size = sub.getSummaryVals().size();

				if (sub.getSecName() == "Cash & Equivalents" && size > 0) {
					sv.cash += sub.getSummaryVals().at(0);
				}
				else if (sub.getSecName() == "Fixed Income" && size > 0) {
					sv.fixed += sub.getSummaryVals().at(0);
				}
				else if (sub.getSecName() == "Equities" && size > 0) {
					sv.equity += sub.getSummaryVals().at(0);
				}
				else if (sub.getSecName() == "Alternative Assets" && size > 0) {
					sv.equity += sub.getSummaryVals().at(0);
				}
				else if (sub.getSecName() == "Tax Exempt (Munis)") {
					sv.equity += sub.getSummaryVals().at(0);
				}
				
			}

		}
		catch (const std::out_of_range& or1)
		{
			if (AR::output.lvl_1)
			{
				std::stringstream errStream;
				errStream << "Out of range error caught in loading section values" <<
					" footer not properly exported, error message:  " <<
					endl << or1.what() << endl << endl;;

				cout << errStream.str();
			}
			
		}

		//Call aggregate secs for each subsection, which
		//will in turn call aggregate rows
		
		for (auto& sub : subs) {
			sub.aggregateSecs(lks, sv);
		}

	}
	else if (secName == "Equities" || secName == "Alternative Assets"){
		aggregateRows(lks, sv);
	}
	
}

void Section::aggregateRows(const Lookups& lks, SectionVals& sv)
{
	//Call aggregateData for DataRows	
	for (auto& row : rows) {
		row.aggregate(lks, sv, secName);
	}

	

	DataRow dr = rows.at(0);
	double percDone = 100 * (dr.getTotalAgg() /
		static_cast<double>(equityReads));
	static double threshold = 0;

	if (percDone > threshold || percDone == 100) {
		int newThresh = (int)(percDone * 100000) % 25;
		std::stringstream ss;
		ss << percDone << "%" << " processing outputs" << endl << endl;

		if (AR::output.lvl_1) {
			cout << ss.str();
		}
		else {
			threshold += newThresh;
			cout << ss.str();
		}

	}
}

std::ifstream& Section::readThrough(std::ifstream& is, const std::string& brk,
	char c)
{
	//default: c = \n, c2 = \0

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

const bool Section::readRows(std::istream& is)
{
	DataRow dr;
	std::string line;

	bool failed = false;
	bool readNext = true;
	//initialize variables simply

	numRows = 0;
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

		bool blank = (line.find_first_not_of(',') == line.npos);

		if (blank && (line != details->stopper)) {
			return false;
		}

		//Also, if next line is blank and the footer stopper
		//is NOT blank, there is no footer section
		//this occurs in Axys reader section when there is 1 row/section
		
		if (readNext)
		{
			DataRow row;

			std::stringstream ssLine(line);
			ssLine >> row;
			addRow(row);
			numRows++;

			if (secName == "Equities" || secName == "Alternative Assets")
				equityReads++;
		}
	}

	return details->hasFooter;
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
	numSubs = 0;

	//Check for initial blankline, which indicated blank section
	bool blankLine = (line.find_first_not_of(',') == line.npos);
	if (blankLine) {
		throw no_such_object("Blank Section found");
		return;
	}

	DataRow dr;

	while (readNext && !failed)
	{

		fullLine = "";
		bool hitStopper = false;
		blankLine = false;

		while (!hitStopper && !failed)
		{
			fullLine = fullLine + line + '\n';
			failed = !std::getline(is, line);

			blankLine = (line.find_first_not_of(',') == line.npos);
			hitStopper = (line == allDetails->at(newLevel - 1).stopper) || blankLine;

			//this stopper must be a SUBsection stopper, must come
			//from one level lower on alldetails vector

			if (hitStopper && !failed && !blankLine)
			{
				//we must be careful bc if program stopped at a blank line, then
				//there is no footer

				//Read subsection footer into the full line
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
		
		if (blankLine) {
			std::getline(is, line);
			//get an additional line to replace "blankline"
			//currently stored in line variable
		}

		readNext = !(line == details->stopper ||
			containsKeyword(line)); // || blankLine);
		//We read NEXT line, to test for
		//outer section stopper
	}

	//cout << "\n\n\n Continue Reading? ";
	//getchar();
	//cout << endl;
}

void Section::readSummaryvals()
{
	/*
		Read Summary values for each section for Axys reader,we will sum all
		the values in the "MRKT_VAL" column of the report
	*/

	/*
		We obtain the total value of the section (fixed,
		Equities, etc, by summing all the row values)
	*/

	double total = 0;

	switch (level)
	{
	case 1:
		//set client Name
		clientName = rows.at(0).getClientName();


		//At subsection level, sum all the values from the rows

		total = 0;
		for (auto& row : rows) {
			total += row.getMktValAsset();
		}
		summaryVals.push_back(total);
		//Mrkt Value sum at index 0

		total = 0;
		for (auto& row : rows) {
			total += row.getTotCostAsset();
		}

		summaryVals.push_back(total);
		//Tot cost sum at index 1

		break;

	default:
		//If we are at a client level, sum all the summary
		//values from the sections below it

		//set client Name
		clientName = subs.at(0).getClientName();

		total = 0;
		for (auto& sub : subs) {
			total += sub.getSectionMrktVal();
		}
		summaryVals.push_back(total);

		total = 0;
		for (auto& sub : subs) {
			total += sub.getSectionTotCost();
		}
		summaryVals.push_back(total);



		
			cout << "Total Equities for " << clientName <<
				" sums too: " << (std::to_string(total)) << endl;

		break;
	}
	
}


void Section::addHoldNumber(const std::string& fileName, SectionVals& sv)
{
	/*
		Adds hold (internal) acount number corresponding to account name by looking up
		unique "Client Name" value in configs/hold_numL.csv file
	*/

	/*
		We will read in each line as a dataRow, then compare the datarow's second
		column entry to that of the column name. If they are identical, we add
		the holding number (4th data column), else we keep searching.
	*/


	sv.holdNum = 0;
	if (AR::output.lvl_1)
		cout << "our clientName is : " << clientName << endl;

	std::ifstream inFile(fileName);
	int discNum;
	char sep;

	string line;
	bool found = false;
	enum MyEnum { CLIENT = 1, HOLNUM = 3 };
	bool temp = true;

	while (!found && std::getline(inFile, line)) {

		DataRow dr;
		std::stringstream ssLine(line);
		ssLine >> dr;
			
		if (dr.getReadsAt(CLIENT) == clientName) {
			std::stringstream ss(dr.getReadsAt(HOLNUM));
			ss >> sv.holdNum;

			found = true;
		}	
	}

	if (AR::output.lvl_1 && !std::getline(inFile, line))
	{
		std::stringstream errStream;
		errStream << "Hold Number NOT found for client "
			<< clientName << endl;
		cout << errStream.str();
	}

	if (AR::output.lvl_1)
		cout << "our holdNum is : " << sv.holdNum << endl << endl;
	
	inFile.close();
}


void Section::addDeleteValue(const std::string& fileName, SectionVals& sv)
{
	/*
		Adds delete number corresponding to account name by looking up
		unique "holdNumber" value in configs/deleteL.csv file
	*/

	int lkUp = 0;
	char val = '0';
	char c;

	ifstream inFile(fileName);

	try
	{
		do {
			inFile >> lkUp >> c >> val;
		} while ((lkUp != sv.holdNum) && inFile);

		if (inFile)
			sv.deleteNum = val;
		else
			sv.deleteNum = '0';
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "Failure reading " << fileName << ", please make sure " <<
			"file is accessible " << endl << e.what() << endl;
			sv.deleteNum = '0';
	}

	inFile.close();
	//cout << "Delete num is: " << sv.deleteNum << endl;
}


/*  Destructor  */
