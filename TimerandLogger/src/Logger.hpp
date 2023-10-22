#ifndef LOGGER_HEADER_CLASS
#define LOGGER_HEADER_CLASS

#include<fstream>
#include<iostream>
#include<chrono>
#include<Windows.h>

static enum Log_Priority
{
	LOG_MESSAGE,LOG_WARN,LOG_CRITICAL,LOG_ERROR,LOG_DEBUG
};

class Logger
{
private:

	static void log_Message(const char* message)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_handle, 8);
		std::string t = "[Message]: ";
		t.append(message);
		t.append(" [" + instance().getTime() + "]");
		t.append("\n");
		int length = (int)strlen(t.c_str());
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), t.c_str(), (DWORD)length, &number_written, 0);
		SetConsoleTextAttribute(console_handle, 15);
	};
	
	static void log_Warn(const char* message)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_handle, 14);
		std::string t = "[Warning]: ";
		t.append(message);
		t.append(" [" + instance().getTime() + "]");
		t.append("\n");
		int length = (int)strlen(t.c_str());
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), t.c_str(), (DWORD)length, &number_written, 0);
		SetConsoleTextAttribute(console_handle, 15);
	};
	
	static void log_Critical(const char* message)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		//Critical 4 Message 8 Warn 14 Error 3 Debug 10
		SetConsoleTextAttribute(console_handle, 4);
		std::string t = "[Critical]: ";
		t.append(message);
		t.append(" [" + instance().getTime() + "]");
		t.append("\n");
		int length = (int)strlen(t.c_str());
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), t.c_str(), (DWORD)length, &number_written, 0);
		SetConsoleTextAttribute(console_handle, 15);
	};
	
	static void log_Error(const char* message)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_handle, 3);
		std::string t = "[Error]: ";
		t.append(message);
		t.append(" [" + instance().getTime() + "]");
		t.append("\n");
		int length = (int)strlen(t.c_str());
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), t.c_str(), (DWORD)length, &number_written, 0);
		SetConsoleTextAttribute(console_handle, 15);
	};
	
	static void log_Debug(const char* message)
	{
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_handle, 10);
		std::string t = "[Debug]: ";
		t.append(message);
		t.append(" [" + instance().getTime() + "]");
		t.append("\n");
		int length = (int)strlen(t.c_str());
		DWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), t.c_str(), (DWORD)length, &number_written, 0);
		SetConsoleTextAttribute(console_handle, 15);
	};


	Logger() 
	{
		initTime = getTime();
	};

	~Logger(){}

	std::string getTime()
	{
		auto start = std::chrono::system_clock::now();
		auto legacyStart = std::chrono::system_clock::to_time_t(start);
		char tmBuff[30];
		ctime_s(tmBuff, sizeof(tmBuff), &legacyStart);
		std::string t = tmBuff;
		t = t.substr(4, 29);
		t.erase(t.find("\n"));
		return t;
	};

public:
	std::string initTime;
	//LOG_MESSAGE,LOG_WARN,LOG_CRITICAL,LOG_ERROR,LOG_DEBUG
	static void log(Log_Priority messagePriority, std::string message, bool outPutType, std::string fileName = "")
	{
		if (!outPutType)
		{
			switch (messagePriority)
			{
			case 0:
				log_Message(message.c_str());
				break;
			case 1:
				log_Warn(message.c_str());
				break;
			case 2:
				log_Critical(message.c_str());
				break;
			case 3:
				log_Error(message.c_str());
				break;
			case 4:
				log_Debug(message.c_str());
				break;
			}
		}
		else
		{
			std::string interFileName;
			if (fileName == "")
				interFileName = instance().initTime;
			else
				interFileName = fileName;
			
			while (interFileName.find(32) != -1)
			{
				int whSpaceIndex = (int)interFileName.find(32);
				interFileName[whSpaceIndex] = '_';
			}
			
			while (interFileName.find(58) != -1)
			{
				int colonSpaceIndex = (int)interFileName.find(58);
				interFileName[colonSpaceIndex] = '.';
			}
			interFileName.append(".txt");
			int newLineIndex;
			std::fstream outFile;
			outFile.open("LogOutput/"+interFileName, std::ios::app);
			std::string t;
			if (outFile.is_open())
			{
				switch (messagePriority)
				{
				case 0:
					t = "[Message]: ";
					t.append(message);
					newLineIndex = (int)t.find("\n");
					if(newLineIndex != -1)
						t.erase(newLineIndex);
					outFile << t << "\n";
					break;
				case 1:
					t = "[Warn]: ";
					t.append(message);
					newLineIndex = (int)t.find("\n");
					if (newLineIndex != -1)
						t.erase(newLineIndex);
					outFile << t << "\n";
					break;
				case 2:
					t = "[Critical]: ";
					t.append(message);
					newLineIndex = (int)t.find("\n");
					if (newLineIndex != -1)
						t.erase(newLineIndex);
					outFile << t << "\n";
					break;
				case 3:
					t = "[Error]: ";
					t.append(message);
					newLineIndex = (int)t.find("\n");
					if (newLineIndex != -1)
						t.erase(newLineIndex);
					outFile << t << "\n";
					break;
				case 4:
					t = "[Debug]: ";
					t.append(message);
					newLineIndex = (int)t.find("\n");
					if (newLineIndex != -1)
						t.erase(newLineIndex);
					outFile << t << "\n";
					break;
				}
			}
		}
	}

	inline static Logger& instance()
	{
		static Logger logger;
		return logger;
	}
};

#endif // !LOGGER_HEADER_CLASS