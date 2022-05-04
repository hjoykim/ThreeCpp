#include "Face3.h"
namespace three {
	size_t Face3::_faceId = 0;
	Face3::Face3() : a(0), b(0), c(0), materialIndex(0)
	{
		color = Color().setColorName(ColorKeywords::black);
		_id = Face3::_faceId++;
	}
	Face3::Face3(unsigned a, unsigned b, unsigned c)	: a(a), b(b), c(c), materialIndex(0)
	{
		color = Color().setColorName(ColorKeywords::black);
		_id = Face3::_faceId++;
	}
	Face3::Face3(unsigned a, unsigned b, unsigned c, Vector3 normal, Color color, unsigned materialIndex)	:a(a), b(b), c(c), normal(normal), color(color), materialIndex(materialIndex)
	{
		_id = Face3::_faceId++;
	}

	

	Face3::Face3(unsigned a, unsigned b, unsigned c, std::vector<Vector3>* normals, Color* color, unsigned materialIndex)
		: a(a), b(b), c(c), materialIndex(materialIndex)
	{
		_id = Face3::_faceId++;
		if (normals != nullptr)
			vertexNormals = *normals;

		if (color != nullptr)
			this->color = *color;
	}

	Face3::Face3(unsigned a, unsigned b, unsigned c, std::vector<Vector3>* normals, std::vector<Color>* colors, unsigned materialIndex)
		:a(a), b(b), c(c), materialIndex(materialIndex)
	{

		_id = Face3::_faceId++;
		if (normals != nullptr)
			vertexNormals = *normals;

		if (colors != nullptr)
			vertexColors = *colors;

		if (vertexColors.size() == 0) {
			Color color1 = Color().setColorName(ColorKeywords::black);
			color = color1;
		}
	}

	Face3::Face3(const Face3& source)
	{		
		_id = source._id;
		a = source.a;
		b = source.b;
		c = source.c;

		normal = source.normal;
		color = source.color;

		vertexNormals = source.vertexNormals;
		vertexColors = source.vertexColors;

		materialIndex = source.materialIndex;
	}

	Face3& three::Face3::clone(Face3* target)
	{
		target->copy(*this);			

		return *target;
	}

	Face3& three::Face3::copy(Face3& source)
	{
		a = source.a;
		b = source.b;
		c = source.c;

		normal = source.normal;
		color = source.color;

		vertexNormals = source.vertexNormals;
		vertexColors = source.vertexColors;

		materialIndex = source.materialIndex;

		return *this;
	}

}
