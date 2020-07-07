/*
	Exceptions.h                                       
	Author: Jack Welsh                           
 */
#pragma once



#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>


using std::string;
using std::logic_error;


// below is an example of deriving our own exception classes
// they do not have to be very complicated
// implementation code for the constructor is just as simple
// all that has to happen is to call the base class 
// constructor and pass the string argument
//
// put the other derived exception class declarations in this file


class file_open_error : public std::invalid_argument
{
public:
    file_open_error(std::string);
    
}; 
// END file_open_error



class bad_date_component : public std::invalid_argument
{
public:
    bad_date_component(std::string);
    
}; 
// END bad_date_component



class missing_arguments : public logic_error
{
	//function ending exception if  not enough argument in comand line
public:
	missing_arguments(std::string);

};
//END missing_Arguments



class bad_mine_ID : public std::invalid_argument
{
	//function ending exception, this mine will not be appended to vector
public:
	bad_mine_ID(std::string);
};
//END bad_mine_ID



class bad_mine_status : public std::invalid_argument
{
	//function ending exception, this mine will not be appended to vector
public:
	bad_mine_status(std::string);
};
//END bad_mine_status



class bad_mine_flagMC : public std::invalid_argument
{
	//function ending exception, this mine will not be appended to vector
public:
	bad_mine_flagMC(std::string);
};

class no_such_object : public std::logic_error
{
	//function ending exception, this mine will not be appended to vector
public:
	no_such_object(std::string);
};



