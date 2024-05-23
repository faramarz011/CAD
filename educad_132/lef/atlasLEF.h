#ifndef ATLAS_LEF
#define ATLAS_LEF

#include "atlasDB.h"

class atlasLEF {
public:
	atlasLEF(){};
	~atlasLEF(){};
	int parse(char *, char *, atlasDB *);
	char *getInfo();
};
#endif
