#include "DataRow.h"

/*
	Implementation file for DataRow.h
*/


/*  Initialize Static Data Members  */

int DataRow::readCols = 0;
int DataRow::writeCols = 0;
//End inits


	/*  Constructor  */

DataRow::DataRow()
{
	
}


	/*  Accessors  */

int DataRow::getWriteCols () {
	return writeCols;
}

std::string DataRow::getClientName() {
	return reads.at(0);
	//client name should be at index 0 of read column
}


	/*  Modifiers  */
void DataRow::setWriteCols(const int wc) {
	writeCols = wc;
}



	/*  Functions  */
void DataRow::pushBackReads(const std::string &str) {
	reads.push_back(str);
}

void DataRow::aggregate(const Lookups& lks, SectionVals& sv, std::string secName)
{
	/*  
		DataRow::Aggregate transforms all read data to write data
	*/

	//Lets see company name too:
	writes.push_back(reads.at(SECURITY));

	//Add Ticker (by Lookup in file)
	writes.push_back(searchTicker(lks.ticker));

	//Add Shares (Read Quantity)
	if (secName == "Equities")
		writes.push_back(reads.at(SHARES));
	else
		writes.push_back("0");
	

	//Add Clients (read Portfolio Name)
	writes.push_back(reads.at(CLIENT));

	//Add AVG_U_COST (read Unit Cost)
	writes.push_back(reads.at(UNIT_COST));

	//Add Purch Date (read Date)
	writes.push_back(reads.at(DATE));

	//Add Number (By Lookup: )
	writes.push_back(std::to_string(sv.holdNum));

	//Add Delete (By Lookup: )
	writes.push_back(sv.deleteNum);

	//Add HOLD_DATE (by Now())
	writes.push_back(sv.nowDate);

	//Add SP_COMP (Read Lookup: Date in SP_Val_lookup, Formula)
	writes.push_back("SP_COMP");

	//Add SP_CURRENT (Entered value)
	writes.push_back(std::to_string(sv.sp_current));

	//Add TOTAL_PORT (read Unit Cost)
	writes.push_back(sv.total_port);

	//Add CASH (From Section val)
	writes.push_back(sv.cash);

	//Add FIXED (From Section val)
	writes.push_back(sv.fixed);

	//Add EQUITY (From Section val)
	writes.push_back(sv.equity);


	//Extra
	writeCols = writes.size();
}

std::string DataRow::searchTicker(const std::string& fileName)
{
	std::string ticker = "!FOUND";
	std::string search = "Company Name stored here";
	std::string discard;
	char sep;
	/*
		Ticker hold each line's ticker value, search will hold
		each file line's company value, which will be compared to 
		security below, which is the string name of THIS datarow's
		company name, stored in its "security" column

		discard is just for excess on each line
	*/

	try
	{
		std::ifstream inFile(fileName);
		//declare and open our file

		const std::string security = reads.at(SECURITY);
		//This is the value we are vLookingUp to find
		//the right ticker

		while ((search != security) && inFile)
		{
			if (inFile.peek() == '"') {
				inFile >> sep;
				std::getline(inFile, search, '"');
				search = '"' + search + '"';
				inFile >> sep;
			}
			else {
				std::getline(inFile, search, ',');
			}
			
			std::getline(inFile, ticker, ',');
			std::getline(inFile, discard);
		}

		if (!inFile) {
			ticker = "!FOUND";
		}

		inFile.close();
	}
	catch (const std::out_of_range& or1)
	{
		std::cout << "Out of range exception caught in " <<
			"datarow::aggregate::searchTicker " << endl;
		std::cout << "Setting ticker to !FOUND" << endl;

		ticker = "!FOUND";
	}
	catch (const file_open_error& op1)
	{
		static bool showError = true;
	
		if (showError)
		{
			std::cout << "File Open error caught in " <<
				"datarow::aggregate::searchTicker " << endl;
			std::cout << "Check file " << fileName << 
				" for corruption or if it is missing"  << endl;
			std::cout << "Setting all tickers to !FOUND" << endl;

			showError = false;
		}

		ticker = "!FOUND";
	}
	
	

	return ticker;
}


//End accessors



	/*  Destructor  */
DataRow::~DataRow()
{
}
