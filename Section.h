#pragma once

//project includes
#include "DataRow.h"

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

class Section
{
private:
	const static std::string breaker;
	const static std::string fileName;
	// breaker is "&&&&&" string that delineates 
	//header footers in SectionConfig

	string subName;
	static vector<std::string> keywords;
	//Section name and string of keywords that
	//program looks for to identify headers

	vector<std::string> header;
	vector<std::string> footer;
	static std::string stopper;
	//First line of footer, responsible for stopping dataRow Reads
	//MUST BE GENERAL TO ALL SECTION FOOTERS

	static int headLength;
	static int footLength;

	static bool hasHeader;
	static bool hasFooter;

	std::vector<DataRow> rows;
	int numRows;


	/*  Friend methods: File Stream Operators  */

	inline friend std::ostream& operator<<(std::ostream& os, const Section& sub)
	{
		/*
			Output dataRows to stream, comma delimited as per .CSV format
			simply calls the dataRow "insertion" operator.
		*/
		//cout << "calling SECTION IO: " << sub.rows.size() << endl;

		const char newL = '\n';
		char first = '\0';

		for (const auto& row : sub.rows) {
			os << first << row;
			first = newL;
		}

		return os;
	}
	//End INSERTION operator


	//Start EXTRACTION operator
	inline friend std::istream& operator>>(std::istream& is, Section& sub)
	{
		std::string line;
		//declare generic string for getting lines

		if (sub.hasHeader) {

			//get first line for header name:
			std::getline(is, line);
			sub.header.push_back(line);
			sub.subName = line;

			for (size_t i = 0; i != sub.headLength - 1; i++) {
				std::getline(is, line);
				sub.header.push_back(line);
			}

		}

		std::getline(is, line);
		bool readNext = !(line == sub.stopper ||
			sub.containsKeyword(line));

		while (readNext) {

			/*
				Read our data into the DataRow Object then
				call addRow function to add it to
				our rows vector for this section.
			*/

			DataRow row;

			std::stringstream ssLine(line);
			ssLine >> row;
			sub.addRow(row);

			std::getline(is, line);
			readNext = !(line == sub.stopper ||
				sub.containsKeyword(line));
			//If next Line = report footer or header, loop terminates as
			//there are no more rows to read in this section
		}

		if (sub.hasFooter) {
			//if section has a footer, read it in.

			for (size_t i = 0; i != sub.footLength - 1; i++) {
				std::getline(is, line);
				sub.header.push_back(line);
			}
		}

		return is;
	}


	//End Stream operators

public:

	/*  Constructor  */
	Section();

	/*  Initializers  */

	void initSubs();

	void initHeader(std::ifstream& is);

	void initFooter(std::ifstream& is);

	void initKeywords(std::ifstream& is);

	/*  Modifiers  */
	/*  Accessors  */

	/* Other Functions  */
	void addRow(const DataRow& r);

	const bool containsKeyword(const std::string& check) const;

	std::ifstream& readThrough(std::ifstream& is, std::string& brk);


	/*  Destructors  */



};

