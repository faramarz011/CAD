#ifndef ATLAS_DEF
#define ATLAS_DEF

#include "atlasDB.h"

class atlasDEF {
public:
	atlasDEF(){};
	~atlasDEF(){};
	int parse(char *, char *, atlasDB *);
	char *getInfo();
};
#endif
