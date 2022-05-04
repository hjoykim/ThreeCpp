#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H
#include <three/loaders/MTLLoader.h>
#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <three/objects/Group.h>
using namespace std;
namespace three {
	class Object {
	public:
		class ObjectMaterial {
		public:
			using ptr = shared_ptr<ObjectMaterial>;

			int index=-1;

			string name="";

			int groupStart = 0;

			int groupEnd = -1;

			int groupCount = -1;

			bool inherited = false;

			bool smooth=false;

			vector<ObjectMaterial::ptr> objMtlLib;

			string stringLib;

			ObjectMaterial() {}

			~ObjectMaterial() = default;

			//clone
			ObjectMaterial::ptr clone(int _index);
		};

		class ObjectGeometry {
		public:
			vector<float> vertices;

			vector<float> normals;

			vector<float> colors;

			vector<float> uvs;

			string type;

			ObjectGeometry() {}

			~ObjectGeometry() = default;
		};
		using ptr = shared_ptr<Object>;

		string name = "";

		bool fromDeclaration = true;

		ObjectGeometry geometry;

		vector<ObjectMaterial::ptr> materials;

		bool smooth = true;

		Object() {}

		~Object() = default;

		ObjectMaterial::ptr& currentMaterial();

		ObjectMaterial::ptr startMaterial(const string& name,vector<string>& libraries);

		ObjectMaterial::ptr& finalize(bool end);

	};

	class ObjectState {
		
		int parseVertexIndex(int value, size_t len);
		
		int parseNormalIndex(int value, size_t len);
		
		int parseUVIndex(int value, size_t len);
		
		void addVertex(int a, int b, int c);
		
		void addColor(int a, int b, int c);
		
		void addUV(int a, int b, int c);
		
		void addNormal(int a, int b, int c);

	public:
		Object::ptr object;

		vector<Object::ptr> objects;

		vector<float> vertices;
		
		vector<float> normals;
		
		vector<float> colors;
		
		vector<float> uvs;
		
		vector<string> materialLibraries;

		ObjectState();

		~ObjectState() = default;

		void startObject(const string& name, bool fromDeclaration);

		void finalize();

		void addVertexPoint(int a);

		void addVertexLine(int a);

		void addUVLine(int a);

		void addPointGeometry(const vector<string>& _vertices);

		void addPointGeometry(const vector<float>& _vertices);

		void addLineGeometry(const vector<string>& _vertices, const vector<string>& _uvs = vector<string>());

		void addLineGeometry(const vector<float>& _vertices, vector<float> _uvs = vector<float>());

		void addFace(int a, int b, int c, int ua, int ub, int uc,int na,int nb,int nc);

		void addFace(int a, int b, int c, int d, int ua, int ub, int uc, int ud, int na, int nb, int nc, int nd);



	};

	class OBJLoader {
	private:	

		float parseFloat(string value);
		int parseInt(string value);
	public:

		MTLLoader::MaterialCreator materials;

		OBJLoader() {}

		~OBJLoader() = default;

		void parseState(ObjectState& state);

		void setMaterials(const MTLLoader::MaterialCreator& materials);

		Group::ptr parse(const string& path);

		Group::ptr load(const string& path);
	};
}
#endif
