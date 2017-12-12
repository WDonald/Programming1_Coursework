/*
=================
cFileMgr.h
- Header file for class definition - SPECIFICATION
- Header file for the fileMgr class
=================
*/
#ifndef _CFILEMGR_H
#define _CFILEMGR_H
#include "spookyGame.h"
#include <sstream>


class cFileMgr 
{
public:
	cFileMgr();
	cFileMgr(cFileHandler* aFile);

	int loadScoreFromFile(cFileHandler* aFile);		
	void writeScoreDataToFile(cFileHandler* aFile,string score);
};
#endif