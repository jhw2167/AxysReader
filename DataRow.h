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

	// BEGIN DATE INSERTION STREAM OPERATOR
	inline friend std::ostream & operator<<(std::ostream & os, const DataRow & dRow)
	{
		/*
			Output columns to stream, comma delimited as per .CSV format
			Used a cute little trick here to ensure the FINAL column does
				not include an additional delimitter
		*/

		const auto* sep = "";
		const auto invBlank = ",";

		for (const auto& col : dRow.writes) {
			os << sep << col;
			sep = invBlank;
		}

		return os;
	}
	//END DATE INSERTION STREAM OPERATOR
 
	//BEGIN DATE EXTRACTION STREAM OPERATOR
	inline friend std::istream& operator>>(std::istream& is, DataRow& dRow)
	{
		/*
			CSV extraction operator with comprehensive error handling
			will extract a single data column from CSV
		*/

		std::string streamErr = "ERROR: in Date class overloaded EXTRACTION operator>>\n";
		streamErr += "the input stream is in a failed state, no processing can take place...\n";

		//Check if stream is initially in failed state
		if (!is) {
			throw std::invalid_argument(streamErr);
		}

		try
		{
			bool endOfRow = false;

			while (!endOfRow) {

				string val;
				is >> val;
				//Declare new String and read column value into it 


				cout << val << endl;


				dRow.reads.push_back(val);

				endOfRow = true;
			}




			bool streamFailed = false;


			if (streamFailed)
			{
				std::string readStreamErr = "DataRow class overloaded operator>> err\n";
				readStreamErr += "the input stream is in a failed state, ";
				readStreamErr += "possible attempt to read past end of file, ";
				throw std::runtime_error(readStreamErr);
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
	//END DATE EXTRACTION STREAM OPERATOR


	//End Stream Operators

public:

	/*  Constructor  */
	DataRow();




	/*  Accessors  */
	int getWriteCols();


	/*  Destructor  */
	~DataRow();

};

