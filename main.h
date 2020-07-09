#pragma once

//Project Includes
#include "Exceptions.h"
#include "DateTime.h"
#include "DataRow.h"
#include "Section.h"

/*
	"AxysReader" C++ program to convert Excel Friendly Axys Finance report
	into desirable format for .dbf file usable by MS Access

	Author: Jack H. Welsh, 7/6/2020

	Version Info:
		Visual Studio 2019 v142
		Windows SDk 10.0, written in x86 for 32-bit OS and above

*/


/*
	//Using
	using std::string;
	using std::cout;
	using std::endl;
	using std::ifstream;
	using std::ostream;
	using std::istream;
	using std::cin;
	using std::vector;

*/

//Forward Function Declarations
void loadData();
void initObjects();