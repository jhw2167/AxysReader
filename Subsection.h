#pragma once

//project includes
#include "DataRow.h"

/*
	Subsection class devoted to handling subsections within a
	"client" section, the Subsection class will include:

		-A VECTOR of Subsection header "keywords" that inidicate
		where the subsection should LOOK for beginining to read
		a subsection - saved in "subSect.txt" { user configured }

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

class Subsection
{
private:
	bool sectionEnd;
	static vector<std::string> keywords;

	static vector<std::string> header;
	static vector<std::string> footer;

	static bool hasHeader;
	static bool hasFooter;

	vector<DataRow> rows;
	int numRows;



	/*  Friend methods: File Stream Operators  */

	inline friend std::ostream& operator<<(std::ostream& os, const Subsection& sub)
	{
		/*
			Output dataRows to stream, comma delimited as per .CSV format
			simply calls the dataRow "insertion" operator.
		*/

		cout << "calling SECTION IO: " << sub.rows.size() << endl;

		for (const auto& row : sub.rows) {
			os << row << endl;
		}

		return os;
	}
	//End INSERTION operator


	//Start EXTRACTION operator
	inline friend std::istream& operator>>(std::istream& is, Subsection& sub) 
	{
		if (sub.hasHeader) {
			//readHeader
		}

		string line;
		std::getline(is, line);

		bool readNext = !(line == sub.footer[0] || line == sub.header[0]);

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
			readNext = !(line == sub.footer[0] || line == sub.header[0]);
			//If next Line = report footer or header, loop terminates as
			//there are no more rows to read in this section
		}

		if (sub.hasFooter) {
			//readHeader
		}

	}


	//End Stream operators

public:

	/*  Constructor  */
	Subsection();


	/*  Modifiers  */
	/*  Accessors  */
	/*  Functions  */
	void addRow(const DataRow &r);


	/*  Destructors  */



};

