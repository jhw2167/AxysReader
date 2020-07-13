#include "Section.h"

/*
	Implementation file for Class Section
*/


/*  Constructor  */
Section::Section() {
	//cout << "dummy constructor " << endl;
	details = nullptr;
	allDetails = nullptr;
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

	for (auto i : details->keywords) {
		found = (sample.find(i) != std::string::npos);
	}


	return found;
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



/*  Destructor  */
