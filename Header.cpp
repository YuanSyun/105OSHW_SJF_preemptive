#include "Header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;



ProcessClass::ProcessClass(int _id, double _ArrivalTime , double _BurstTime) {
	id = _id;
	BurstTime = _BurstTime;
	RemaingTime = _BurstTime;
	ArrivalTime = _ArrivalTime;
}//end ProcessClass Constructor

int ProcessClass::get_id() { return id; };
double ProcessClass::get_BurstTime() { return BurstTime; };
double ProcessClass::get_RemaingTime() { return RemaingTime; };
double ProcessClass::get_ArrivalTime() { return ArrivalTime; };

void ProcessClass::ExecutTime(double _time) {
	RemaingTime = RemaingTime - _time;
}//end ProcessClass ExecuTime



SJFP_Scheduling::SJFP_Scheduling(int _processMAXNumber) {

	//2-D Address to pointer
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[3];
	}
	executionIndex = 0;
}//end RRScheduling Constructor

SJFP_Scheduling::SJFP_Scheduling(int _processMAXNumber, double _quantum) {

	//2-D Address to pointer
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[3];
	}
	executionIndex = 0;
}//end RRScheduling Constructor

SJFP_Scheduling::~SJFP_Scheduling() {}//解構子

void SJFP_Scheduling::setScheduling(int _process_id, double _startTime, double _endTime) {
	ptrExecutionProcess[executionIndex][0] = _process_id;
	ptrExecutionProcess[executionIndex][1] = _startTime;
	ptrExecutionProcess[executionIndex][2] = _endTime;
	executionIndex++;
}

double * SJFP_Scheduling::get_ExecutionProcess(int _index) { return ptrExecutionProcess[_index]; };
int SJFP_Scheduling::get_executionIndex() { return executionIndex; };



void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_processArray_Index, const int MAX_PROCESS_NUMBER, SJFP_Scheduling *_RRSch) {
	//Open File
	try {
		fstream fin;
		fin.open(_FileName, ios::in);
		if (fin.fail()) {
			fin.close();
			cout << "\n#01-Read file opening failed.\n";
			system("pause");
			exit(1);
		}//end if
		cout << "\n#01-Read File Success" << endl;

		//Save Process Information
		const int BUFFER_SIZE = 20;
		char process_id[BUFFER_SIZE] = { NULL };
		char arrival_time[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		
		while (*_processArray_Index < MAX_PROCESS_NUMBER) {
			
			//遇到檔案結尾了
			if (fin.eof()) {break;}

			fin.getline(process_id, BUFFER_SIZE,' ');
			fin.getline(arrival_time, BUFFER_SIZE, ' ');
			fin.getline(burst_time, BUFFER_SIZE, '\n');

			//Save process information to processArray
			_processArray[*_processArray_Index] = new ProcessClass(atoi( process_id ), atof( arrival_time ), atof( burst_time ));

			//Check Save Success
			cout << "Process Id = "<< _processArray[*_processArray_Index]->get_id();
			cout << ", Arrival Time = " << _processArray[*_processArray_Index]->get_ArrivalTime();
			cout << ", Burst Time = "<< _processArray[*_processArray_Index]->get_BurstTime();
			cout << endl;

			//Reset char array
			resetCharArray(process_id, BUFFER_SIZE);
			resetCharArray(arrival_time, BUFFER_SIZE);
			resetCharArray(burst_time, BUFFER_SIZE);

			*_processArray_Index += 1;

		}//end while

		fin.close();
	}
	catch (exception e) {
		cout << "\n#01-ReadProcessInfo : " << e.what() << endl;
	}//end try

}//end ReadProcessInfo



void Scheduling(ProcessClass **_processArray, int*_processArray_Index, SJFP_Scheduling *_RRSch) {
	
	int NotFinshProcessNumber = *_processArray_Index;
	double unitProcessTime = 1.0;
	double currentTime = 0.0;
	int shortestTimeProceessIndex = NULL;
	double shortestReamingTime = 9999;
	double pastTime = -1;
	int pastProcessIndex = -1;

	while (NotFinshProcessNumber > 0) {

		//挑出最小的ReamingTimeProcess
		for (int i = 0; i < *_processArray_Index; i++) {

			//已經到達 && 剩餘時間>0
			if ((currentTime >= _processArray[i]->get_ArrivalTime()) && (_processArray[i]->get_RemaingTime() > 0) ) {

				//是否為最小
				if (_processArray[i]->get_RemaingTime() <= shortestReamingTime) {
					shortestReamingTime = _processArray[i]->get_RemaingTime();
					shortestTimeProceessIndex = i;
				}//end if

			}//end if
		}//end if

		//如果有切換Process
		if (pastProcessIndex != shortestTimeProceessIndex){

			//紀錄執行間隔，第一次不用紀錄
			if (pastProcessIndex != -1) {
				_RRSch->setScheduling(_processArray[pastProcessIndex]->get_id(), pastTime, currentTime);
			}//end 
			
			pastTime = currentTime;
			pastProcessIndex = shortestTimeProceessIndex;

		}//end if

		//執行 unit Process Time
		currentTime += unitProcessTime;
		_processArray[shortestTimeProceessIndex]->ExecutTime(unitProcessTime);

		//最小的Remaing Time process執行完，
		if (_processArray[shortestTimeProceessIndex]->get_RemaingTime() <= 0) {

			//完成一個Process了
			NotFinshProcessNumber -= 1;
			//紀錄執行間隔
			_RRSch->setScheduling(_processArray[shortestTimeProceessIndex]->get_id(), pastTime, currentTime);
			//重設最小Remaing Time Process
			shortestReamingTime = 9999;
			shortestTimeProceessIndex = NULL;
			pastProcessIndex = -1;

		}//end if

	}//end while

}//end Scheduling



void WirteExecutionResult(SJFP_Scheduling *_RRSch, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME) {

	fstream fin(_READ_FILE_NAME, ios::in);
	fstream fout(_WRITE_FILE_NAME, ios::out);

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];

	//複製題目
	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}//end while
	fout << endl;

	for (int i = 0; i < _RRSch->get_executionIndex(); i++) {
		cout << "pId = " << _RRSch->get_ExecutionProcess(i)[0];
		cout << ", start = " << _RRSch->get_ExecutionProcess(i)[1];
		cout << ", end = " << _RRSch->get_ExecutionProcess(i)[2];
		cout << endl;

		fout << _RRSch->get_ExecutionProcess(i)[0];
		fout << " " << _RRSch->get_ExecutionProcess(i)[1];
		fout << " ~ " << _RRSch->get_ExecutionProcess(i)[2];
		fout << endl;
	}//end for

	fin.close();
	fout.close();

}//end WirteExecutionResult



void showCharArray(char *_showchar, int _charsize) {

	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}//end for

}//end showCharArray



void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}//end for
}//end resetCharArray

