#pragma once

//Project Includes
#include "Section.h"

/*
	Menu Class is a "wrapper" class for all central activities of 
	AxysReader Progam and components including:

	- Manages simple cmdline interface for users to read and write files

	-Houses all main enviroment variables line "report," read file, write
	file and config file variables

	-Loads data from .csv flile provided by user (in Axys report format)

	-Writes data in desired format

	-Aggregates and performs subrouties on data to provide user specified output
	
	
	*/



class Menu
{
private:
	
	bool exMenu;
	//determines when to exit main loop, and exit program

	std::string readFile;
	//primary file to read from when reading Axys report

	std::string writeFile;
	//primary file to write too after reading Axys report
	//by default this will be initialized to "report.csv"

	Lookups lookupFiles;

	std::vector<Section> wrapper;
	//The entire .csv doc will wrapped in one sections vector

	std::vector<hf_config> configs;
	std::string SectionsFile;
	std::string flagsFile;
	int levels;

	/*  Other Functions -- Private  */
	void setFiles();
	void setFileHelp(std::string& coreFile);

	void initConfigs(const bool readOut = false);
	void configOutputs();
	void configHF();

	void readFiles();
	void aggregate();
	void aggregateTask(const std::vector<Section>::iterator start,
		const std::vector<Section>::iterator end,
		const std::stringstream& time, const double& sp500);

	void writeFiles();
	std::ifstream& readThrough(std::ifstream& is, std::string& brk);

public:

	/*  Constructors  */
	Menu();

	/*  Other Functions  */
	void mainMenu(int opt);

	const bool exit() const;


};

