#include "Image.h"

long long int Image::redValues[256]{};
long long int Image::greenValues[256]{};
long long int Image::blueValues[256]{};
long long int Image::whiteValues[256]{};
long long int Image::blackValues[256]{};
long long int Image::amountBlack = 0;
long long int Image::amountWhite = 0;

Color::Color()
	:r(0), g(0), b(0)
{
}

Color::Color(int r, int g, int b)
	: r(r), g(g), b(b)
{
	if (r < 0 || r > 255)
		r = 0;
	if (g < 0 || g > 255)
		g = 0;
	if (b < 0 || b > 255)
		b = 0;
}

Color::~Color()
{
}


Image::Image(std::string path, int thr, int thresh, int flag)
	: filepath(path), threshold(thresh), cppFlag(flag)
{
	if (flag)
	{
		dllCppHandle = ::LoadLibrary(__TEXT("CppDLL.dll"));
		if (dllCppHandle != NULL)
		{
			cppFunc = reinterpret_cast<ptr>(::GetProcAddress(dllCppHandle, "cpp_binarization"));
		}
		else
		{
			return;
		}
	}
	else
	{
		dllAsmHandle = LoadLibrary(TEXT("asmDLL.dll"));
		if (dllAsmHandle != NULL)
		{
			proc = (MyBinarization)GetProcAddress(dllAsmHandle, "MyBinarization");
		}
		else
		{
			return;
		}
	}
		setThreads(thr);
		Read(path.c_str());
		Export("binarized.bmp");
}

Image::~Image()
{
	if(arr != nullptr)
		delete[] arr;
}

void Image::setThreads(int thr)
{
	threads = thr;
}

int Image::getThreads()
{
	return threads;
}

Color Image::getColor(int x, int y) const
{
	return m_colors[y * m_width + x];
}

void Image::setColor(const Color& color, int x, int y)
{
	m_colors[y * m_width + x].r = color.r;
	m_colors[y * m_width + x].g = color.g;
	m_colors[y * m_width + x].b = color.b;
}

void Image::Read(const char* path)
{
	std::fstream f;
	f.open(path, std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		return;
	}

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;

	unsigned char fileHeader[fileHeaderSize];
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	unsigned char informationHeader[informationHeaderSize];
	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
	m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
	m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	m_colors.resize(m_width * m_height);

	const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			unsigned char color[3];
			f.read(reinterpret_cast<char*>(color), 3);

			m_colors[y * m_width + x].r = static_cast<int>(color[2]);
			m_colors[y * m_width + x].g = static_cast<int>(color[1]);
			m_colors[y * m_width + x].b = static_cast<int>(color[0]);
		}
		f.ignore(paddingAmount);
	}

	f.close();

	m_length = m_width * m_height;

	countColors();

	fromVecToArray(m_colors);

	m_pixels_per_thread = ceil((double)m_length / (double)threads);

	clock_t start = clock();


	for (int i = 0; i < threads; i++)
	{
		if (i == (threads - 1))
		{
			if (cppFlag == true)
			{
				m_start = m_end;
				m_end = m_length;
				m_threads.push_back(std::thread(cppFunc, arr, threshold, m_start, m_end));
			}
			else
			{
				m_start = m_end;
				m_end = m_length;

				m_threads.push_back(std::thread(&Image::assemblyBinarization, this, arr, threshold * 3, m_end, m_start));
			}
		}
		else 
		{
			if (cppFlag == true)
			{
				m_start = m_end;
				m_end = m_start + m_pixels_per_thread;
				m_threads.push_back(std::thread(cppFunc, arr, threshold, m_start, m_end));
			}
			else
			{
				m_start = m_end;
				m_end = m_start + m_pixels_per_thread;

				m_threads.push_back(std::thread(&Image::assemblyBinarization, this, arr, threshold * 3, m_start, m_end));
			}
		}
		
	}

	for (std::thread& thread : m_threads)
	{
		thread.join();
	}


	clock_t end = clock();

	elapsedTime = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;

	fromArrayToVec();
}

void Image::Export(const char* path) const
{
	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary);

	if (!f.is_open())
	{
		return;
	}

	unsigned char bmpPad[3] = { 0, 0, 0 };
	const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

	unsigned char fileHeader[fileHeaderSize];

	fileHeader[0] = 'B';
	fileHeader[1] = 'M';

	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;

	fileHeader[6] = 0;
	fileHeader[7] = 0;

	fileHeader[8] = 0;
	fileHeader[9] = 0;

	fileHeader[10] = informationHeaderSize + fileHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;

	informationHeader[4] = m_width;
	informationHeader[5] = m_width >> 8;
	informationHeader[6] = m_width >> 16;
	informationHeader[7] = m_width >> 24;

	informationHeader[8] = m_height;
	informationHeader[9] = m_height >> 8;
	informationHeader[10] = m_height >> 16;
	informationHeader[11] = m_height >> 24;

	informationHeader[12] = 1;
	informationHeader[13] = 0;

	informationHeader[14] = 24;
	informationHeader[15] = 0;

	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;

	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;

	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;

	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;

	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;

	informationHeader[36] = 0;

	informationHeader[37] = 0;

	informationHeader[38] = 0;
	informationHeader[39] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
			unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
			unsigned char b = static_cast<unsigned char>(getColor(x, y).b);

			unsigned char color[] = { b, g, r };

			f.write(reinterpret_cast<char*>(color), 3);
		}
		f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}
	f.close();
}

void Image::fromVecToArray(const std::vector<Color>& colors)
{
	arr = new int[colors.size() * 3];

	for (int i = 0; i < colors.size(); i++)
	{
		arr[i * 3] = colors[i].r;
		arr[(i * 3) + 1] = colors[i].g;
		arr[(i * 3) + 2] = colors[i].b;
	}

	arrLen = colors.size();
}

void Image::fromArrayToVec()
{
	m_colors.clear();
	amountBlack = 0;
	amountWhite = 0;

	for (int i = 0; i < arrLen; i++)
	{
		Color tmp(arr[3 * i], arr[(3 * i) + 1], arr[(3 * i) + 2]);

		if (arr[3 * i] == 0)
		{
			amountBlack++;
		}
		else
		{
			amountWhite++;
		}

		m_colors.push_back(tmp);
	}
}

void Image::assemblyBinarization(int* arr, int thresh, int start, int end)
{
	proc(arr, thresh, start, end);
}

void Image::countColors()
{
	for (int i = 0; i < 256; i++)
	{
		redValues[i] = 0;
		greenValues[i] = 0;
		blueValues[i] = 0;
	}

	for (int i = 0; i < m_colors.size(); i++)
	{
		redValues[m_colors[i].r]++;
		greenValues[m_colors[i].g]++;
		blueValues[m_colors[i].b]++;	
	}
}

