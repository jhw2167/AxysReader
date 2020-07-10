#include "Section.h"

/*
	Implementation file for Class Section
*/


/*  Constructor  */
Section::Section(hf_config* dts, int lvl) {
	level = lvl;
	//All sections must have a level

	secName = "";
	//not all sections will have a name

	numRows = 0;
	numSubs = 0;
	//initial number of rows and subsections per 
	//section is 0

	setDetails(dts);
}


/*  Modifiers  */
void Section::setDetails(hf_config* dts) {
	details = dts;
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


/*  Destructor  */
