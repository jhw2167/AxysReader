#include "Section.h"

/*
	Implementation file for Class Section
*/


/*  Constructor  */
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

	std::getline(is, line);
	bool readNext = !(line == details->stopper ||
		containsKeyword(line));

	while (readNext) {

		/*
			Read our data into the DataRow Object then
			call addRow function to add it to
			our rows vector for this section.
		*/

		DataRow row;

		std::stringstream ssLine(line);
		ssLine >> row;
		addRow(row);
		numRows++;

		std::getline(is, line);
		readNext = !(line == details->stopper ||
			containsKeyword(line));
		//If next Line = report footer or header, loop terminates as
		//there are no more rows to read in this section
	}

}

void Section::readSubsections(std::istream& is)
{
	/*
		Section* newSec = new Section(&configs, levels);
	inStream >> *newSec;
	subs.push_back(newSec);
	
	*/
	int newLevel = level - 1;

	if (newLevel == 1)
	{
		Section ns = Section(&allDetails->at(newLevel - 1), newLevel);
		is >> ns;
		subs.push_back(ns);
	}
	else 
	{
		Section ns = Section(allDetails, newLevel);
		is >> ns;
		subs.push_back(ns);
	}
}



/*  Destructor  */
