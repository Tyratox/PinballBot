/*
 * StatsLogger.h
 *
 */

#include <functional>
#include <string>

#ifndef STATS_STATSLOGGER_H_
#define STATS_STATSLOGGER_H_

/*
 * A class that holds all the logging columns
 */
class StatsLogger{
	private:
		/**
		 * A struct that holds the name and the callback function of a column
		 */
		class LoggingColumn{
			public:
				std::string name;
				std::function<std::string()> callback;

				/**
				 * Creates a logging column
				 * @param	name			std::string							The name/title of the column
				 * @param	callback		std::function<std::string()>	The callback function
				 */
				LoggingColumn(std::string name, std::function<std::string()> callback);
		};


		std::vector<LoggingColumn> columns;

	public:

		StatsLogger();

		/**
		 * Registers a logging column
		 * @param	columnName		std::string							The name/title of the column
		 * @param	callback		std::function<std::string()>		The callback function
		 * @return void
		 */
		void registerLoggingColumn(std::string columnName, std::function<std::string()> callback);

		/**
		 * Creates the log file and writes the headers
		 * @param	file		std::string		The file to init
		 * @return void
		 */
		void initLog(std::string file);

		/**
		 * Logs the data
		 * @param	file		std::string		The file to log to
		 * @return void
		 */
		void log(std::string file);

		/**
		 * Renames the log file in a archivable way
		 * @param	file		std::string		The file to archive
		 * @return				void
		 */
		void archiveLog(std::string file);


};


#endif /* STATS_STATSLOGGER_H_ */
