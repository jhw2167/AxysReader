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




//End accessors



	/*  Destructor  */
DataRow::~DataRow()
{
}
