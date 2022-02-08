
#ifndef OBJPROCESSUTILS_H
#define OBJPROCESSUTILS_H
#include <string>
#include <vector>
class OBJProcessUtils {
public:
	static std::vector<std::string> SplitStringAtChar(std::string a_strData, char a_cDelimiter);
	static bool ParseStringToInt(std::string& a_rStrIn);
	static std::string GetFileDirectory(const std::string a_strFilePath);
	static std::string GetFileType(const std::string a_strFilePath);
	static std::string GetFileName(const std::string a_strFilePath);
};
#endif // OBJPROCESSUTILS_H
