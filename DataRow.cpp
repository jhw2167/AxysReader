#include "DataRow.h"

/*
	Implementation file for DataRow.h
*/


/*  Initialize Static Data Members  */

int DataRow::readCols = 0;
int DataRow::writeCols = 0;

int DataRow::totalRead = 0;
int DataRow::totalWritten = 0;
int DataRow::totalAgg = 0;


namespace AR {
	struct Debugger output;
}

//End inits


	/*  Constructor  */

DataRow::DataRow()
{
	
}


	/*  Accessors  */

const int DataRow::getWriteCols () {
	return writeCols;
}

const int DataRow::getTotalReads() {
	return totalRead;
}

const int DataRow::getTotalWrites() {
	return totalWritten;
}

const int DataRow::getTotalAgg() {
	return totalAgg;
}

const double DataRow::getMktValAsset() {

	std::stringstream ssVal(reads.at(MRKT_VAL));
	double val;
	ssVal >> val;

	return val;
}

const double DataRow::getTotCostAsset()
{
	std::stringstream ssVal(reads.at(TOT_COST));
	double val;
	ssVal >> val;

	return val;
}

std::string DataRow::getClientName() {
	return reads.at(0);
	//client name should be at index 0 of read column
}

std::string DataRow::getReadsAt(int index) {

	bool goodIndex = (index < reads.size()) && (index >= 0);

	if (goodIndex)
		return reads.at(index);
	else
		return std::string();
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
	//writes.push_back(reads.at(SECURITY));

	//Add Ticker (by Lookup in file)
	writes.push_back(searchTicker(lks.ticker));

	//Add Shares (Read Quantity)

	writes.push_back(reads.at(SHARES));

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
	writes.push_back(searchSP(lks.val_SP));

	//Add SP_CURRENT (Entered value)
	writes.push_back(std::to_string(sv.sp_current));

	//Add TOTAL_PORT (read Unit Cost)
	writes.push_back(std::to_string(sv.total_port));

	//Add CASH (From Section val)
	writes.push_back(std::to_string(sv.cash));

	//Add FIXED (From Section val)
	writes.push_back(std::to_string(sv.fixed));

	//Add EQUITY (From Section val)
	writes.push_back(std::to_string(sv.equity));


	//Extra
	totalAgg++;

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

	std::stringstream errStream;

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
		if (AR::output.lvl_2) {
			errStream << "Out of range exception caught in " <<
				"datarow::aggregate::searchTicker " << endl;
			errStream << "Setting ticker to !FOUND" << endl;
		}
			
		or1;
		ticker = "!FOUND";
	}
	catch (const file_open_error& op1)
	{
		
		static bool showFOE = true;

		if (showFOE && AR::output.lvl_1)
		{
			errStream << "File Open error caught in " <<
				"datarow::aggregate::searchTicker " << endl;
			errStream << "Check file " << fileName <<
				" for corruption or if it is missing" << endl;
			errStream << "Setting all tickers to !FOUND" << endl;

			showFOE = false;
		}

		op1;
		ticker = "!FOUND";
	}

	if (AR::output.lvl_1) {
		cout << errStream.str();
	}

	return ticker;
}

std::string DataRow::searchSP(const std::string& fileName, std::string targetDate)
{
	/*
		We search for the dated SP_500 value in sp_val file by date
		if we do not find the desired date we subtract a day and search again
		until found. if the date is less than the minDate, we assign
		0 and continue to next entry.		
	*/

	//If we cant find the date, we ensure its greater than
			// 1/2/1985
	const static Date minDate = Date(1985, 1, 2);

	std::string sp_comp = "0";
	std::string search = "Temp Date val stored here";

	std::string discard;
	/*
		sp_comp hold each line's sp_500 value, search will hold
		each file line's date value, which will be compared to
		targetDate (generated from file read)
		below
		discard is just for excess on each line
	*/
	std::stringstream errStream;

	try
	{
		std::ifstream inFile(fileName);
		//declare and open our file

		if (!inFile) {
			//cout << "not in file: " << fileName << endl;;
			throw file_open_error("");
		}

		if (targetDate == minDate.getStringDate()) {
			targetDate = reads.at(DATE);

			if (Date(targetDate) < minDate) {
				inFile.close();
			}
		}
		//This is the value we are vLookingUp to find
		//the right ticker

		std::getline(inFile, discard);
		std::getline(inFile, discard);
		//First two lines are extraneous

		while ((search != targetDate) && inFile)
		{
			std::getline(inFile, search, ',');
			std::getline(inFile, sp_comp, ',');
			std::getline(inFile, discard);
		}

		if (!inFile) {
			inFile.close();

			Date notFound(targetDate);

			if (notFound > minDate) {
				notFound--;
				searchSP(fileName, notFound.getStringDate());
			}
			else {
				throw std::out_of_range("");				
			}
			
		}
		else
			inFile.close();
	}
	catch (const std::out_of_range& or1)
	{
		if (AR::output.lvl_2) {
			errStream << "Out of range exception caught in " <<
				"datarow::aggregate::searchTicker for date " << targetDate << endl;
			errStream << "sp_val not found, Setting sp val to 0" << endl;
		}

		or1;
		sp_comp = "0";
	}
	catch (const file_open_error& op1)
	{
		static bool showError = true;

		if (showError && AR::output.lvl_1)
		{
			errStream << "File Open error caught in " <<
				"datarow::aggregate::searchTicker " << endl;
			errStream << "Check file " << fileName <<
				" for corruption or if it is missing" << endl;
			errStream << "Setting all tickers to !FOUND" << endl;

			showError = false;
		}

		op1;
		sp_comp = "0";
	}
	catch (const bad_date_component& bd1)
	{
		errStream << "Bad date component exception found in sp_search " <<
			"for date: " << targetDate << "sp value will be seet to default 0";
		sp_comp = "0";
	}

	if (AR::output.lvl_1) {
		cout << errStream.str();
	}

	return sp_comp;
}



//End accessors



	/*  Destructor  */
DataRow::~DataRow()
{
}
