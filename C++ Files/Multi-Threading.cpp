// Multi-Threading.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <process.h>
#include <iostream>

using namespace std;

DWORD WINAPI myThread(LPVOID lpParameter) {
	unsigned int& myCounter = *((unsigned int*)lpParameter);

	cout << "Thread ID: " << GetCurrentThreadId() << endl;

	while (myCounter < 0xFFFFFFFF) {
		++myCounter;
	}
	return 0;
}

unsigned int __stdcall mythreadB(void* data)
{
	printf("_beginthreadex %d \n", GetCurrentThreadId());
	return 0;
}

void mythreadC(void* data)
{
	printf("_beginthread %d \n", GetCurrentThreadId());
}

void main(int argc, char* argv[]) {
	unsigned int myCounter = 0;
	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, myThread, &myCounter, 0, &myThreadID);

	/*
		The methods _beginthreadex and _beginthread are thread methods from the runtime
		library called process.h. Using them is recommended over using CreateThread
	*/
	HANDLE myhandleB = (HANDLE)_beginthreadex(0, 0, &mythreadB, 0, 0, 0);

	//When calling WaitForSingleObject, it waits for the current thread to finish up before
	//It terminates. You can also call WaitForMultipleObjects, which you know what it does

	//WaitForMultipleObjects(2, myhandle, true, INFINITE);

	WaitForSingleObject(myhandleB, INFINITE);
	HANDLE myhandleC = (HANDLE)_beginthread(&mythreadC , 0, 0);
	char myChar = ' ';

	while (myChar != 'q') {
		cout << myCounter << endl;
		myChar = getchar();
	}

	//Call CloseHandle method to clean up the created threads
	CloseHandle(myHandle);
	CloseHandle(myhandleB);
	CloseHandle(myhandleC);

	/*
		You can pause and resume threads using these two methods
		SuspendThread(handle)
		ResumeThread(handle)
	*/

	/*
		You can end threads with the following methods
		ExitThread()
		TerminateThread()
		_endthread()
		_endthreadex()
		cloaseHandle()
	*/
}