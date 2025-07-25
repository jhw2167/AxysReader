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

	static int equityReads;
	//number of Equity and Alternative Asset Rows Read

	std::string clientName;
	std::string secName;
	//Mostly used for subsections that have select few
	//keyword names like "Equities"

	vector<std::string> header;
	vector<std::string> footer;
	
	std::vector<DataRow> rows;
	int numRows;

	std::vector<Section> subs;
	int numSubs;
	//All subsections of a given section

	const hf_config* details;
	const std::vector<hf_config>* allDetails;
	//hf_config is a class defined in hf_config.h that 
	//houses details of header and footer for each
	//section type

	std::vector<double> summaryVals;
	//holds summary vls for the end of each section

	/*  Private Functions  */
	const bool readRows(std::istream& is);
	void readSubsections(std::istream& is);
	void readSummaryvals();

	void addHoldNumber(const std::string& holdNumFile, SectionVals& sv);
	void addDeleteValue(const std::string& deleteFile, SectionVals& sv);

	/*  Friend methods: File Stream Operators  */

	inline friend std::ostream& operator<<(std::ostream& os, const Section& sec)
	{
		/*
			Output dataRows to stream, comma delimited as per .CSV format
			simply calls the dataRow "insertion" operator.
		
		*/

		//cout << "calling SECTION OSTREAM for level " << sec.level <<
			//", size is: " << sec.rows.size() << endl;

		if (sec.level == 1) {
			
			if (sec.secName == "Equities" || sec.secName == "Alternative Assets")
			{
				for (const auto& row : sec.rows) {
					os << row << endl;
				}
			}

		}
		else {

			for (const auto& sub : sec.subs) {
				os << sub;
			}
		}

		return os;
	}
	//End INSERTION operator


	//Start EXTRACTION operator
	inline friend std::istream& operator>>(std::istream& is, Section& sec)
	{
		DataRow dr;
		std::string line;
		//declare generic string for getting lines		

		if (sec.details->hasHeader) {

			//get first line for header name:
			std::getline(is, line);
			sec.header.push_back(line);
			int index = std::min(line.find_first_of(','), line.size());
			sec.secName = line.substr(0, index);

			if (AR::output.lvl_1 && sec.level == 2) {
				cout << "First line of header is: " << sec.secName << endl;
			}
			else if (AR::output.lvl_2){
				cout << "First line of header is: " << sec.secName << endl;
				//getchar();
			}

			for (size_t i = 0; i != sec.details->headLength - 1; i++) {
				std::getline(is, line);
				sec.header.push_back(line);
			}
		}

		//cout << "Made it to Section Istream: " << endl;
		bool check = sec.details->hasFooter;
		bool blankSec = false;

		try
		{
			if (sec.level == 1) {
				check = sec.readRows(is);
			}
			else {
				sec.readSubsections(is);
			}

		}
		catch (const no_such_object& ns1){

			if (AR::output.lvl_1) {
				cout << "Blank section found at level " << sec.level
					<< "... Proceeding with reads" << endl << endl;
			}
			
			ns1;
			blankSec = true;
		}
		catch (const std::exception& e1){

			if (AR::output.lvl_1) {
				cout << "Unkown exception caught in section level " <<
					sec.level << " with error code: " << endl
					<< e1.what() << endl;
			}


			blankSec = true;
		}

		if (check && sec.details->hasFooter) {
			//if section has a footer, read it in.
			//cout << "Level is: " << sec.level << endl;


			for (size_t i = 0; i != sec.details->footLength - 1; i++) {
				std::getline(is, line);
				//cout << "line of footer is: " << line << endl;
				sec.footer.push_back(line);
			}
		}

		if (!blankSec) {
			sec.readSummaryvals();
		}
		else if (sec.level == 2) {
			std::getline(is, line);
			//we need to get one more line from
			//footer in a blank section
		}

		if (sec.secName == "Equities" || sec.secName == "Alternative Assets")
		{
			/*  Handling visibility of output by different levels  */

			if (AR::output.lvl_1) {
				cout << "Client: " << sec.rows.at(0).getClientName() << endl;
			}

			if (AR::output.lvl_2) {
				cout << "Section: " << sec.secName << endl;
				cout << "Vector Length: " << sec.rows.size() << "  numRows: " << sec.numRows <<
				endl << "  Total Reads so far: " << sec.rows.at(0).getTotalReads();
			}

			if (AR::output.lvl_1) {
				cout << endl << "| " << "Total equity reads: " << sec.equityReads << " |";
				cout << endl << "----------------------------\n\n\n";
			}
		}

		return is;
	}


	//End Stream operators

public:

	/*  Constructor  */
	Section();

	Section(const hf_config* dts, int lvl);

	
	Section(const std::vector<hf_config>* aDts, int lvl);


	/*  Initializers  */
	void initVars(int lvl);


	/*  Modifiers  */
	void setDetails(const hf_config *dts);

	void setAllDetails(const std::vector<hf_config>* aDts);


	/*  Accessors  */
	const hf_config& getDetails();

	std::string getClientName();

	std::string getSecName();

	const std::vector<double>& getSummaryVals();

	const double getSectionMrktVal();

	const double getSectionTotCost();

	/* Other Functions  */
	void addRow(const DataRow& r);

	const bool containsKeyword(const std::string& check) const;

	void aggregateSecs(const Lookups& lks, SectionVals& sv);

	void aggregateRows(const Lookups& lks, SectionVals& sv);

	std::ifstream& readThrough(std::ifstream& is, const std::string& brk,
		char c = '\n');

	/*  Destructors  */



};

