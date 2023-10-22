#include "Timer.hpp"


Timer::Data::Data(std::string functionName)
{
	m_d_functionName = functionName;
	m_d_index = 0;
}

Timer::Timer()
{
	initTimer();
}

inline Timer& Timer::instance()
{
	static Timer t;
	return t;
}

void Timer::begin(std::string func)
{
	std::unordered_map<std::string, Data>::iterator it = instance().m_functionList.find(func);
	if (it == instance().m_functionList.end()) {
		instance().m_functionList.insert(std::pair<std::string, Data>(func, Data(func)));
		it = instance().m_functionList.find(func);
	}
	it->second.m_d_startTime = std::chrono::high_resolution_clock::now();
}

void Timer::end(std::string func)
{
	std::unordered_map<std::string, Data>::iterator it = instance().m_functionList.find(func);
	if (it != instance().m_functionList.end()) {
		it->second.m_d_endTime = std::chrono::high_resolution_clock::now();
		it->second.m_d_duration = it->second.m_d_endTime - it->second.m_d_startTime;
		it->second.m_d_index++;
	}
	else {
		std::cout << "Error: This function '" << func << "' is not defined" << std::endl;
	}
}

void Timer::printReport()
{
	instance().m_Program->m_d_endTime = std::chrono::high_resolution_clock::now();
	instance().m_Program->m_d_duration = instance().m_Program->m_d_endTime - instance().m_Program->m_d_startTime;
	Logger::log(LOG_MESSAGE, "Timer Output", 1, instance().timeForFile);
	#if defined  _DEBUG
	std::cout << "The total time of the program was: " << instance().m_Program->m_d_duration.count() * 1000.0f << "ms" << std::endl;
#else 
	std::cout << "The total time of the program was: " << instance().m_Program->m_d_duration.count() * 1000000.0f << "us" << std::endl;
#endif

	float totalPercentage = 0.0f;
	float tempPercentage = 0.0f;

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
	Logger::log(LOG_MESSAGE, temp, 1, instance().timeForFile);
	for (it = instance().m_functionList.begin(); it != instance().m_functionList.end(); it++)
	{
		pair = createReport(it->first, it->second);
		Logger::log(LOG_MESSAGE, pair.first, 1, instance().timeForFile);
		totalPercentage += pair.second;
	}

	pair = createReport(instance().m_Program->m_d_functionName, *instance().m_Program);
	Logger::log(LOG_MESSAGE, pair.first, 1, instance().timeForFile);

	if (totalPercentage != 100.0f)
	{
		char unacountedPercent[] = "Unaccounted Percentages: ";
		std::string s_percentage = std::to_string(100.0f - totalPercentage);
		std::string percentageOutput = name;
		s_percentage.insert(0, unacountedPercent);
		int percentLength = (int)strlen(s_percentage.c_str());
		percentLength = 103 - percentLength;
		percentLength /= 2;
		percentageOutput.insert(percentLength, s_percentage);
		percentageOutput.erase(106);
		percentageOutput.append(" [" + instance().timeForFile + "] ");
		Logger::log(LOG_WARN, percentageOutput, 1, instance().timeForFile);
	}
}

std::pair<std::string,float> Timer::createReport(std::string key, Data m_Data)
{
	int length = 0;
	float tempPercentage = 0.0f;
	std::string msg = "                                                                                                      ";
	tempPercentage = (m_Data.m_d_duration.count() / instance().m_Program->m_d_duration.count()) * 100;
	length = (int)strlen(key.c_str());
	length = 22 - length;
	length /= 2;
	msg.insert(length, key);
	msg[21] = '|';
#if defined  _DEBUG
	length = (int)strlen(std::to_string(m_Data.m_d_duration.count() * 1000.0f).c_str());
	length = 30 - length;
	length /= 2;
	msg.insert(21 + length, std::to_string(m_Data.m_d_duration.count() * 1000.0f));
#else
	length = (int)strlen(std::to_string(m_Data.m_d_duration.count()).c_str());
	length = 30 - length;
	length /= 2;
	msg.insert(21 + length, std::to_string(m_Data.m_d_duration.count() * 1000000.0f));
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

	msg.append(" [" + instance().timeForFile + "] ");
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
	timeForFile = t;
}

void Timer::initTimer()
{
	m_Program = new Data("Whole Program");
	m_Program->m_d_startTime = std::chrono::high_resolution_clock::now();
	getTimeInString();
}
