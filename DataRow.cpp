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


	/*  Modifiers  */
void DataRow::setWriteCols(const int wc) {
	writeCols = wc;
}



	/*  Functions  */
void DataRow::pushBackReads(const std::string &str) {
	reads.push_back(str);
}

void aggregate()
{
	/*  
		DataRow::Aggregate transforms all read data to write data
	*/

	//Add Ticker (by Lookup)

	//Add Shares (Read Quantity)

	//Add Clients (read Portfolio Name)

	//Add AVG_U_COST (read Unit Cost)

	//Add Purch Date (read Date)

	//Add Number (By Lookup: )

	//Add Delete (By Lookup: )

	//Add HOLD_DATE (by Now())

	//Add SP_COMP (Read Lookup: Date in SP_Val_lookup, Formula)

	//Add SP_CURRENT (Entered value)

	//Add TOTAL_PORT (read Unit Cost)

	//Add CASH (read Date)

	//Add FIXED (By Lookup: )

	//Add EQUITY (By Lookup: )

	//Extra

}


//End accessors



	/*  Destructor  */
DataRow::~DataRow()
{
}
