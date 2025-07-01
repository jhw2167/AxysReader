#include "DataRow.h"
#include <unordered_map>

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
	writes.push_back(searchSP(lks.val_SP, reads.at(DATE)));

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

std::string DataRow::searchTicker(const std::string& fileName) {
	static std::unordered_map<std::string, std::string> tickerMap;
	static bool loaded = false;

	std::stringstream errStream;

	if (!loaded) {
		std::ifstream inFile(fileName);
		if (!inFile) {
			throw file_open_error("Could not open ticker file: " + fileName);
		}

		std::string company, ticker, discard;

		while (inFile) {
			if (inFile.peek() == '"') {
				inFile.get(); // skip "
				getline(inFile, company, '"');
				company = '"' + company + '"';
				inFile.get(); // skip ,
			}
			else {
				getline(inFile, company, ',');
			}
			getline(inFile, ticker, ',');
			getline(inFile, discard);

			tickerMap[company] = ticker;
		}
		loaded = true;
	}

	std::string security = reads.at(SECURITY);

	auto it = tickerMap.find(security);
	if (it != tickerMap.end()) {
		return it->second;
	}
	else {
		if (AR::output.lvl_2) {
			errStream << "Ticker not found for company: " << security << endl;
		}
		if (AR::output.lvl_1) {
			cout << errStream.str();
		}
		return "!FOUND";
	}
}


std::string DataRow::searchSP(const std::string& fileName, std::string targetDateString) {
	static std::vector<std::pair<Date, std::string>> spVals;
	static bool loaded = false;
	const static Date minDate = Date(1985, 1, 2);

	std::stringstream errStream;

	if (!loaded) {
		std::ifstream inFile(fileName);
		if (!inFile) {
			throw file_open_error("Could not open SP file: " + fileName);
		}

		std::string dateStr, spVal, discard;
		getline(inFile, discard); // skip header
		getline(inFile, discard); // skip header

		while (getline(inFile, dateStr, ',')) {
			getline(inFile, spVal, ',');
			getline(inFile, discard);

			try {
				Date d(dateStr);
				spVals.emplace_back(d, spVal);
			}
			catch (const bad_date_component&) {
				if (AR::output.lvl_2) {
					cout << "Invalid date in SP file: " << dateStr << endl;
				}
			}
		}

		// just to be sure
		std::sort(spVals.begin(), spVals.end(),
			[](const auto& a, const auto& b) { return a.first < b.first; });

		loaded = true;
	}

	Date targetDate(targetDateString);
	if (targetDate < minDate) {
		if (AR::output.lvl_2) {
			cout << "Date below minDate: " << targetDateString << endl;
		}
		return "165.37"; // fallback
	}

	auto it = std::lower_bound(spVals.begin(), spVals.end(), targetDate,
		[](const auto& pair, const Date& val) {
			return pair.first < val;
		});

	if (it == spVals.begin()) {
		return it->second; // earliest value
	}
	if (it == spVals.end() || it->first != targetDate) {
		// if not exact, fallback to previous date
		--it;
	}

	return it->second;
}



//End accessors



	/*  Destructor  */
DataRow::~DataRow()
{
}
