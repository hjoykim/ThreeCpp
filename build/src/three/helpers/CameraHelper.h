#ifndef CAMERA_HELPER_H
#define CAMERA_HELPER_H
#include <three/cameras/Camera.h>
#include <three/objects/Line.h>
#include <three/materials/LineBasicMaterial.h>
#include <three/core/BufferAttribute.h>
#include <three/core/BufferGeometry.h>
#include <unordered_map>
#include <string>
namespace three {
	using namespace std;
	class CameraHelper : public LineSegments {		
	private:
		vector<float> vertices;
		vector<float> colors;
		unordered_map<string, vector<unsigned>> pointMap;
		Vector3 _vector;
		
		Camera::ptr camera;

		void addLine(string a, string b, Color color) {
			addPoint(a, color);
			addPoint(b, color);
		}

		void addPoint(string id, Color color) {
			vertices.push_back(0); vertices.push_back(0); vertices.push_back(0);
			colors.push_back(color.r); colors.push_back(color.g); colors.push_back(color.b);

			if (pointMap.count(id)==0)
			{
				pointMap.insert({ id, vector<unsigned>() });
			}
			vector<unsigned>& idMap = pointMap[id];
			idMap.push_back(vertices.size() / 3 - 1);
		}

		
		void setPoint(string point, const Camera::ptr& camera, float x, float y, float z) {

			_vector.set(x, y, z).unproject(*camera);

			auto& points = pointMap[point];

			BufferGeometry::ptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
			if (points.size() != 0) {

				BufferAttribute<float>::ptr position = bufferGeometry->getAttribute(AttributeName::position);

				for (int i = 0; i < points.size(); i++) {

					position->setXYZ(points[i], _vector.x, _vector.y, _vector.z);

				}
			}
		}
	public:

		using ptr = std::shared_ptr<CameraHelper>;

		CameraHelper(const Camera::ptr& camera) : LineSegments(),camera(camera) {
			this->geometry = BufferGeometry::create();
			this->material = LineBasicMaterial::create(Color(0xffffff));
			material->vertexColors = true;
			material->toneMapped = true;

			Color colorFrustum = Color(0xffaa00);
			Color colorCone = Color(0xff0000);
			Color colorUp = Color(0x00aaff);
			Color colorTarget = Color(0xffffff);
			Color colorCross = Color(0x333333);

			// near

			addLine("n1", "n2", colorFrustum);
			addLine("n2", "n4", colorFrustum);
			addLine("n4", "n3", colorFrustum);
			addLine("n3", "n1", colorFrustum);

			// far

			addLine("f1", "f2", colorFrustum);
			addLine("f2", "f4", colorFrustum);
			addLine("f4", "f3", colorFrustum);
			addLine("f3", "f1", colorFrustum);

			// sides

			addLine("n1", "f1", colorFrustum);
			addLine("n2", "f2", colorFrustum);
			addLine("n3", "f3", colorFrustum);
			addLine("n4", "f4", colorFrustum);

			// cone

			addLine("p", "n1", colorCone);
			addLine("p", "n2", colorCone);
			addLine("p", "n3", colorCone);
			addLine("p", "n4", colorCone);

			// up

			addLine("u1", "u2", colorUp);
			addLine("u2", "u3", colorUp);
			addLine("u3", "u1", colorUp);

			// target

			addLine("c", "t", colorTarget);
			addLine("p", "c", colorCross);

			// cross

			addLine("cn1", "cn2", colorCross);
			addLine("cn3", "cn4", colorCross);

			addLine("cf1", "cf2", colorCross);
			addLine("cf3", "cf4", colorCross);

			BufferGeometry::ptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
			bufferGeometry->setAttribute(AttributeName::position, BufferAttribute<float>::create(vertices,3));
			bufferGeometry->setAttribute(AttributeName::color, BufferAttribute<float>::create(colors,3));


			this->camera->updateProjectionMatrix();

			this->matrix = camera->matrixWorld;

			this->matrixAutoUpdate = false;

			this->update();

		}
		static ptr create(const Camera::ptr& camera) {
			return std::make_shared<CameraHelper>(camera);
		}
		virtual ~CameraHelper() {

		}
		void update() {

			int w = 1, h = 1;

			this->matrix = camera->matrixWorld;

			Camera::ptr _camera = Camera::create();
			// we need just camera projection matrix inverse
			// world matrix must be identity

			_camera->projectionMatrixInverse.copy(this->camera->projectionMatrixInverse);

			// center / target

			setPoint("c", _camera, 0, 0, -1);
			setPoint("t", _camera, 0, 0, 1);

			// near

			setPoint("n1", _camera, -w, -h, -1);
			setPoint("n2", _camera, w, -h, -1);
			setPoint("n3", _camera, -w, h, -1);
			setPoint("n4", _camera, w, h, -1);

			// far

			setPoint("f1", _camera, -w, -h, 1);
			setPoint("f2", _camera, w, -h, 1);
			setPoint("f3", _camera, -w, h, 1);
			setPoint("f4", _camera, w, h, 1);

			// up

			setPoint("u1", _camera, w * 0.7f, h * 1.1f, -1);
			setPoint("u2", _camera, -w * 0.7f, h * 1.1f, -1);
			setPoint("u3", _camera, 0, h * 2, -1);

			// cross

			setPoint("cf1", _camera, -w, 0, 1);
			setPoint("cf2", _camera, w, 0, 1);
			setPoint("cf3", _camera, 0, -h, 1);
			setPoint("cf4", _camera, 0, h, 1);

			setPoint("cn1", _camera, -w, 0, -1);
			setPoint("cn2", _camera, w, 0, -1);
			setPoint("cn3", _camera, 0, -h, -1);
			setPoint("cn4", _camera, 0, h, -1);

			BufferGeometry::ptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);

			BufferAttribute<float>::ptr attribute = bufferGeometry->getAttribute(AttributeName::position);
			attribute->setNeedsUpdate(true);

			_camera.reset();
		}
	};
}
#endif