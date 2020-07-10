#pragma once

//project includes
#include "Exceptions.h"

/*
	Class object "hf_config" will encapsulate configuration variables for
	each "Section" level (Client level, or subsection level), vars include:
	- string : filename
	- string : breaker
	- string : stopper
	- int : headLength (and footLength)
	- bool : hasHeader (and has footer)

*/


class hf_config
{
public:
	int level;
	const static std::string breaker;
	std::string fileName;
	// breaker is "&&&&&" string that delineates 
	//header and footer bounds in SectionConfig

	vector<std::string> keywords;
	//Section name and string of keywords that
	//program looks for to identify headers


	std::string stopper;
	//First line of footer, responsible for stopping dataRow Reads
	//MUST BE GENERAL TO ALL SECTION FOOTERS

	int headLength;
	int footLength;

	bool hasHeader;
	bool hasFooter;

	/*  Constructor  */
	hf_config(hf_config& hf, int lvl);

	hf_config(std::ifstream& toRead, int lvl);


	/*  Initializers  */

	void config(std::ifstream& toRead);

	void initHeader(std::ifstream& is);

	void initFooter(std::ifstream& is);

	void initKeywords(std::ifstream& is);

	/*  Modifiers  */
	void setMembers(hf_config& hf);


	/*  Other Functions  */

	std::ifstream& readThrough(std::ifstream& is, std::string& brk);
};

