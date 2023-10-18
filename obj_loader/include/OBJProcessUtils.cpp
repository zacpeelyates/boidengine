// File: OBJProcessUtils.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/02/16
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: OBJ processing helper functions

#include "OBJProcessUtils.h"
#include <sstream>

std::vector<std::string> OBJProcessUtils::SplitStringAtChar(std::string a_strData, char a_cDelimiter)
{
	//Splits a string at give char and returns a vector of all the parts 
	std::vector<std::string> outVec;
	std::stringstream ss(a_strData);
	std::string segment;
	while (std::getline(ss, segment, a_cDelimiter))
	{
		outVec.push_back(segment);
	}
	return outVec;
}

bool OBJProcessUtils::ParseStringToInt(std::string& a_rStrIn)
{
	//Checks if given string can be parsed to an integer
	if (a_rStrIn.empty()) return false;
	return std::find_if(a_rStrIn.begin(), a_rStrIn.end(), [](unsigned char c) {return !std::isdigit(c); }) == a_rStrIn.end();
}

//file information utility definitions
std::string OBJProcessUtils::GetFileName(const std::string& a_strFilePath)
{
	//returns the file name with no preceeding path or extention
	const int start = a_strFilePath.rfind('/')+1;
	const int size = a_strFilePath.rfind('.') - start;
	return a_strFilePath.substr(start, size);
	//fixed this it was silently making everything not work (was putting start, end instead of start,size)
}

std::string OBJProcessUtils::GetFileType(const std::string& a_strFilePath)
{
	//returns the file extention with no preceeding name or path
	return a_strFilePath.substr(a_strFilePath.rfind('.') + 1);
}

std::string OBJProcessUtils::GetFileDirectory(const std::string& a_strFilePath)
{
	//returns directory the file is contained in
	return a_strFilePath.substr(0, a_strFilePath.rfind('/') + 1);
}
