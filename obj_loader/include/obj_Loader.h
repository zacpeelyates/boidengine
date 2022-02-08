//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	obj_Loader.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  01/01/21
// Brief: Function definitions + member model map for OBJLoader.cpp 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include "OBJDatatypes.h"
class OBJLoader {
	public:

		static OBJModel* OBJProcess(const std::string& a_strFilePath, const bool a_bPrintComments);
	protected:
		static bool OBJLoadMaterials(const std::string& a_strFilePath, OBJModel& a_roLoadedData, const bool a_bPrintComments);
		static OBJFace ProcessFace(std::string a_faceData);
		
		
	private:
		static std::map<std::string, OBJModel*> m_modelMap;
	
};
#endif // OBJ_LOADER_H
