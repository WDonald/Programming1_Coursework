/*
=================
- cFileMgr.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cFileMgr.h"

/*
=================================================================
 Defualt Constructor
=================================================================
*/
cFileMgr::cFileMgr()
{
}


cFileMgr::cFileMgr(cFileHandler* aFile)
{
	this->loadScoreFromFile(aFile);
}

/*
=================================================================
- load the score from the file
=================================================================
*/
 int cFileMgr::loadScoreFromFile(cFileHandler* aFile)
{
	
	string fileData;
	fileData = aFile->readDataFromFile();
	cout << "fileData " << fileData << endl;
	
	stringstream scoreData(fileData);
	
	int scoreToLoad;
	//http://www.geeksforgeeks.org/converting-strings-numbers-cc/
	
	scoreData >> scoreToLoad;
	cout << "scoreToLoad "<< scoreToLoad << endl ;
	
	aFile->closeFile();
	
	return scoreToLoad; 
}


/*
=================================================================
- Write the score to the file
=================================================================
*/
void cFileMgr::writeScoreDataToFile(cFileHandler* aFile, string score)
{
	string scoreDataToWrite = "";
	string theScoreData = score;
	stringstream outputStr(theScoreData);
	
	if (!theScoreData.empty())
	{
		while(outputStr >> scoreDataToWrite) //(getline(outputStr, scoreDataToWrite, '\n'))
		{
			//https://www.uow.edu.au/~lukes/TEXTBOOK/notes-cpp/io/readtextfile.html
			//scoreDataToWrite += '\n';
			aFile->writeDataToFile(scoreDataToWrite);
			cout << "saved " << scoreDataToWrite <<" to "<< aFile->getFileName() << endl;
		}
	}
	aFile->closeFile();
}
