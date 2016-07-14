/*
 * StatsLogger.cpp
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>


#include "StatsLogger.h"



StatsLogger::LoggingColumn::LoggingColumn(std::string name, std::function<std::string()> callback) : name(name), callback(callback){
}

StatsLogger::StatsLogger(){
}

void StatsLogger::registerLoggingColumn(std::string columnName, std::function<std::string()> callback){
	columns.push_back(LoggingColumn(columnName, callback));
}

void StatsLogger::initLog(std::string file){
	std::ofstream statsFile;
	statsFile.open(file);

	statsFile << columns[0].name;

	for(int i=1;i<columns.size();i++){
		statsFile << ";" << columns[i].name;
	}

	statsFile << std::endl;
}

void StatsLogger::log(std::string file){
	//Log stats
	std::ofstream statsFile;

	statsFile.open(file, std::ios_base::app);//Append

	statsFile << columns[0].callback();

	for(int i=1;i<columns.size();i++){
		statsFile << ";" << columns[i].callback();
	}

	statsFile << std::endl;
}

void StatsLogger::archiveLog(std::string file){
	std::string nameAndPath	= (file.substr(0, file.find_last_of(".") - 1));
	std::string extension	= (file.substr(0, file.find_last_of(".") + 1));

	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer [20];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer, 20, "%Y-%m-%d-%H-%M", timeinfo);
	std::puts(buffer);

	rename(file.c_str(), (nameAndPath + "-" + std::string(buffer) + extension).c_str());
}
