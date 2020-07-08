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



//End accessors



	/*  Destructor  */
DataRow::~DataRow()
{
}
