/*
	DateTime.cpp
	Author: Jack Welsh
	Implementation file for dateTime.h
*/


#include "DateTime.h"

using std::cout;
using std::endl;

int MIN_HOUR = 0;
int MAX_HOUR = 24;    
// assumes that 24:00 is valid for midnight, 00:00 would also be valid

int MIN_MINUTE = 0;
int MAX_MINUTE = 59;

const int Date::mDays[12] = { 31, 29, 31, 30, 31, 30,
31, 31, 30, 31, 30, 31 };

/****************************************************************************
****************************************************************************
**                                                                        **
**                                                                        **
**                 class Date implementation code                         **
**                                                                        **
**                                                                        **
****************************************************************************
****************************************************************************/


//BEGIN DATE INSERTION STREAM OPERATOR
std::ostream& operator<<(std::ostream &os, const Date &dateToRead)
{
	int year = dateToRead.yyyy;				//stores date components in local variables
	int month = dateToRead.mm;
	int day = dateToRead.dd;

	const int century20 = 19;
	const int century21 = 20;
	//const vars act as prefixes to orient date correctly

	const int pad = 0;			
	//0 padding to ensure date and month values are always two digits

	char s = '-';			
	//slash char as delimiting value;

	os << year << s;

	if (month < 10)
		os << pad;
	
	os << month << s;

	if (day < 10)
		os << pad;

	os << day;

	return os;

}
//END DATE INSERTION STREAM OPERATOR

//BEGIN DATE EXTRACTION STREAM OPERATOR
std::istream& operator>>(std::istream &is, Date &dateToSet)
{

	std::string streamErr = "ERROR: in Date class overloaded EXTRACTION operator>>\n";
	streamErr += "the input stream is in a failed state, no processing can take place...\n";

	//Check if stream is initially in failed state
	if (!is)
	{
		throw std::invalid_argument(streamErr);
	}

	int readYear = 0, readMonth = 0, readDay = 0;		//used to store read day month and year values

	char ch;								//used for extrating delimiting character

	try 
	{
		bool streamFailed = false;			// flag signals if program can keep reading data


		is >> readYear;
		if (!is) { streamFailed = true; }		//tests to see if stream failed after each read, will throw exception if so

		is >> ch;

		is >> readMonth;
		if (!is) { streamFailed = true; }

		is >> ch;

		is >> readDay;
		if (!is) { streamFailed = true; }


		if (streamFailed)
		{
			std::string readStreamErr = "Date class overloaded operator>> err\n";
			readStreamErr += "the input stream is in a failed state, ";
			readStreamErr += "possible attempt to read past end of file, ";
			throw std::runtime_error(readStreamErr);
		}

		dateToSet.setDate(readYear, readMonth, readDay);

	}

	catch (std::invalid_argument &ia1)		//catches invalid argument exception if stream cannot be read
	{
		std::cout << "Caught invalid_argument exception in Date class overloaded operator>>\n";
		std::cout << ia1.what() << endl;

		// allow processing to continue, exception rethrown
		throw;
	}
	catch (std::runtime_error &rte1)
	{
		cout << "Runtime error caught in Date (extraction) operator>> \n";
		std::cout << rte1.what() << endl;
	}

	catch (...)							//catches all possible exceptions from stream extraction or from setting date
	{
		std::cout << "unknown exception caught in Date class overloaded operator>>\n";
		throw;
	}

	return is;

}
//END DATE EXTRACTION STREAM OPERATOR




//BEGIN DATE CONSTRUCTOR
Date::Date(int year, int month, int day) {
	//yyyy, mm, dd format
	setDate(year, month, day);
}		
//END DATE CONSTRUCTOR 


//BEGIN DATE STRING CONSTRUCTOR
Date::Date(std::string newDate)
{
	std::stringstream ssDate(newDate);

	int yr, mm, dy;
	char c;

	ssDate >> mm;
	ssDate >> c;

	ssDate >> dy;
	ssDate >> c;

	ssDate >> yr;
	
	setDate(yr, mm, dy);

	//cout << "Sample date is: " << *this << endl;;

}

//BEGIN DATE CLASS COPY CONSTUCTOR
Date::Date(const Date &dateToCopy)
{
	setDate(dateToCopy.yyyy, dateToCopy.mm, dateToCopy.dd);
}
//END date class copy constructor 


//BEGIN SET DATE MEMBER FUNCTION -- TAKES YY, MM, DD
void Date::setDate(int year, int month, int day)	//yyyy, mm, dd format
{
	//string and bool for testing and describing invalid date arguments
	std::string dateErr = "Set date class attempted to set invalid date, setting date to default values \n";
	bool invalidDate = false;

	bool oddMonth = (1 == month % 2);
	bool febuary = (month == 2);
	bool pastAugust = (month > 7);
		

	if ( year < min_year || year > max_year)
	{
		//included personal intToString::toString() method to conversions
		//Found at top of DateTime.h
		invalidDate = true;
		dateErr += intToString::toString(year);
		dateErr += " is an invalid year, year must be between ";
		dateErr += intToString::toString(min_year) + " and " + intToString::toString(max_year);
	}
	else if (month < min_month || month > max_month)
	{
		invalidDate = true;
		dateErr += intToString::toString(month);
		dateErr += " is an invalid month, month must be between ";
		dateErr += intToString::toString(min_month) + " and " + intToString::toString(max_month);

	}
	else if (febuary)
	{
		if (day < min_day || day > max_day - 2)		//if the month is febuary
		{
			invalidDate = true;
			dateErr += intToString::toString(day);
			dateErr += " is an invalid day for month ";
			dateErr += intToString::toString(month);
			dateErr += ", day must be between ";
			dateErr += intToString::toString(min_day) + " and " 
				+ intToString::toString(max_day - 2) + '\n';
		}
	}

	else if ( (pastAugust && oddMonth) || (!pastAugust && !oddMonth))		//if we're in a month with 30 days
	{
		if (day < min_day || day > max_day - 1)
		{
			invalidDate = true;
			dateErr += intToString::toString(day);
			dateErr += " is an invalid day for month ";
			dateErr += intToString::toString(month);
			dateErr += ", day must be between ";
			dateErr += intToString::toString(min_day) + " and " + intToString::toString(max_day - 1);
		}
	}
	else if ((oddMonth && !pastAugust) || (!oddMonth && pastAugust))		//if we're in a month with 31 days
	{
		if (day < min_day || day > max_day)
		{
			invalidDate = true;
			dateErr += day;
			dateErr += " is an invalid day for month ";
			dateErr += intToString::toString(month);
			dateErr += ", day must be between ";
			dateErr += intToString::toString(min_day) + " and " + intToString::toString(max_day);
		}
	}

	try
	{
		if (invalidDate) {
			cout << "Attempt to assign invalid date, setting date to default values" << endl;
			throw bad_date_component(dateErr);
		}
	}
	catch (bad_date_component &bdc1)
	{
		cout << bdc1.what();

		//Exception is function ending
		throw;
	}

	yyyy = year;
	mm = month;
	dd = day;

}
//END SET DATE METHOD


//BEGIN SET DATE MEMBER FUNCTION -- TAKES DATE OBJECT
void Date::setDate(const Date &otherDate)	
{

	this->yyyy = otherDate.yyyy;
	this->mm = otherDate.mm;
	this->dd = otherDate.dd;

}
//END SET DATE METHOD


//BEGIN GET DAY METHOD
int Date::getDay() const
{
	return this->dd;
}
//END GET DAY METHOD


//BEGIN GET MONTH METHOD
int Date::getMonth() const
{
	return this->mm;
}
//END GET MONTH METHOD

//BEGIN GET YEAR METHOD
int Date::getYear() const
{
	return this->yyyy;
}
//END GET YEAR METHOD


//BEGIN GET STRING DATE
std::string Date::getStringDate() const
{
	char c = '/';
	std::string dateStr;

	dateStr = std::to_string(mm) + c ;
	dateStr += std::to_string(dd) + c;
	dateStr += std::to_string(yyyy);

	return dateStr;
}
//END GET STRING DATE


//BEGIN OVERLOADED EQUALITY OPERATOR
bool Date::operator==(const Date &rhsObj) const
{
	bool isEqualTo = false;

	if (yyyy == rhsObj.yyyy)					//resursive tests equality of date, month and year to determine equality
	{
		if (mm == rhsObj.mm)
		{
			if (dd == rhsObj.dd){
				isEqualTo = true;
			}
		}
	}

	return isEqualTo;
}
//END OVERLOADED EQUALITY OPERATOR



//BEGIN OVERLOADED EQUALITY OPERATOR
bool Date::operator!=(const Date &rhsObj) const
{
	return !(operator==(rhsObj));					//returns inverse of == operator
}
//BEGIN OVERLOADED EQUALITY OPERATOR


//BEGIN OVERLOADED EQUALITY OPERATOR
bool Date::operator<=(const Date &rhsObj) const
{
	return (operator<(rhsObj) || operator==(rhsObj));
}
//BEGIN OVERLOADED EQUALITY OPERATOR


//BEGIN OVERLOADED Less than OPERATOR
bool Date::operator<(const Date &rhsObj) const
{

	bool isLessThan = false;

	if (yyyy < rhsObj.yyyy)
	{
		isLessThan = true;
	}

	if (!isLessThan)		//additional boolean potential substitutes potential two tests for 1 if initial test by years is true
	{

		if (yyyy == rhsObj.yyyy)
		{
			if (mm < rhsObj.mm)
			{
				isLessThan = true;
			}
		}

		if (yyyy == rhsObj.yyyy)
		{
			if (mm == rhsObj.mm)
			{
				if (dd < rhsObj.dd)
				{
					isLessThan = true;

				}
			}
		}

	}

	return isLessThan;

}
//END OVERLOADED LESS THAN OPERATOR


//BEGIN OVERLOADED GREATER THAN OPERATOR
bool Date::operator>=(const Date &rhsObj) const {
	return !operator<(rhsObj);
}
//END OVERLOADED GREATER THAN / EQUAL OPERATOR


//BEGIN OVERLOADED GREATER THAN OPERATOR
bool Date::operator>(const Date &rhsObj) const {
	return !operator<=(rhsObj);
}
//END OVERLOADED GREATER THAN OPERATOR


//BEGIN OVERLOADED ASSIGNMENT OPERATOR
const Date& Date::operator=(const Date &rhsObj){
	setDate(rhsObj.yyyy, rhsObj.mm, rhsObj.dd);
	return *this;
}

Date Date::operator--(int dec)
{
	//cout << "Date we are decrementing is: " << *this << endl;

	if (dd != 1) {
		dd--;
	}
	else if (mm != 1) {
		--mm;
		dd = mDays[mm - 1];
	} 
	else {
		mm = 12;
		dd = mDays[DEC];
		yyyy--;
	}

	cout << endl;

	return *this;
}



/****************************************************************************
*                    Time class default constructor                        *
****************************************************************************/


//BEGIN CONSTRUCTOR FOR TIME WITH PARAMETERS WITH HOUR AND MINUTE PARAMETERS
Time::Time(int hour, int minute)
{
#ifdef DEBUG_DATE
	std::cout << "Entered constructor Time::Time(int, int)" << std::endl;
#endif

	// call method setTime where data validation will be accomplished

	setTime(hour, minute);

#ifdef DEBUG_DATE
	std::cout << "Exiting constructor Time::Time(int, int)" << std::endl;
#endif
}
//END constructor with parameters for Time class



/****************************************************************************
*                  Time class member function setTime                      *
****************************************************************************/

//BEGIN SET TIME MEMBER FUNCTION
void Time::setTime(int hour, int minute)
{

	// use string stream to generate message with heterogeneous data types
	// see code in if(badArguments) block
	std::ostringstream errStrm;

	// set data members to default values, if values passed
	// in to the parameters pass validation, then the
	// data members will be updated
	hh = MIN_HOUR;
	mm = MIN_MINUTE;

	bool badArguments = false;

	if (hour < MIN_HOUR || minute < MIN_MINUTE || hour > MAX_HOUR || minute > MAX_MINUTE)
	{
		// one on more of the values passed in are not valid
		badArguments = true;
	}
	else if (hour == MAX_HOUR && minute > 0)
	{
		// if the value passed for hour is 24, then the only valid value for minutes is 0
		badArguments = true;
	}

	if (badArguments)
	{
		// one or more parameters are not valid, throw an exception
		// note that we do not need a try block to throw the exception since
		// we do not plan to handle the exception within Time::setTime
		// control will immediately return to the calling function

		errStrm << "ERROR: in method Time::setTime( " << hour << ", " << minute << " )\n";
		errStrm << "hour set to MIN_HOUR (" << MIN_HOUR << "), ";
		errStrm << "minute set to MIN_MINUTE (" << MIN_MINUTE << ")\n";
		std::string errTime = errStrm.str();

		std::invalid_argument ia1(errTime);
		throw ia1;
	}

	// both parameters are valid, update object's data members

	hh = hour;
	mm = minute;



}


//END member function Time::setTime



/****************************************************************************
*                 Time class stream extraction operator                    *
****************************************************************************/

//BEGIN STREAM EXTRACTION OPERATOR
std::istream& operator>>(std::istream &in, Time &t)
{
	// input format is expected to be hh:mm
	// examples:
	// 24:00 or 00:00 or 24:0 or 0:0 for midnight
	// 2:45
	// 02:45
	// 15:30
	// 18:05

	char ch;
	int hour = -1;
	int minute = -1;

	// if stream is originally in a failed throw an exception
	// this is thrown outside of the try block so it will not be
	// handled locally, but will immediately pass control back
	// to the calling function

	std::string errInStream = "ERROR: in Time class overloaded operator>>\n";
	errInStream += "the input stream is in a failed state, no processing can take place...\n";


	if (!in)
	{
		throw std::invalid_argument(errInStream);
	}

	try
	{
		bool streamFailed = false;

		in >> hour; //extract the hours and store them in local variable
		if (!in) { streamFailed = true; }

		in >> ch; //extract and discard the dilimiting character
		if (!in) { streamFailed = true; }

		in >> minute; //extract the minutes and store them in local variable
		if (!in) { streamFailed = true; }

		// if stream entered a failed state while extracting values
		// throw an exception
		// this is thrown inside of the try block so it will be
		// handled locally
		// in this case, local processing does not rethrow

		if (streamFailed)
		{
			std::string errInStream = "WARNING: in Time class overloaded operator>>\n";
			errInStream += "the input stream is in a failed state, ";
			errInStream += "possible attempt to read past end of file, ";
			errInStream += "time object data members no updated ...\n";
			throw std::invalid_argument(errInStream);
		}

		t.setTime(hour, minute); // call setTime method where validation will take place
	}
	catch (std::invalid_argument ia1)
	{
		std::cout << "WARNING: ";
		std::cout << "caught invalid_argument exception in Time class overloaded operator>>\n";
		std::cout << ia1.what() << endl;
		// allow processing to continue, do not re-throw
	}
	catch (...)
	{
		std::cout << "unknown exception caught in Time class overloaded operator>>\n";
		throw;
	}

	return in;
}
//END overloaded stream extraction operator for Time class


/****************************************************************************
*                 Time class stream insertion operator                     *
****************************************************************************/

//BEGIN STREAM INSERTION OPERATOR
std::ostream& operator<<(std::ostream &os, const Time &t)
{
	os << std::setfill('0');

	os << std::setw(2) << t.hh << ":";
	os << std::setw(2) << t.mm;

	os << std::setfill(' ');

	return os;

} 
//END overloaded stream insertion operator for Time class



  /****************************************************************************
  *                     Time class assignment operator                       *
  ****************************************************************************/

  //BEGIN TIME CLASS OVERLOADED ASSIGNMENT OPERATOR
Time Time::operator=(const Time &rhsObj)
{
	if (this != &rhsObj)
	{
		// not a self assignment, OK to proceed
		// call method setTime where validation will be accomplished

		setTime(rhsObj.hh, rhsObj.mm);
	}

	return *this;
}
//END overloaded assignment operator for Time class








ProcessTimer::ProcessTimer()

{

	timeStart = 0;

	timeEnd = 0;

}



ProcessTimer::ProcessTimer(clock_t tmSt)

{

	timeStart = tmSt;

	timeEnd = 0;

}



ProcessTimer::~ProcessTimer()

{

	std::cout << "Inside destructor for ProcessTimer object" << std::endl;

}



std::ostream& operator<<(std::ostream &os, const ProcessTimer &tmStmp)

{

	std::cout << "Start time:\t" << tmStmp.timeStart << "\n";

	std::cout << "End time:\t" << tmStmp.timeEnd << "\n";

	std::cout << "Elapsed time:\t";

	std::cout << (tmStmp.timeEnd - static_cast<double>(tmStmp.timeStart)) / CLOCKS_PER_SEC;

	std::cout << "\n";

	std::cout << std::endl;



	return os;

}



void ProcessTimer::setTimeStart(clock_t tmStart)
{
	timeStart = tmStart;
}



void ProcessTimer::setTimeEnd(clock_t tmEnd)
{
	timeEnd = tmEnd;
}



clock_t ProcessTimer::getTimeStart()
{
	return timeStart;
}



clock_t ProcessTimer::getTimeEnd()
{
	return timeEnd;
}



double ProcessTimer::getTimeElapsed()

{
	return static_cast<double>((timeEnd - static_cast<double>(timeStart)) / CLOCKS_PER_SEC);
}