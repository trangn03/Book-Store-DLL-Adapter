/**
 * Copyright 2022 Mike Peralta
 * All Rights Reserved, except my students, who may use this in their homework
 * 
 * Better license coming soon.
 */

//
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


//
#include "json.hpp"


//
namespace PuhPTests
{
	/**
	 * Bring std::to_string into this namespace,
	 * so we can overload it
	 */
	using std::to_string;
	/**
	 * Overload to_string in this namespace so we can use it on vectors
	 * You can add your own to_string overloads in this namespace to help
	 * 	the tests properly render your items when needed.
	 */
	template <class T>
	std::string to_string(const std::vector<T>& v) {
		
		std::stringstream ss;
		bool first = true;
		
		ss << "{";
		for (auto item : v) {
			if (first) {
				first = false;
			}
			else{
				ss << ", ";
			}
			ss << "\"" << item << "\"";
		}
		ss << "}";
		
		return ss.str();
	}
	/// lol
	std::string to_string(const std::string& s) {
		return s;
	}
	
	/**
	 * Enumeration so we can name different types of log entries
	 */
	enum class LogEntryType
	{
		None,
		Any,
		Info,
		Pass,
		Fail,
		Warning,
		Error
	};
	
	/**
	 * Class to hold a single log entry.
	 */
	class LogEntry
	{
		//
		public:
		
			LogEntry(std::string message, LogEntryType type = LogEntryType::Info)
				: message_(message), type_(type)
			{}
			
			LogEntryType getType() const {
				return this->type_;
			}
			
			/**
			 * Render this log entry as a string appropriate for printing.
			 */
			std::string toString() const {
				
				std::string tString = LogEntry::typeToString(this->type_);
				if ( this->type_ == LogEntryType::Error || this->type_ == LogEntryType::Fail ) {
					tString = "***" + tString + "***";
				}
				
				return "[" + tString + "] " + this->message_;
			}
			
			/**
			 * Convert a LogEntryType enumeration value to a string, to aid printing.
			 */
			static std::string typeToString(LogEntryType t) {
				switch(t)
				{
					case LogEntryType::None:
						return "None";
					case LogEntryType::Any:
						return "Any";
					case LogEntryType::Info:
						return "Info";
					case LogEntryType::Pass:
						return "Pass";
					case LogEntryType::Fail:
						return "Fail";
					case LogEntryType::Warning:
						return "Warning";
					case LogEntryType::Error:
						return "Error";
				}
				throw std::logic_error("Somehow made it to the end of the switch in PuhPTests::LogEntry::typeToString");
			}
			
		private:
			std::string message_;
			LogEntryType type_;
	};
	
	/**
	 * This class attempts to capture output from cin and cerr (stdout and stderr)
	 * As of 2022-03-09, it won't capture output from printf()
	 * Begins capturing on creation.
	 */
	class OutputCapture
	{
		//
		public:
			
			//
			OutputCapture(bool defer_capture = false)
			{
				if ( !defer_capture ) {
					this->beginCapture();
				}
			}
			
			~OutputCapture()
			{
				this->endCapture();
			}
			
			/// Fetch the stdout output that has been captured so far
			std::string getStdout()
			{
				this->flush();
				return this->stdout_buffer.str();
			}
			/// Fetch the stderr output that has been captured so far
			std::string getStderr()
			{
				this->flush();
				return this->stderr_buffer.str();
			}
			
			/// Start capturing stdout and stderr
			void beginCapture()
			{
				this->endCapture();
				
				this->stdout_buffer.str("");
				this->old_stdout = std::cout.rdbuf(this->stdout_buffer.rdbuf());
				std::cout.rdbuf(this->stdout_buffer.rdbuf());
				
				this->stderr_buffer.str("");
				this->old_stderr = std::cerr.rdbuf(this->stderr_buffer.rdbuf());
				std::cerr.rdbuf(this->stderr_buffer.rdbuf());
			}
			
			/// Stop capturing stdout and stderr (but keep a copy of what was captured so far).
			void endCapture()
			{
				//
				this->flush();
				
				if ( this->old_stdout != nullptr ) {
					std::cout.rdbuf(this->old_stdout);
					this->old_stdout = nullptr;
				}
				if ( this->old_stderr != nullptr ) {
					std::cerr.rdbuf(this->old_stderr);
					this->old_stderr = nullptr;
				}
			}
			
			/// Make sure cout and cerr have been flushed, so they're actually captured by this class.
			void flush()
			{
				std::cout << std::flush;
				std::cerr << std::flush;
			}
			
		private:
			
			//
			std::stringstream stdout_buffer;
			std::stringstream stderr_buffer;
			
			//
			std::streambuf* old_stdout = nullptr;
			std::streambuf* old_stderr = nullptr;
	};
	
	/**
	 * This class represents one test case.
	 * A test case can contain multiple assertions and point adjustments; It's just a way to logically group things.
	 */
	class Test
	{
		public:
			
			//
			static constexpr const char * DEFAULT_LABEL = "NOLABEL";
			
			//
			Test(std::string label, int points = 0, int points_possible = 0)
				: label_(label), points_(points), points_possible_(points_possible)
			{
				//this->log("Test instance created");
			}
			Test(const Test& other) {
				this->copyOther(other);
				//this->log("Test instance copying from existing instance: " + other.label_);
			}
			
			//
			std::string getLabel() const { return this->label_; }
			
			/**
			 * Grants points if the passed boolean evaluates to true
			 */
			void assertTrue(bool b, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				if ( b == true ) {
					this->adjustPoints(points, label + " :: TRUE");
				}
				else {
					this->log(
						LogEntry(
							label + " :: Unable to award " + std::to_string(points) + " points; Assertion was FALSE",
							LogEntryType::Fail
						)
					);
				}
			}
			
			/**
			 * Grants points if the passed boolean evaluates to false
			 */
			void assertFalse(bool b, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				if ( b == false ) {
					this->adjustPoints(points, label + " :: Assertion is false");
				}
				else {
					this->log(
						LogEntry(
							label + " :: Unable to award " + std::to_string(points) + " points; Assertion was not false",
							LogEntryType::Fail
						)
					);
				}
			}
			
			/**
			 * Grant points is the two items are "equal"
			 * 
			 * Equality is determined by the existing comparison operator between the two types,
			 * 	which you'll want to make sure are properly overloaded/defined.
			 * 
			 * Works with any two classes that have properly implemented comparison operators.
			 * 
			 * Will also attempt to print out the two items using to_string.
			 * Above, std::to_string is brought into the current namespace,
			 * 	which means you can overload simply "to_string" in this namespace to add support
			 * 	for additional types. See the overload for std::vector near the top of this source file.
			 */
			template<class T1, class T2>
			void assertEqual(T1 a, T2 b, int points = 0, std::string label = Test::DEFAULT_LABEL)
			{
				//
				bool is_equal = a == b;
				std::string comparison_label = (is_equal ? " == " : " != ");
				
				//	Only print the two items if they're not equal
				if ( !is_equal ) {
					label += " (<<<" + to_string(a) + ">>>" + comparison_label + "<<<" + to_string(b) + ">>>" + ")";
				}
				
				// Use to_string instead of std::to_string so we have access to overloads
				this->assertTrue(is_equal, points, label);
			}
			
			/**
			 * Attempts to run the passed lambda "func",
			 * and grants points if an exception DOES OCCUR.
			 */
			void assertException(std::function<void()> func, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				//
				bool caught_exception = false;
				try
				{
					func();
				}
				catch( const std::exception& e )
				{
					this->adjustPoints(points, label + " :: Caught exception ==> " + e.what());
					caught_exception = true;
				}
				catch( ... )
				{
					this->adjustPoints(points, label + " :: Caught exception of unrecognized type");
					caught_exception = true;
				}
				
				//
				if ( caught_exception == false ) {
					this->log(
						LogEntry(
							label + " :: Unable to award " + std::to_string(points) + " points; No exception caught!",
							LogEntryType::Fail
						)
					);
				}
			}
			
			/**
			 * Attempts to run the passed lambda "func",
			 * and grants points if an exception does not occur.
			 */
			void assertNoException(std::function<void()> func, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				//
				bool caught_exception = false;
				std::string exception_message = "";
				try
				{
					func();
				}
				catch(const std::exception& e)
				{
					caught_exception = true;
					exception_message = e.what();
				}
				catch( ... )
				{
					caught_exception = true;
					exception_message = "Unknown exception type";
				}
				
				//
				if ( caught_exception == false ) {
					this->adjustPoints(points, label + " :: No exception caught.");
				}
				else {
					this->log(
						LogEntry(
							label + " :: Unable to award " + std::to_string(points) + " points; Exception caught: " + exception_message,
							LogEntryType::Fail
						)
					);
				}
			}
			
			/// Write a log entry
			void log(const LogEntry& entry) {
				this->logs.push_back(entry);
				std::cout << "[" << this->label_ << "]" << entry.toString() << std::endl;
			}
			/// Write a log entry
			void log(const std::string& s) {
				LogEntry entry(s, LogEntryType::Info);
				this->log(entry);
			}
			
			/// Assignment operator overload
			Test& operator=(const Test& rhs) {
				this->copyOther(rhs);
				return *this;
			}
			
			/**
			 * Adjusts points for this test, with a given reason and log entry type.
			 * Pass LogEntryType::None to let the method decide on Pass/Fail, based on the points.
			 * 
			 * You typically won't need to call this; It will be called by assertion methods above.
			 */
			int adjustPoints(int adjustment, std::string reason, LogEntryType log_type = LogEntryType::None) {
				
				//
				int points_old = this->points_;
				this->points_ += adjustment;
				
				//
				if ( log_type == LogEntryType::None) {
					if (adjustment > 0) {
						log_type = LogEntryType::Pass;
					}
					else if (adjustment < 0) {
						log_type = LogEntryType::Fail;
					}
				}
				this->log(
					LogEntry(
						"Adjusting points " + std::to_string(points_old) + " ==> " + std::to_string(this->points_) + " (" + std::to_string(adjustment) + "): " + reason,
						log_type
					)
				);
				
				return this->points_;
			}
			
			/**
			 * Adjust the total points possible.
			 * 
			 * Typically called by an assertion above, and does not need to be called by user code.
			 */
			int adjustPointsPossible(int points, std::string label) {
				
				//
				if (points > 0) {
					this->points_possible_ += points;
					this->log(label + " :: Adjusting points possible to " + std::to_string(this->points_possible_));
				}
				
				return this->points_possible_;
			}
			
			/// Return the number of points earned
			int getPoints() const { return this->points_; }
			
			/// Return the number of points possible
			int getPointsPossible() const { return this->points_possible_; }
			
			/// Render all logs to a newline-separated string. Useful for printing.
			std::string getLogsAsString(std::vector<LogEntryType> log_types = {LogEntryType::Any}, size_t label_room = 0) {
				
				//
				std::stringstream ss;
				
				//
				std::string label = this->getLabel();
				size_t label_padding = 0;
				if ( label.size() < label_room ) {
					label_padding = label_room - label.size();
				}
				
				//
				bool any = std::find(log_types.begin(), log_types.end(), LogEntryType::Any) != log_types.end();
				for (const LogEntry& entry : this->logs) {
					
					LogEntryType t = entry.getType();
					
					//	Print if we're printing all, or this is the right type of log
					if ( any || std::find(log_types.begin(), log_types.end(), t) != log_types.end() ) {
						
						//	Trying to help Fail logs pop out a bit more
						if ( t == LogEntryType::Fail ) {
							ss << ">>>>>>>>>>" << std::endl;
						}
						
						//	Print the actual log line
						ss << "[" << this->getLabel() << std::string(label_padding, ' ') << "]" << entry.toString() << std::endl;
						
						//	Trying to help Fail logs pop out a bit more
						if ( t == LogEntryType::Fail ) {
							ss << ">>>>>>>>>>" << std::endl;
						}
					}
				}
				
				return ss.str();
			}
			/// Render all logs *of a particular type* to a newline-separated string. Useful for printing.
			std::string getLogsAsString(LogEntryType t, size_t label_room = 0) {
				return this->getLogsAsString({t}, label_room);
			}
			/// Render all Pass/Fail logs to a newline-separated string. Useful for printing to students.
			std::string getPassFailLogsAsString(size_t label_room = 0) {
				return this->getLogsAsString({LogEntryType::Pass, LogEntryType::Fail}, label_room);
			}
			
			/// Render all logs to a JSON object
			nlohmann::json getLogsAsJson() {
				
				//
				nlohmann::json j;
				
				//
				for (const LogEntry& entry : this->logs) {
					j.push_back(entry.toString());
				}
				
				return j;
			}
			
			/// Render this entire test case to a JSON object
			nlohmann::json toJson() {
				return nlohmann::json {
					{"label", this->label_},
					{"logs", this->getLogsAsString() },
					{"points", this->points_},
					{"points_possible", this->points_possible_}
				};
			}
			
			/// Render this entire test case to a JSON object, compatible with Gradescope
			nlohmann::json toGradescopeJson() {
				return nlohmann::json {
					{"score", this->points_},
					{"max_score", this->points_possible_},
					{"output", this->getLogsAsString()},
					{"name", this->label_}
				};
			}
			
		private:
			
			//
			std::string label_;
			int points_;
			int points_possible_;
			std::vector<LogEntry> logs;
			
			/// Helper function to make this test case into a copy of another.
			void copyOther(const Test& other) {
				this->label_ = other.label_;
				this->points_ = other.points_;
				this->points_possible_ = other.points_possible_;
				this->logs = other.logs;
			}
	};
	
	//
	class Tests
	{
		public:
			
			//
			static constexpr int DEFAULT_INDENT = 3;
			static constexpr const char * DEFAULT_RESULTS_FILENAME = "results.json";
			
			// CTOR
			Tests() {}
			
			/// Add a test to this group of tests
			void add(const Test& test)
			{
				this->tests_.push_back(test);
				this->longest_test_label = std::max(this->longest_test_label, test.getLabel().size());
			}
			
			/**
			 * Set to true if you want gradescope to show stdout to students
			 * If this is set to false, it's likely some students will see
			 * a blank page when they submit code that doesn't compile.
			 */
			void setGradescopeShowStdoutToStudents(bool b)
			{
				this->gradescope_show_stdout_to_students_ = b;
			} 
			
			/**
			 * Render a JSON object that contains each test's properties.
			 */
			nlohmann::json getTestsAsJson() const {
				
				//
				nlohmann::json j;
				for ( Test tt : this->tests_) {
					j.push_back(tt.toJson());
				}
				
				return j;
			}
			/**
			 * Render a JSON object in a format compatible with Gradescope,
			 * that contains the results of all tests.
			 */
			nlohmann::json getTestsAsGradescopeJson() const {
				
				//
				nlohmann::json j;
				for ( Test tt : this->tests_) {
					j.push_back(tt.toGradescopeJson());
				}
				
				return j;
			}
			
			/**
			 * Render the results of all tests, along with a test summary, to a JSON object
			 */
			nlohmann::json getResultsAsJson() const {
				
				//
				nlohmann::json results;
				
				//
				results["output"] = this->getTestsSummaryAsString();
				results["tests"] = this->getTestsAsJson();
				
				return results;
			}
			
			/**
			 * Render a JSON object containing all tests and information needed for Gradescope.
			 */
			nlohmann::json getResultsAsGradescopeJson() const {
				
				//
				nlohmann::json results;
				
				//
				results["output"] = this->getTestsSummaryAsString();
				results["tests"] = this->getTestsAsGradescopeJson();
				
				//
				if ( this->gradescope_show_stdout_to_students_ == true ) {
					results["stdout_visibility"] = "visible";
				}
				else {
					results["stdout_visibility"] = "hidden";
				}
				
				return results;
			}
			
			/// Render all test results to a string.
			std::string getResultsAsString() {
				
				//
				nlohmann::json results;
				
				//
				results["tests"] = this->getTestsAsJson();
				
				return results.dump(Tests::DEFAULT_INDENT);
			}
			
			/**
			 * Write the results (output, tests) to a file, in JSON format
			 */
			void writeResultsAsJson(std::string file_path = Tests::DEFAULT_RESULTS_FILENAME ) const {
				
				//
				nlohmann::json results = this->getResultsAsJson();
				
				//
				{
					std::ofstream f(file_path);
					f << results.dump(Tests::DEFAULT_INDENT);
				}
			}
			
			/**
			 * Write the results (output, tests) to a file, in a JSON format compatible with Gradescope
			 * This is typically the method you'll want to call after all tests, inside your auto grader.
			 */
			void writeResultsAsGradescopeJson(std::string file_path = Tests::DEFAULT_RESULTS_FILENAME ) const {
				
				//
				nlohmann::json results = this->getResultsAsGradescopeJson();
				
				//
				{
					std::ofstream f(file_path);
					f << results.dump(Tests::DEFAULT_INDENT);
				}
			}
			
			/**
			 * Print a summary of tests to stdout.
			 * This is useful to help the student get a general idea of where they need to improve.
			 */
			void printTestsSummary() {
				std::cout << this->getTestsSummaryAsString() << std::endl;
			}
			
			/**
			 * Generate a string summary of the tests.
			 * This is useful to help the student get a general idea of where they need to improve.
			 */
			std::string getTestsSummaryAsString() const {
				
				//
				std::stringstream ss;
				
				//
				ss << "***** Tests Summary *****" << std::endl;
				
				//
				int total = 0, total_possible = 0;
				for ( const Test& t : this->tests_ ) {
					
					//
					total += t.getPoints();
					total_possible += t.getPointsPossible();
					
					//
					ss << "[" << t.getLabel() << "] " << t.getPoints() << " / " << t.getPointsPossible() << std::endl;
				}
				
				//
				ss << std::endl;
				ss << "Total: " << total << " / " << total_possible << std::endl;
				
				return ss.str();
			}
			
			/**
			 * Generate a summary of logs, but ONLY logs of type PASS or FAIL
			 * This can be useful to help the student ignore log entries that
			 * 	don't affect their score.
			 */
			std::string getPassFailLogsAsString() const {
				
				std::stringstream ss;
				
				ss << "***** Pass / Fail Logs *****" << std::endl;
				
				for ( auto t : this->tests_ ) {
					ss << t.getPassFailLogsAsString(this->longest_test_label);
				}
				return ss.str();
			}
			
			/**
			 * Overload the stream operator to do appending
			 * Allows you to do something like:
			 * 
			 * Tests tests;
			 * Test t("My test");
			 * tests << t;
			 * 
			 * Alternatively, just use the add() method.
			 */
			Tests& operator<<(const Test& t) {
				
				this->add(t);
				
				return *this;
			}
			
		private:
			
			std::vector<Test> tests_;
			size_t longest_test_label = 0;
			
			bool gradescope_show_stdout_to_students_ = true;
	};
}




