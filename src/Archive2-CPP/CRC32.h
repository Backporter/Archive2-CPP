#include <string>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <iostream>
#include <list>
#include <vector>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <Windows.h>
#include <libloaderapi.h>
#include <minwindef.h>
#include <minwinbase.h>
using namespace std;
namespace fs = std::filesystem;

#ifndef caculatecrc32_H_
#define caculatecrc32_H_
UINT caculatecrc32(const char * s, int lenth);
#endif

#ifndef Compute_H_
#define Compute_H_
UINT Compute(::string stringtohash);
#endif