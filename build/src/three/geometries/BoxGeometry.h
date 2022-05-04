#ifndef BOX_GEOMETRY_THREE_H
#define BOX_GEOMETRY_THREE_H
#include <three/core/BufferGeometry.h>
#include <three/core/BufferAttribute.h>
namespace three {
	class BoxGeometryParameter {
	protected:
		float width;
		float height;
		float depth;
		float widthSegments;
		float heightSegments;
		float depthSegments;
	};
	class BoxBufferGeometry : public BufferGeometry,public BoxGeometryParameter {
	private:
		std::vector<unsigned> _indices;
		std::vector<float> _vertices;
		std::vector<float> _uvs;
		std::vector<float> _normals;

		unsigned numberOfVertices = 0;
		unsigned groupStart = 0;

		void buildPlane(unsigned char u, unsigned char v, unsigned char w, int udir, int vdir, float width, float height, float depth, int gridX, int gridY, int materialIndex);

		
	public:
		using ptr = std::shared_ptr<BoxBufferGeometry>;
				
		BoxBufferGeometry(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0);
			

		static ptr create(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0) 
		{
			return std::make_shared<BoxBufferGeometry>(width,height,depth,widthSegments,heightSegments,depthSegments);
		}
	};

	class BoxGeometry : public Geometry,public BoxGeometryParameter {
	public:
		using ptr = std::shared_ptr<BoxGeometry>;
		BoxGeometry(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0);
		
		

		static ptr create(float width = 0, float height = 0, float depth = 0, int widthSegments = 0, int heightSegments = 0, int depthSegments = 0) {
			return std::make_shared<BoxGeometry>(width, height, depth, widthSegments, heightSegments, depthSegments);
		}
	};
}
#endif