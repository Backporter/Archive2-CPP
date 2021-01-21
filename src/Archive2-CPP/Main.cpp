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
#include "CRC32.h"
#include "zlib.h"
#include <thread>
using namespace std;
namespace fs = std::filesystem;

int threadneedstoend;
std::vector<std::string> GNFs;
uint32_t DDS = 0x00736464;
uint32_t allignment = 0xBAADF00D;
uint32_t DDS_Extension = 0x7364642E;
uint32_t chunkCount = 1;

#pragma pack(1)
typedef struct {
	uint32_t magic = 0x58445442;
	uint32_t version = 1;
	uint32_t ba2type = 0x464D4E47;
	int TotalFiles;
} header;

#pragma pack(1)
typedef struct {
	UINT NameHash;
	uint32_t type = 0x00736464;
	UINT PathHash;
	BYTE unk = 0;
	BYTE chunkcount{ 0x1 };
	BYTE chunksize[2]{ 0x30, 0x00 };
	BYTE GNF_META[0x20];
	uint64_t Offset;
	UINT size;
	uint64_t RealSize;
	UINT Allginment = 0xBAADF00D;
} File_Delecation;

// not used but here as i plan to maybe use it later?
#pragma pack(1)
typedef struct {
	uint16_t strlenth;
	char str;
} Pascal_string;

string getFileName(const string& s) {

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

int generateoffset(int cct) {
	int _O = 0x18;
	for (int i = 0; i < cct; i++) {
		_O += 0x48;
	}
	return _O;
}
std::string path = std::filesystem::current_path().string() + "\\data";

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void writepa() {
	hidecursor();
	std::cout << "\r";
	do {
		cout << "Packing\r";
		Sleep(700);
		cout << "Packing.\r";
		Sleep(700);
		cout << "Packing..\r";
		Sleep(700);
		cout << "Packing...\r";
		Sleep(700);
		cout << "                  \r";
	} while (threadneedstoend != 1);
}

int main(int argc, char **argv)
{
	std::thread th1(writepa);
	if (!std::filesystem::exists(path)) {
		exit(1);
	}
	header _header;
	string BA2name = "COutput - Textures.ba2";
	for (const auto & entry : fs::recursive_directory_iterator(path)) {
		if (!fs::is_directory(entry.path().string())) {
			GNFs.push_back(entry.path().string());
		}
	}
	if (argc > 3) {
		BA2name = argv[1];
	}
	_header.TotalFiles = GNFs.size();
	FILE *BA2;
	uLong size;
	fopen_s(&BA2, BA2name.c_str(), "wb+");
	fwrite(&_header, sizeof(header), 1, BA2);
	UINT offsetbase = generateoffset(GNFs.size());
	uint64_t Offset = offsetbase;
	uint64_t strtableoffset = 0;
	fwrite(&strtableoffset, sizeof(uint64_t), 1, BA2);
	strtableoffset = offsetbase;
	for (const auto & entry : GNFs) {
		File_Delecation __File_Delecation;
		FILE *GNFf;
		int siz = getFileName(entry).length() - 4;
		string name = getFileName(entry).substr(0, siz);
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		string dir = fs::path(entry).parent_path().string().substr(path.length() + 1, entry.length() - path.length());
		transform(dir.begin(), dir.end(), dir.begin(), ::tolower);
		fopen_s(&GNFf, entry.c_str(), "rb");
		fseek(GNFf, 0, SEEK_END);
		size = ftell(GNFf) - 256;
		fseek(GNFf, 0x10, SEEK_SET);
		fread(__File_Delecation.GNF_META, sizeof(BYTE[32]), 1, GNFf);
		__File_Delecation.NameHash = Compute(name);
		__File_Delecation.PathHash = Compute(dir);
		__File_Delecation.RealSize = size;
		__File_Delecation.Offset = Offset;
		ULONG dCompressedDataSize = (size * 1.1) + 12;
		unsigned char * gnfdata = new unsigned char[size];
		fread(gnfdata, sizeof(unsigned char), size, GNFf);
		unsigned char * compressedgnfdata = new unsigned char[dCompressedDataSize];
		fseek(GNFf, 0x100, SEEK_SET);
		fread(gnfdata, size, 1, GNFf);
		int z_result;
		z_result = compress2(compressedgnfdata, &dCompressedDataSize, gnfdata, size, 6);
		__File_Delecation.size = dCompressedDataSize;
		strtableoffset += dCompressedDataSize;
		fwrite(&__File_Delecation, sizeof(File_Delecation), 1, BA2);
		long pos = ftell(BA2);
		fseek(BA2, Offset, SEEK_SET);
		Offset += dCompressedDataSize;
		fwrite(compressedgnfdata, dCompressedDataSize, 1, BA2);
		fseek(BA2, pos, SEEK_SET);
		free(compressedgnfdata);
		free(gnfdata);
		fclose(GNFf);
	}
	fseek(BA2, strtableoffset, SEEK_SET);
	for (const auto & enn : GNFs) {
		int countforsub1 = path.length() + 1;
		int countforsub2 = enn.length() - countforsub1;
		std::string name = enn.substr(countforsub1, countforsub2);
		fwrite(&countforsub2, sizeof(uint16_t), 1, BA2);
		fwrite(name.c_str(), countforsub2, 1, BA2);
	}
	fseek(BA2, 0x10, SEEK_SET);
	fwrite(&strtableoffset, sizeof(uint64_t), 1, BA2);
	fclose(BA2);
	threadneedstoend = 1;
	th1.join();
}