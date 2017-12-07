#include "vector3.h"
#include <jsoncpp\dist\json\json.h>
#include <jsoncpp\dist\jsoncpp.cpp>
#include <jsoncpp\dist\json\json-forwards.h>
#include <sphere.h>

class mtl {
public:
	mtl();
	Scene* mtl_loader(char* filename);
	void mtl_writer(char* filename);
private:
	Sphere s;
	Material m;
	Light l;
};