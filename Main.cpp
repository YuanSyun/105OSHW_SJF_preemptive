#include "Header.h"
#include "iostream"
using namespace std;

//CONST
const char READ_FILE_NAME[] = "SJF(preemptive)_test.txt";
const char WRITE_FILE_NAME[] = "SJF(preemptive)_test.txt_shcedulingResult.txt";
const int MAX_PROCESS_NUMBER = 100;

//Variable
ProcessClass *processArray[MAX_PROCESS_NUMBER];//ProcessClass point Process[]
int processArray_Index = 0;
SJFP_Scheduling *SJFPSch = new SJFP_Scheduling( MAX_PROCESS_NUMBER );
double *arrivalTimeArray[MAX_PROCESS_NUMBER];

void main() {

	cout << "Main Start" << endl;

	cout << "\nStep1 : Read File" << endl;
	cout << "Only Accept File Name = " << READ_FILE_NAME << endl;
	ReadProcessInfo(READ_FILE_NAME, processArray, &processArray_Index, MAX_PROCESS_NUMBER, SJFPSch);
	
	cout << "\nStep2 : Sort arrival time" << endl;
	Scheduling(processArray, &processArray_Index, SJFPSch);

	cout << "\nStep3 : Write File" << endl;
	WirteExecutionResult(SJFPSch, READ_FILE_NAME, WRITE_FILE_NAME);
	cout << "\nResult File Name = " << WRITE_FILE_NAME << endl;
	
	system("pause");
}//end main