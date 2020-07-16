#pragma once
/*
*  DateTime.h
*
*  COSC 052 Summer 2018
*  Project #3
*
*  Due on: JUL #31, 2018
*  Author: Jack Welsh
*
*
*  In accordance with the class policies and Georgetown's
*  Honor Code, I certify that, with the exception of the
*  class resources and those items noted below, I have neither
*  given nor received any assistance on this project.
*
*  References not otherwise commented within the program source code.
*  Note that you should not mention any help from the TAs, the professor,
*  or any code taken from the class textbooks.
*/


#ifndef COSC52Summer2018P1_Date_h
#define COSC52Summer2018P1_Date_h

#include "Exceptions.h"

//#define DEBUG_DATE = 1




/****************************************************************************
****************************************************************************
**                                                                        **
**                     class Date declaration                             **
**                                                                        **
****************************************************************************
****************************************************************************/



struct intToString
{
	//Personal toString method included to make it easier to print out intbers
	static string toString(int number)
	{
		//using std::stringstream to convert ints to strings
		std::stringstream ss;
		string intString;

		ss << number;
		ss >> intString;

		return intString;
	}
};


///*

//*/

const int min_year = 1492;
const int max_year = 2192;

const int min_month = 1;
const int max_month = 12;

const int min_day = 1;
const int max_day = 31;


class Date
{

	friend std::ostream& operator<<(std::ostream &os, const Date &dateToRead);

	friend std::istream& operator>>(std::istream &is, Date &dateToRead);


private:

	enum months { JAN = 0, FEB, MAR, APR, MAY, JUN,
		JUL, AUG, SEP, OCT, NOV, DEC};

	const static int mDays[12];

	int dd, mm, yyyy;							// Integer member variables defining day, month and year

public:

	Date(int year = 1821, int month = 7, int day = 13);		//yyyy, mm, dd format

	Date(std::string newDate);

	Date(const Date &dateToCopy);			//date class copy constructor 

	void setDate(int year, int month, int day);				//yyyy, mm, dd format

	void setDate(const Date &otherDate);

	int getDay() const;				
	//returns integer day of date object

	int getMonth() const;			
	//returns integer month of date object

	int getYear() const;			
	//returns integer year of date object

	std::string getStringDate() const;

	bool operator==(const Date &rhsObj) const;

	bool operator!=(const Date &rhsObj) const;

	bool operator<=(const Date &rhsObj) const;

	bool operator<(const Date &rhsObj) const;

	bool operator>=(const Date &rhsObj) const;

	bool operator>(const Date &rhsObj) const;

	const Date& operator=(const Date &rhsObj);

	Date operator--(int dec);

};










/****************************************************************************
****************************************************************************
**                                                                        **
**                 class Time declaration                                 **
**                                                                        **
**                 This code is provided to students                      **
**                 in COSC 052 Summer 2018 to use in                      **
**                 part or in total for class projects.                   **
**                 Students may use this code as their                    **
**                 own, without any attribution.                          **
**                                                                        **
****************************************************************************
****************************************************************************/

class Time
{
	//Overloaded stream extraction operator
	//
	//Postcondition: Extracts values for each data member from
	//the input stream.  Format of the data in the stream
	//is assumed to be:  hh:mm
	//
	//The delimiting character is read and discarded.
	//
	friend std::istream& operator>>(std::istream &in, Time &t);


	//Overloaded stream insertion operator.
	//
	//Postcondition: The time is output to the steam.  Delimiting
	//characters are replaced and the decimal point is added
	//between the seconds and fraction sections.  The result
	//is a time value output in the same format as was read in.
	//
	friend std::ostream& operator<<(std::ostream &os, const Time &t);


private:

	//two digit integer for hour of day
	int hh;

	//two digit integer for minute of day
	int mm;


public:

	//constructor with parameters, with default values
	//This constructor calls the member function setTime()
	//and passes the same arguments as were passed in to
	//the constructor, function setTime() updates the data members
	//and completes data validation.
	//
	//Postcondition: Sets the values of hh and mm
	//according to the values of the arguments passed in.
	//If no values are passed, this acts as the default constructor
	//all integer data members are set to zero
	//
	Time(int = 0, int = 0);


	//Function to set the values of the object data members,
	//can be called directly or by a constructor.
	//
	//Postcondition: Data members are set to the values
	//of the respective arguments passed in.  Values passed
	//are also validated, if any validation tests fail
	//then the data members are set to default values.
	//Note that this validation strategy is a business rule for
	//our project and would probably not be appropriate for
	//most situations and could result in unintended consequences.
	//
	void setTime(int, int);


	//In-line function to return the hour value.
	//
	//Postcondtion: The value of data member hh is retured.
	//
	int getHour() const { return hh; }

	//In-line function to return the minute value.
	//
	//Postcondtion: The value of data member mm is retured.
	//
	int getMinute() const { return mm; }


	//Overloaded assignment operator.
	//
	//Postcondtion: The values of right hand side object's
	//data members are copied to the respective data
	//members of the left hand side object.
	//The overloaded assignment operator calls setTime
	//so that data being copied are validated.
	//
	Time operator=(const Time&);


}; //END class Time declaration




   /****************************************************************************
   ****************************************************************************
   **                                                                        **
   **                 class ProcessTimer declaration                         **
   **                                                                        **
   **                 This code is provided to students                      **
   **                 in COSC 052 Summer 2018 to use in                      **
   **                 part or in total for class projects.                   **
   **                 Students may use this code as their                    **
   **                 own, without any attribution.                          **
   **                                                                        **
   **                 (Copy and paste into DateTime.h)                       **
   **                                                                        **
   ****************************************************************************
   ****************************************************************************/

class ProcessTimer
{
	friend std::ostream& operator<<(std::ostream&, const ProcessTimer&);

private:
	clock_t timeStart; //start time
	clock_t timeEnd; //end time

public:
	ProcessTimer();
	ProcessTimer(clock_t);
	~ProcessTimer();
	void setTimeStart(clock_t);
	void setTimeEnd(clock_t);
	clock_t getTimeStart();
	clock_t getTimeEnd();
	double getTimeElapsed();

}; //end class ProcessTimer







#endif

