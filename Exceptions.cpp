/*
	Exceptions.cpp
	Author: Jack Welsh
	Implementation code for exceptions.h
*/

#include "Exceptions.h"

file_open_error::file_open_error(std::string err) : invalid_argument(err)
{

}

bad_date_component::bad_date_component(std::string err) : invalid_argument(err)
{

}

missing_arguments::missing_arguments(std::string err) : logic_error(err)
{

}

bad_mine_ID::bad_mine_ID(std::string err) : invalid_argument(err)
{

}

bad_mine_status::bad_mine_status(std::string err) : invalid_argument(err)
{

}

bad_mine_flagMC::bad_mine_flagMC(std::string err) : invalid_argument(err)
{

}

no_such_object::no_such_object(std::string err) : logic_error(err)
{

}