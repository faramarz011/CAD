#ifndef ATLAS_STRING_H
#define ATLAS_STRING_H

#include <string>
using namespace std;

class atlasString : public string{
public:
	atlasString();
	atlasString(string s);
	void WriteBinFile(FILE *);
	void ReadBinFile(FILE *);
	void operator=(string);
	//void operator=(char *str);
	void operator=(char str[]);
	void setStr(char *str);//for TCL
	void setStr(string s);//for TCL
	const char *getStr();//for TCL
};
#endif 
