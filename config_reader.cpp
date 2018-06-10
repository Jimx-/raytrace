#include <fstream>

#include "sphere.h"
#include "scene.h"
#include "config_reader.h"
#include "OBJ_Loader.h"
#include "libbmpread/bmpread.h"
#include "triangle.h"

#include <unordered_map>
#include <functional>
#include <vector>

using namespace std;

static void parse_sphere(Scene* scene, const Json::Value& root, PMaterial material);
static void parse_triangle(Scene* scene, const Json::Value& root, PMaterial material);
static void parse_mesh(Scene* scene, const Json::Value& root, PMaterial material);

unordered_map<string, function<void(Scene* scene, const Json::Value&, PMaterial)>> object_map{
    { "sphere", parse_sphere },
    { "triangle", parse_triangle },
	{ "mesh", parse_mesh },
};

static void parse_sphere(Scene* scene, const Json::Value& root, PMaterial material)
{
    auto mat1 = std::make_shared<Material>(0.2, 0.4, Color{1.0, 0.0, 0.0});
    Sphere* s = new Sphere(Point3{ root["center"][0].asDouble(),root["center"][1].asDouble(),root["center"][2].asDouble() },
                           root["radius"].asDouble(), material);
	scene->add_object(s);
}

static void parse_triangle(Scene* scene, const Json::Value& root, PMaterial material)
{
}

static void parse_mesh(Scene* scene, const Json::Value& root, PMaterial material)
{
	objl::Loader Loader;


	// Load .obj File
	bool loadout = Loader.LoadFile(root["filename"].asString());

	if (loadout) {
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			PMaterial material;
			if (curMesh.MeshMaterial.map_Kd == "") {
				material = std::make_shared<Material>(0.2, 0.4, Color{1.0, 0.0, 0.0});
			} else if (curMesh.MeshMaterial.map_Kd == "F16t.bmp") {
				material = std::make_shared<Material>(0.2, 0.2, Color{0.0, 0.0, 0.0}, 0.4, 1.5);
			} else {
				bmpread_t bitmap;

				if(!bmpread(curMesh.MeshMaterial.map_Kd.c_str(), 0, &bitmap))
				{
					exit(1);
				}

				auto mat_tex = std::make_shared<TextureMaterial>(bitmap.width, bitmap.height, 0.2, 0.2);
				mat_tex->set_filtering(TextureFilter::BILINEAR);
				mat_tex->copy_data(bitmap.rgb_data);
				material = mat_tex;
				bmpread_free(&bitmap);
			}

			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				std::vector<Vector3> vertices;
				std::vector<TexCoord> texcoords;
				std::vector<Vector3> normals;
				for (int k = j; k < j + 3; k++) {
					auto& v = curMesh.Vertices[curMesh.Indices[k]];
					vertices.emplace_back(Vector3{v.Position.X * 500, v.Position.Y * 500, v.Position.Z * 500});
					texcoords.emplace_back(TexCoord{v.TextureCoordinate.X, v.TextureCoordinate.Y});
					normals.emplace_back(Vector3{v.Normal.X, v.Normal.Y, v.Normal.Z});
				}

				scene->add_object(new Triangle(vertices[0], vertices[1], vertices[2], texcoords[0], texcoords[1], texcoords[2], normals[0], normals[1], normals[2], material));
			}
		}
	}
}

PMaterial parse_material(const Json::Value& root)
{
    double metallic = root["metallic"].asDouble();
    double roughness = root["roughness"].asDouble();

    Color color{ root["color"][0].asDouble(), root["color"][1].asDouble(), root["color"][2].asDouble() };

    double reflectivity = root.isMember("reflectivity") ? root["reflectivity"].asDouble() : Material::NO_REFLECTION;
	double refractive_index = root.isMember("refractive_index") ? root["refractive_index"].asDouble() : Material::NO_REFRATION;

	return make_shared<Material>(metallic, roughness, color, reflectivity, refractive_index);
}

Camera* parse_camera(const Json::Value& root)
{
#define DEF_FIELD(name) Point3 name{ root[#name][0].asDouble(), root[#name][1].asDouble(), root[#name][2].asDouble() }
    DEF_FIELD(center);
    DEF_FIELD(up);
    DEF_FIELD(look_at);

    return new Camera(center, up, look_at);
}

Scene* load_scene(const string& filename) {
	ifstream f;
	f.open(filename);
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(f, root, false)) {
		cout << "failed to load" << endl;
		Json::Value failed;
	}

	Scene*scene = new Scene(root["supersamples"].asInt());
	for (auto&& light_json : root["lights"]) {
		Light* l = new Light(
            Point3{ light_json["position"][0].asDouble(),light_json["position"][1].asDouble(),light_json["position"][2].asDouble() },
			Color{ light_json["color"][0].asDouble(), light_json["color"][1].asDouble(),light_json["color"][2].asDouble() },
			light_json["intensity"].asDouble());
		scene->add_light(l);
	}

	for (auto&& obj_json : root["objects"]) {
        PMaterial material = parse_material(obj_json["material"]);
        object_map[obj_json["type"].asString()](scene, obj_json, material);
	}

    scene->set_camera(parse_camera(root["camera"]));
    scene->set_background(Color{1.0f, 0.8f, 0.0f});
	return scene;
}

