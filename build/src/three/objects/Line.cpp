#include "Line.h"
namespace three {
	Line::Line(const Geometry::ptr& geometry, const Material::ptr& material) : Object3D()
	{
		this->geometry = geometry != nullptr ? geometry : Geometry::create();
		this->material = material != nullptr ? material : LineBasicMaterial::create();
		this->type = "Line";
		this->isLine = true;
	}
	Line::Line(const Geometry::ptr& geometry, const std::vector<Material::ptr>& materials) : Object3D()
	{
		this->geometry = geometry != nullptr ? geometry : Geometry::create();
		if (materials.size() > 0) {
			this->materials = materials;
			this->material = materials[0];
		}
		this->type = "Line";
		this->isLine = true;
	}
	Line& Line::computeLineDistances()
	{

		Vector3 _start;
		Vector3 _end;
		if (!geometry->isGeometry)
		{

			// we assume non-indexed geometry
			auto bgeometry = *std::dynamic_pointer_cast<BufferGeometry::ptr>(geometry);

			if (bgeometry->index == nullptr)
			{

				BufferAttribute<float>::ptr positionAttribute = bgeometry->getAttribute(AttributeName::position);//(BufferAttribute<float>)((geometry as BufferGeometry).Attributes["position"]);
				std::vector<float> lineDistances;// = new List<float>();
				lineDistances.push_back(0);

				for (unsigned i = 1; i < positionAttribute->count; i++)
				{

					_start.fromBufferAttribute(*positionAttribute, i - 1);
					_end.fromBufferAttribute(*positionAttribute, i);

					//lineDistances[i] = lineDistances[i - 1];

					lineDistances.push_back(lineDistances[i - 1]);

					lineDistances[i] += _start.distanceTo(_end);

				}
				auto lineDistance = BufferAttribute<float>::create(&lineDistances[0], lineDistances.size(), 1);
				bgeometry->setAttribute(AttributeName::lineDistance,lineDistance);
				//bgeometry->setLineDistances(lineDistance);
				//(geometry as BufferGeometry).SetAttribute("lineDistance", new BufferAttribute<float>(lineDistances.ToArray(), 1));
			}
			/*else
			{

				Console.WriteLine("THREE.Line.computeLineDistances(): Computation only possible with non-indexed BufferGeometry.");

			}*/

		}
		else if (geometry->isGeometry)
		{

			auto _vertices = &geometry->vertices;
			auto _lineDistances = &geometry->lineDistances;

			_lineDistances->clear();

			_lineDistances->push_back(0);

			for (unsigned i = 1; i < _vertices->size(); i++)
			{

				_lineDistances->push_back(_lineDistances->at(i - 1));

				(*_lineDistances)[i] += _vertices->at(i - 1).distanceTo(_vertices->at(i));

			}
		}
		return *this;
	}
	void Line::raycast()
	{
	}
	LineSegments& LineSegments::computeLineDistances() {

		Vector3 _start;
		Vector3 _end;
		if (!geometry->isGeometry)
		{
			auto bgeometry = *std::dynamic_pointer_cast<BufferGeometry::ptr>(geometry);
			if (bgeometry->index == nullptr)
			{
				BufferAttribute<float>::ptr positionAttribute = bgeometry->getAttribute(AttributeName::position);//(BufferAttribute<float>)(geometry as BufferGeometry).Attributes["position"];
				std::vector<float> lineDistances;// = new List<float>();

				for (unsigned int i = 0; i < positionAttribute->count; i += 2)
				{
					_start = _start.fromBufferAttribute(*positionAttribute, i);
					_end = _end.fromBufferAttribute(*positionAttribute, i + 1);

					lineDistances.push_back(i == 0 ? 0 : lineDistances[i - 1]);
					lineDistances.push_back(lineDistances[i] + _start.distanceTo(_end));
				}
				auto lineDistance = BufferAttribute<float>::create(&lineDistances[0], lineDistances.size(), 1);
				//bgeometry->setLineDistances(lineDistance);
				bgeometry->setAttribute(AttributeName::lineDistance,lineDistance);
				//(geometry as BufferGeometry).SetAttribute("lineDistance", new BufferAttribute<float>(lineDistances.ToArray(), 1));
			}
			/* else
			 {
				 Trace.TraceWarning("THREE.Objects.LineSegments.ComputeLineDistance(): Computation only possible with non-indexed BufferGeometry");
			 }*/
		}
		else if (geometry->isGeometry)
		{
			auto _vertices = &geometry->vertices;
			auto _lineDistances = &geometry->lineDistances;



			for (int i = 0; i < _vertices->size(); i += 2)
			{
				_start = _vertices->at(i);
				_end = _vertices->at(i + 1);

				_lineDistances->push_back(i == 0 ? 0 : _lineDistances->at(i - 1));
				_lineDistances->push_back(_lineDistances->at(i) + _start.distanceTo(_end));
			}
			//geometry.LineDistances = lineDistances;
		}
		return *this;
	}
}
