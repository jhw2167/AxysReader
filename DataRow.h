#pragma once

/*
	DataRow object header. The user defined "DataRow" type will know how to:

		-Read values from a .CSV row into its value array
		-Access .txt files that instruct it HOW to read and write .csv files,
			and where to find information that it needs
		-Aggregate data sufficient to write to csv(with data read from file, and data searched up elsewhere)
		-write a data row in .CSV format 
*/

//Project includes
#include "DateTime.h"


class DataRow
{
private:
	
	static int readCols;
	static int writeCols;

	vector<string> reads;
	vector<string> writes;


	/*  Friend methods: File Stream Operators  */

	// BEGIN INSERTION STREAM OPERATOR
	inline friend std::ostream & operator<<(std::ostream & os, const DataRow & dRow)
	{
		/*
			Output columns to stream, comma delimited as per .CSV format
			Used a cute little trick here to ensure the FINAL column does
				not include an additional delimitter
		*/

		//cout << "calling dataRow Ostream: " <<  dRow.reads.size() << endl;

		const auto* sep = "";
		const auto invBlank = ",";

		for (const auto& col : dRow.reads) {
			os << sep << col;
			sep = invBlank;
		}

		return os;
	}
	//END INSERTION STREAM OPERATOR
 

	//BEGIN EXTRACTION STREAM OPERATOR
	inline friend std::istream& operator>>(std::istream& is, DataRow& dRow)
	{
		/*
			CSV extraction operator with comprehensive error handling
			will extract a single data column from CSV
		*/

		std::string streamErr = "ERROR: in DataRow class overloaded EXTRACTION operator>>\n";
		streamErr += "the input stream is in a failed state, no processing can take place...\n";

		//cout << "Made it to datarow Istream: " << endl;

		//Check if stream is initially in failed state
		if (!is) {
			throw std::invalid_argument(streamErr);
		}

		try
		{
			string val;
			char sep = ',';
			std::getline(is, val);
			std::istringstream ssLine(val);
			//Declare new String and read row value into it 

			char nextChar = ssLine.peek();
			bool nextCol = !(nextChar == -1);
			//Set nextChar and nextCol

			while (nextCol) {

				/*
					Parse data using char ',' as delimiter, we must be
					careful though, cause some data have ',' in them already,
					these data are wrapped in "" we must detect
				*/

				if (nextChar == '"') {
					ssLine >> sep;
					std::getline(ssLine, val, '"');
					val = '"' + val + '"';
					ssLine >> sep;
					/*
						If the column has a ',' in it already, we use
						' " ' as our delimiter, then pick up the comma
						following it with a ssLine >> sep at the end
					*/
				}
				else {
					std::getline(ssLine, val, ',');
					/* Else we just get comma delimitted column */
				}

				dRow.pushBackReads(val);
				nextChar = ssLine.peek();
				//Peek next char to see what it is

				nextCol = !(nextChar == -1);
				//If nextChar is -1 (end of the line), while loop will terminate
			}

		}

		catch (std::invalid_argument& ia1)
		{
			//catches invalid argument exception if stream cannot be read
			std::cout << "Caught invalid_argument exception in Date class overloaded operator>>\n";
			std::cout << ia1.what() << endl;

			// allow processing to continue, exception rethrown
			throw;
		}
		catch (std::runtime_error& rte1)
		{
			cout << "Runtime error caught in DataRow (extraction) operator>> \n";
			std::cout << rte1.what() << endl;
		}
		catch (...)
		{
			//catches all possible exceptions from stream extraction or from setting date

			std::cout << "unknown exception caught in DataRow class overloaded operator>>\n";
			throw;
		}

		return is;
	}
	//END EXTRACTION STREAM OPERATOR


	//End Stream Operators

public:

	/*  Constructor  */
	DataRow();


	/*  Accessors  */
	int getWriteCols();


	/*  Modifiers  */
	void setWriteCols(const int wc);


	/*  Functions  */
	void pushBackReads(const std::string &str);


	/*  Destructor  */
	~DataRow();

};

