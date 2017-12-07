#include <mtl.h>
#include <fstream>
#include <scene.h>
#include <scene.h>
using namespace std;


Scene* mtl::mtl_loader(char* filename) {
	auto mat1 = std::make_shared<Material>(0.2, 0.2, Color{ 0.0, 0.0, 0.0 }, 0.4, 1.5);
	ifstream f;
	f.open(filename);
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(f, root, false)) {
		cout << "failed to load" << endl;
		Json::Value failed;
	}

	Scene*scene = new Scene(1);
	for (int i = 0; i < root["light"].size; i++) {
		int j = 0;
		Light* l = new Light(Point3{ root["light"][i]["position"][j].asDouble(),root["light"][i]["position"][j + 1].asDouble(),root["light"][i]["position"][j + 2].asDouble() },
			Color{ root["light"][i]["color"][j].asDouble(), root["light"][i]["color"][j + 1].asDouble(),root["light"][i]["color"][j + 2].asDouble() },
			root["light"][i]["intensity"].asInt());
		scene->add_light(l);
	}
	for (int i = 0; i < root["object"].size; i++) {
		if (root["object"][i]["type"].asString() == "sphere") {
			int j = 0;
			//Material m(root["object"][i]["material"]["metallic"].asDouble, root["object"][i]["material"]["roughness"].asDouble, Color{ root["object"][i]["material"]["color"][j].asDouble(),root["object"][i]["material"]["color"][j + 1].asDouble(),root["object"][i]["material"]["color"][j + 2].asDouble() }, root["object"][i]["material"]["reflectivity"].asDouble(), root["object"][i]["material"]["refractive_index"].asDouble() );
			Sphere* s = new Sphere(Point3{ root["object"][i]["center"][j].asDouble(),root["object"][i]["center"][j + 1].asDouble(),root["object"][i]["center"][j + 2].asDouble() },
				root["object"][i]["radious"].asDouble(), mat1);
			scene->add_object(s);

		}
		else {}
	}
    scene->set_background(Color{1.0f, 0.8f, 0.0f});
	return scene;
}

