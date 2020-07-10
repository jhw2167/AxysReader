#pragma once

//project includes
#include "DataRow.h"
#include "hf_config.h"

/*
	Section class devoted to handling Sections within a
	"client" section, the Section class will include:

		-A VECTOR of Section header "keywords" that inidicate
		where the Section should LOOK for beginining to read
		a Section - saved in "subSect.txt" { user configured }

		-A VECTOR of "headers" (and footers) that includes all columns
		from the section header indexed by row i.e. header[0] is a
		string representing entire first row of section header

		-boolean values that indicate if there is a header or footer
		to be read at all { user configured }

		- A VECTOR of DataRows that belong to this section

		- An INT containing the number of rows in this section

		- Overloaded stream operators to know how to read and write itself
		in and out of files

*/


/*
	
*/

class Section
{
private:

	int level;
	//Section level is higher the further "up" it is on the hierarchy
	//datarows - level 0
	//Subsections (equities, Cash and Equivalents) - level 1
	//Client Portfolio headers - level 2

	std::string secName;
	//Mostly used for subsections that have select few
	//keyword names like "Equities"

	vector<std::string> header;
	vector<std::string> footer;
	
	std::vector<DataRow> rows;
	int numRows;

	std::vector<Section*> subs;
	int numSubs;
	//All subsections of a given section

	hf_config* details;
	//hf_config is a class defined in hf_config.h that 
	//houses details of header and footer for each
	//section type


	/*  Friend methods: File Stream Operators  */

	inline friend std::ostream& operator<<(std::ostream& os, const Section& sec)
	{
		/*
			Output dataRows to stream, comma delimited as per .CSV format
			simply calls the dataRow "insertion" operator.
		*/
		//cout << "calling SECTION IO: " << sec.rows.size() << endl;

		const char newL = '\n';
		char first = '\0';

		for (const auto& row : sec.rows) {
			os << first << row;
			first = newL;
		}

		return os;
	}
	//End INSERTION operator


	//Start EXTRACTION operator
	inline friend std::istream& operator>>(std::istream& is, Section& sec)
	{
		std::string line;
		//declare generic string for getting lines

		if (sec.details->hasHeader) {

			//get first line for header name:
			std::getline(is, line);
			sec.header.push_back(line);
			sec.secName = line;

			for (size_t i = 0; i != sec.details->headLength - 1; i++) {
				std::getline(is, line);
				sec.header.push_back(line);
			}

		}

		std::getline(is, line);
		bool readNext = !(line == sec.details->stopper ||
			sec.containsKeyword(line));

		while (readNext) {

			/*
				Read our data into the DataRow Object then
				call addRow function to add it to
				our rows vector for this section.
			*/

			DataRow row;

			std::stringstream ssLine(line);
			ssLine >> row;
			sec.addRow(row);

			std::getline(is, line);
			readNext = !(line == sec.details->stopper ||
				sec.containsKeyword(line));
			//If next Line = report footer or header, loop terminates as
			//there are no more rows to read in this section
		}

		if (sec.details->hasFooter) {
			//if section has a footer, read it in.

			for (size_t i = 0; i != sec.details->footLength - 1; i++) {
				std::getline(is, line);
				sec.header.push_back(line);
			}
		}

		return is;
	}


	//End Stream operators

public:

	/*  Constructor  */
	Section(hf_config* dts, int lvl);


	/*  Modifiers  */
	void setDetails(hf_config *dts);


	/*  Accessors  */
	const hf_config& getDetails();


	/* Other Functions  */
	void addRow(const DataRow& r);

	const bool containsKeyword(const std::string& check) const;

	//std::ifstream& readThrough(std::ifstream& is, std::string& brk);


	/*  Destructors  */



};

