# ReadMe
Quick little program that enables you to see how long a function took to execute along with creating a log functionality. 
# How to use
Take the Header of the Logger file and the Header and CPP file for the Timer and place them into your project then use them as shown below.
The general use of these files is to keep track of how long functions take along with logging important events. An example use case for the Timer would be 
```c++
void renderObject(std::vector<Objects> objs)
{
	Timer::Begin("renderObjects");
	for(Objects obj : objs)
	{
		obj.draw();
	}
	Timer::End("renderObjects");
}
```
Where as the Logger may be more useful in this use case
```c++
void correctAmount(int a)
{
	int amount = 52;
	if(amount == a)
		Logger::log(LOG_MESSAGE, "The input was the correct amount of 52", 0);
	else
	{
		std::string temp = "The input was not the correct amount of 52 it was: ";
		temp.append(std::to_string(a));
		Logger::log(LOG_ERROR, temp, 1, "ERROR_LOG");
	}
}
```
However they are also used together so at the end during the destructor of the Timer the Logger is called to output all of the Timers that were generated and what their final times where.
*Note it is dangerous to leave a timer not closed because that may skew the time that it reports at the very end.*
*Note 2 the Timer should not be looped upon itself because that can cause errant result as shown in [LogOutput](https://github.com/insidetheboxgames/LoggerandTimer/blob/Debugging/TimerandLogger/LogOutput/Oct_23_17.38.29_2023.txt)*
## Timer:
### Main Functionality:
The main use case of the timer class is to time how long a function take to execute. It will track all functions that you place a Timer::begin and a Timer::end into.
#### Instance:
The instance function provides the static functionality of the program. By giving it a instance to work from it can manipulate and change variables without having to declare an object.
#### Begin
Finds if this function already exists. If it does not then it inserts a pair into the map for the inputted function name and the data. then it sets the instance to see if another instance of that name exists.
#### End
Works like the Begin function. Checks if the function exists in the map. Then it sets the end Time and then calculates the duration. It also increases the index of the function.
#### getTimeInString
Gets the time into a format that works for strings. Uses ctime_s to convert into a character array and then is assigned to a string. The string is then cut down and eventually the new line character is erased.
#### initTimer
This is the first creation of the time in which it creates the whole program timer, and sets its start time. It then calls the getTimeInString function so the filename can be consistent.

#### createReport
generates the look of the report when it is being outputted to a file. The way it works is that it takes in the key or the name of the function, and the data related to the function. It first takes the string length of the key. Then by subtracting by 22 and dividing by 2. When you insert at the newly calculated length should put it within 1-2 characters of the middle of the 22 character buffer laid out for function names.(While not a strict requirement it looks good for it to be less than 22 characters). Then depending if you are in debug or release mode determines how your duration will be outputted. In release it is more meaning full for small functions to be outputted in microseconds versus in milliseconds which is how it is outputted in debug mode. The same taking the length of the string subtracting by the buffer built into it and then dividing by two also applies. This function returns a std::pair of a string and float when the string is the message that will be logged and the float is the percentage that this function took up.
#### printReport
This function gets the end of the time for the whole programs clock. It outputs the total duration to the console while then going on to iterate through the unordered map for function instances and creating their log output. It also calculates the main message for the entire program through createReport. Finally it calculates how much of the time was spent on things that were not able to be calculated, outputting the percentage offset.

## Logger:
#### Dependence disclaimer
This file is windows only because of the windows.h dependence however this is only because I wanted colored output in the console. To get rid of this dependence in Logger.hpp you need to remove the Handle declaration, the SetConsoleTextAttribute, the Dword, WriteConsoleA, and the SetConsoleTextAttribute lines and this can instead be replaced with a std::cout or what ever method out print out you would like.
#### Main Functionality:
This file takes in a priority, message, the output type and at times a file name. This is then translated into a log output to either a file or to the console.
#### getTime
This is used to get the current time but with the month day and year attached to better distinguish when items are happening. The functions in the main file do not provide with enough time to cover a single second however in larger projects it does have that capability to log over larger expanses of time and distinguishing between the time when reading over the log File.
#### Instance
Works the same as the one in the Timer class. Just helps so I don't have to declare an official object every time I want to use the Logger functionality.
#### Log
The main log function takes in the message priority, along message string and then the Boolean output type and occasionally the filename. If the outputType input is set to 0 then the output is to be in the console. When outputting to the console it creates the console handle, sets the color of the text through the console handle. Then it appends the message at the beginning with the correct priority header. At the very end of the string it adds the time in which this was outputted and then it outputs it to the console. Otherwise it will be outputted to a file. When using time as your file name you need to remove the ' ' and the ':' characters. This table replaces  Then it opens the file makes sure its open and then based on the message priority will decide what header it should put on the message. Then it makes sure no extra new line characters were added and if one was it erases it. Finally it outputs to the file.
