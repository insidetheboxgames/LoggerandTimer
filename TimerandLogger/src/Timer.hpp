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
		float averageCreator = 0;

		//Key
		std::string m_d_functionName;
		Data(std::string functionName)
		{
			m_d_functionName = functionName;
			m_d_index = 1;
		};
	};

public:

	Timer()
	{
		initTimer();
	};

	~Timer()
	{
		printReport();
	}
	inline static Timer& instance();

	static void setTimeForFile(std::string time)
	{
		instance().m_timeForFile = time;
	}


	static void begin(std::string func)
	{
		std::unordered_map<std::string, Data>::iterator it = instance().m_functionList.find(func);
		if (it == instance().m_functionList.end()) {
			instance().m_functionList.insert(std::pair<std::string, Data>(func, Data(func)));
			it = instance().m_functionList.find(func);
		}
		it->second.m_d_startTime = std::chrono::high_resolution_clock::now();
	};
	static void end(std::string func)
	{
		std::unordered_map<std::string, Data>::iterator it = instance().m_functionList.find(func);
		if (it != instance().m_functionList.end()) {
			it->second.m_d_endTime = std::chrono::high_resolution_clock::now();
			it->second.m_d_index++;
			it->second.m_d_duration = (it->second.m_d_endTime - it->second.m_d_startTime);
			it->second.averageCreator += it->second.m_d_duration.count();
		}
		else {
			std::cout << "Error: This function '" << func << "' is not defined" << std::endl;
		}
	};
	static void printReport();

private:
	static std::pair <std::string, float> createReport(std::string key, Data m_Data);
	void getTimeInString();
	std::string m_timeForFile;
	float m_totalPercentage = 0;
	void initTimer();
	Data* m_Program;
	std::unordered_map<std::string, Data> m_functionList;
};

inline Timer& Timer::instance()
{
	static Timer t;
	return t;
}

void Timer::printReport()
{
	instance().m_Program->m_d_endTime = std::chrono::high_resolution_clock::now();
	instance().m_Program->m_d_duration = (instance().m_Program->m_d_endTime - instance().m_Program->m_d_startTime);
	Logger::log(LOG_MESSAGE, "Timer Output", 1, instance().m_timeForFile);
	float totalPercentage = 0.0f;

	std::unordered_map<std::string, Data>::iterator it;
	//21 52 79 101
	std::string temp = "    Function Name    |        Total Time(ms)        |        Percentage        |        Index        |";
#if defined NDEBUG
	temp[41] = 'u';
#endif
	std::string msg;
	char name[103] = "                                                                                                      ";
	int length = 0;
	std::pair<std::string, float> pair;
	Logger::log(LOG_MESSAGE, temp, 1, instance().m_timeForFile);
	for (it = instance().m_functionList.begin(); it != instance().m_functionList.end(); it++)
	{
		pair = createReport(it->first, it->second);
		Logger::log(LOG_MESSAGE, pair.first, 1, instance().m_timeForFile);
		totalPercentage += pair.second;
	}

	pair = createReport(instance().m_Program->m_d_functionName, *instance().m_Program);
	Logger::log(LOG_MESSAGE, pair.first, 1, instance().m_timeForFile);

	if (totalPercentage != 100.0f)
	{
		char unacountedPercent[] = "Unaccounted Percentages: ";
		std::string s_UnPercentage = std::to_string(100.0f - totalPercentage);
		std::string s_totPercentage = std::to_string(totalPercentage);

		s_totPercentage.insert(0, "Total Timed Percentage: ");
		s_UnPercentage.insert(0, unacountedPercent);

		std::string totalTimedPercentage = name;
		std::string percentageOutput = name;

		int unPercentLength = (int)strlen(s_UnPercentage.c_str());
		int totPercentLength = (int)strlen(s_totPercentage.c_str());

		unPercentLength = 103 - unPercentLength;
		totPercentLength = 103 - totPercentLength;
		unPercentLength /= 2;
		totPercentLength /= 2;
		percentageOutput.insert(unPercentLength, s_UnPercentage);
		totalTimedPercentage.insert(totPercentLength, s_totPercentage);
		percentageOutput.erase(106);
		totalTimedPercentage.erase(105);
		percentageOutput.append(" [" + instance().m_timeForFile + "] ");
		totalTimedPercentage.append(" [" + instance().m_timeForFile + "] ");
		Logger::log(LOG_DEBUG, totalTimedPercentage, 1, instance().m_timeForFile);
		Logger::log(LOG_WARN, percentageOutput, 1, instance().m_timeForFile);
	}
}

std::pair<std::string, float> Timer::createReport(std::string key, Data m_Data)
{
	if (m_Data.averageCreator == 0)
		m_Data.averageCreator = m_Data.m_d_duration.count();
	int length = 0;
	float tempPercentage = 0.0f;
	std::string msg = "                                                                                                      ";
	tempPercentage = ((m_Data.m_d_duration.count() * m_Data.m_d_index) / (instance().m_Program->m_d_duration.count())) * 100;
	instance().m_totalPercentage += tempPercentage;
	length = (int)strlen(key.c_str());
	length = 22 - length;
	length /= 2;
	msg.insert(length, key);
	msg[21] = '|';
#if defined  _DEBUG
	length = (int)strlen(std::to_string(m_Data.m_d_duration.count() * m_Data.m_d_index * 1000.0f).c_str());
	length = 30 - length;
	length /= 2;
	msg.insert(21 + length, std::to_string(m_Data.m_d_duration.count() * m_Data.m_d_index * 1000.0f));
#else
	length = (int)strlen(std::to_string(m_Data.m_d_duration.count() * m_Data.m_d_index * 1000000.0f).c_str());
	length = 30 - length;
	length /= 2;
	msg.insert(21 + length, std::to_string(m_Data.m_d_duration.count() * m_Data.m_d_index * 1000000.0f));
#endif
	msg[52] = '|';
	length = (int)strlen(std::to_string(tempPercentage).c_str());
	length = 27 - length;
	length /= 2;
	msg.insert(53 + length, std::to_string(tempPercentage));
	msg[79] = '|';
	length = (int)strlen(std::to_string(m_Data.m_d_index).c_str());
	length = 22 - length;
	length /= 2;
	msg.insert(80 + length, std::to_string(m_Data.m_d_index));
	msg.erase(103);
	msg[101] = '|';

	msg.append(" [" + instance().m_timeForFile + "] ");
	return std::pair<std::string, float>(msg, tempPercentage);
}

void Timer::getTimeInString()
{
	auto start = std::chrono::system_clock::now();
	auto legacyStart = std::chrono::system_clock::to_time_t(start);
	char tmBuff[30];
	ctime_s(tmBuff, sizeof(tmBuff), &legacyStart);
	std::string t = tmBuff;
	t = t.substr(4, 29);
	t.erase(t.find("\n"));
	m_timeForFile = t;
}

void Timer::initTimer()
{
	m_Program = new Data("Whole Program");
	m_Program->m_d_startTime = std::chrono::high_resolution_clock::now();
	getTimeInString();
}

#endif // TIMER_HEADER_CLASS