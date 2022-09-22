#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

const int particleCount = 2000000000;
int * const position = new int[particleCount];


void Execute(int index)
{
	position[index] = (double)index * 1000 / 100 / 10;
}


void DoWork(int start, int end)
{
	for (int i = start; i < end; ++i)
	{
		if(i < particleCount && i >= 0)
			Execute(i);
	}
}

int main()
{
	chrono::system_clock::time_point startTime, endTime;
	chrono::microseconds deltaTime;
	double resTime;
	startTime = chrono::system_clock::now();
	for (int i = 0; i < particleCount; ++i)
	{
		position[i] = (double)i * 1000 / 100 / 10;
	}
	endTime = chrono::system_clock::now();
	deltaTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
	resTime = deltaTime.count() / 1000.0;
	cout << "常规方法用时：" << resTime << " ms" << endl;


	int thdCount = thread::hardware_concurrency();
	int cellCount = (particleCount / thdCount) + 1;
	int start = 0;
	int end = cellCount;
	vector<thread*> allThreads;

	startTime = chrono::system_clock::now();
	for (int i = 0; i < thdCount; ++i)
	{
		thread* th = new thread(DoWork, start, end);
		start = end;
		end += cellCount;
		allThreads.push_back(th);
	}
	for(thread* it : allThreads)
	{
		it->join();
	}
	endTime = chrono::system_clock::now();
	deltaTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
	resTime = deltaTime.count() / 1000.0;
	cout << "多线程用时：" << resTime << " ms" << endl;


	for (thread* it : allThreads)
	{
		delete it;
	}

	//for (int i = 0; i < particleCount; ++i)
	//{
	//	cout << position[i] << endl;
	//}
	return 0;
}