#pragma once
#include <string>
using namespace std;



//Save Process Information
class ProcessClass {
private:
	int id;
	double BurstTime;
	double ArrivalTime;
	double RemaingTime;
public:
	ProcessClass(int _id, double _ArrivalTime,double _BurstTime);
	int get_id();
	double get_BurstTime();
	double get_RemaingTime();
	double get_ArrivalTime();
	void ExecutTime(double _time);
};



//Save Scheduling Information
class SJFP_Scheduling {
private:
	double **ptrExecutionProcess;//double point double[MAX_PROCESS_NUMBER] point double[3]
	int executionIndex;
public:
	SJFP_Scheduling() {};
	SJFP_Scheduling(int);
	SJFP_Scheduling(int _proccessMAXNumber, double _quantum);
	~SJFP_Scheduling();
	void setScheduling(int _process_id, double _startTime, double _endTime);
	double * get_ExecutionProcess(int);
	int get_executionIndex();
};



//Max proccess number
const int MAX = 100;



//Function
void ReadProcessInfo(const char *FileName, ProcessClass **, int *, const int, SJFP_Scheduling *);
void Scheduling(ProcessClass **proccesArrayAddress, int *, SJFP_Scheduling *Scheduling);
void WirteExecutionResult(SJFP_Scheduling *, const char *, const char *);
void showCharArray(char *, int);
void resetCharArray(char *, int);



