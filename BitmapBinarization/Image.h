#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread> 
#include <Windows.h>
#include <string>

struct Color
{
	int r, g, b;

	Color();
	Color(int r, int g, int b);
	~Color();
};

class Image
{
public:
	Image() = default;
	Image(std::string path, int thr, int threshold, int cppFlag);
	~Image();

	void setThreads(int thr);

	int getThreads();

	Color getColor(int x, int y) const;
	void setColor(const Color& color, int x, int y);

	void Read(const char* path);
	void Export(const char* path) const;

	void fromVecToArray(const std::vector<Color>& colors);
	void fromArrayToVec();

	void assemblyBinarization(int* arr, int thresh, int start, int end);

	void countColors(/*int red[], int green[], int blue[]*/);

	double elapsedTime;

	int percentageOfWhites;
	int percentageOfBlacks;

	static long long int amountWhite;
	static long long int amountBlack;

	int arrLen = 0;

	static long long int redValues[256];
	static long long int greenValues[256];
	static long long int blueValues[256];
	static long long int whiteValues[256];
	static long long int blackValues[256];

private:
	//HINSTANCE lib = ::LoadLibrary(__TEXT("CppDLL.dll"));

	bool cppFlag;
	int threshold; 

	HINSTANCE dllCppHandle = NULL;
	typedef void (*ptr)(int*, int, int, int);
	ptr cppFunc;

	typedef void(__stdcall* MyBinarization)(int*, int, int, int);
	HINSTANCE dllAsmHandle = NULL;
	MyBinarization proc;

	std::vector<std::thread> m_threads;

	int m_width;
	int m_height; 
	int m_length;
	std::vector<Color> m_colors;

	std::string filepath;
	std::string outputFilepath;

	int threads;

	int m_start = 0;
	int m_end = 0;

	int m_pixels_per_thread;

	int* arr;

	
};
