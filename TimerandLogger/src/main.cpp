#include"Logger.hpp"
#include"Timer.hpp"
#include<iostream>
#include <math.h>


void line(int x0, int x1, int y0, int y1)
{
	Timer::begin("Line");
	int dx = x1 - x0;
	int dy = y1 - y0;
	int x = x0;
	int y = y0;
	int point = 2*dy - dx;

	while (x < x1)
	{
		if (point >= 0)
		{
			y++;
			point = point + 2 * dy - 2 * dy;
		}
		else
		{
			point = point + 2 * dx;
		}
		x++;
	}
	Timer::end("Line");
}


int* divisionByRemainder(int a, int b)
{
	Timer::begin("Division by Remainder");
	int divisor = 0;
	while (a < b)
	{
		a -= b;
		divisor++;
	}
	int result[2];
	result[0] = divisor;
	result[1] = a; 
	Timer::end("Division by Remainder");
	return result;
}


int Summation(int n)
{
	Timer::begin("Summation");

	int result = 0;
	for (int i = n; i >= 0; i--)
	{
		result += i;
	}
	Timer::end("Summation");
	return result;
}
int tSummation(int n)
{
	Timer::begin("tSummation");

	int result = 0;
	for (int i = n; i >= 0; i--)
	{
		result += i;
	}
	Timer::end("tSummation");
	return result;
}


int main() {
	Summation(1000000);
	divisionByRemainder(9845162, 50);
	Summation(1000000);
	divisionByRemainder(9845162, 50);
	Summation(1000000);
	divisionByRemainder(9845162, 50);
	Summation(1000000);
	line(50, 80, 30, 60);
	line(10, 80, 15, 60);
	line(60, 20, 12, 60);
	
	std::string t = "This is a test " + std::to_string(10574) + std::to_string(15745);
	Timer::setTimeForFile(Logger::instance().initTime);

	Logger::log(LOG_CRITICAL, t,1);
	Logger::log(LOG_DEBUG, t,1);
	Logger::log(LOG_ERROR, t,1);
	Logger::log(LOG_MESSAGE, t,1);
	Logger::log(LOG_WARN, t,1);

	
	return 0;
}