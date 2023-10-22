#ifndef TIMER_HEADER_CLASS
#define TIMER_HEADER_CLASS

#include<string>
#include<unordered_map>
#include"Logger.hpp"

class Timer
{
	struct Data
	{
		std::chrono::time_point<std::chrono::steady_clock> m_d_startTime, m_d_endTime;
		std::chrono::duration<float> m_d_duration;
		int m_d_index;

		//Key
		std::string m_d_functionName;
		Data(std::string functionName);
	};

public:

	Timer();

	~Timer()
	{
		printReport();
	}
	inline static Timer& instance();

	static void setTimeForFile(std::string time)
	{
		instance().timeForFile = time;
	}


	static void begin(std::string func);
	static void end(std::string func);
	static void printReport();

private:
	static std::pair <std::string, float> createReport(std::string key, Data m_Data);
	void getTimeInString();
	std::string timeForFile;
	void initTimer();
	Data* m_Program;
	std::unordered_map<std::string, Data> m_functionList;
};
#endif // TIMER_HEADER_CLASS