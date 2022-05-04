#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H
#include <three/math/Vector3.h>
#include <three/math/Triangle.h>
#include <three/core/Object3D.h>
#include <three/core/BufferGeometry.h>
#include <three/math/Line3.h>
namespace three {
	namespace math {
		class Face;
		
		class VertexNode {
		public:
			
			Vector3 point;
			VertexNode* prev = nullptr;
			VertexNode* next = nullptr;
			Face* face;
			VertexNode() {}
			
			VertexNode(Vector3& point) :point(point) {}
			
		};
		class VertexList {
		public:
			VertexNode* head;
			VertexNode* tail;

			VertexList() : head(nullptr), tail(nullptr) {}
			
			VertexNode* first() {
				return head;
			}
			VertexNode* last() {
				return tail;
			}
			void clear() {
				head = nullptr;
				tail = nullptr;
			}

			VertexList& insertBefore(VertexNode* target, VertexNode* vertex) {
				vertex->prev = target->prev;
				vertex->next = target;

				if (vertex->prev == nullptr)
					this->head = vertex;
				else
					vertex->prev->next = vertex;

				target->prev = vertex;

				return *this;
			}

			VertexList& insertAfter(VertexNode* target, VertexNode* vertex) {
				vertex->prev = target;
				vertex->next = target->next;

				if (vertex->next == nullptr)
					this->tail = vertex;
				else
					vertex->next->prev = vertex;

				target->next = vertex;

				return *this;
			}

			VertexList& append(VertexNode* vertex) {
				if (this->head == nullptr)
				{
					this->head = vertex;
				}
				else
				{
					this->tail->next = vertex;
				}

				vertex->prev = this->tail;
				vertex->next = nullptr; // the tail has no subsequent vertex

				this->tail = vertex;

				return *this;
			}

			VertexList& appendChain(VertexNode* vertex) {
				if (this->head == nullptr)
				{
					this->head = vertex;
				}
				else
				{
					this->tail->next = vertex;
				}

				vertex->prev = this->tail;

				// ensure that the 'tail' reference points to the last vertex of the chain

				while (vertex->next != nullptr)
				{

					vertex = vertex->next;

				}

				this->tail = vertex;

				return *this;
			}

			VertexList& remove(VertexNode* vertex) {
				if (vertex->prev == nullptr)
				{
					this->head = vertex->next;
				}
				else
				{
					vertex->prev->next = vertex->next;
				}

				if (vertex->next == nullptr)
				{
					this->tail = vertex->prev;
				}
				else
				{
					vertex->next->prev = vertex->prev;
				}

				return *this;
			}
			VertexList& removeSubList(VertexNode* a, VertexNode* b) {
				if (a->prev == nullptr)
				{
					this->head = b->next;
				}
				else
				{
					a->prev->next = b->next;
				}

				if (b->next == nullptr)
				{
					this->tail = a->prev;
				}
				else
				{
					b->next->prev = a->prev;
				}

				return *this;
			}

			bool isEmpty() {
				return this->head == nullptr;
			}
		};

		class Face;


		class HalfEdge {
		public:

			VertexNode* vertex;

			HalfEdge* prev = nullptr;
			HalfEdge* next = nullptr;
			HalfEdge* twin = nullptr;

			Face* face;

			HalfEdge(VertexNode* vertex, Face* face) : vertex(vertex), face(face) {}
			~HalfEdge() {

			}
			VertexNode* head() {
				return this->vertex;
			}
			VertexNode* tail() {
				return this->prev != nullptr ? this->prev->vertex : nullptr;
			}
			float length() {
				auto head = this->head();
				auto tail = this->tail();

				if (tail != nullptr) {
					return tail->point.distanceTo(head->point);
				}

				return -1;
			}
			float lengthSquared() {
				auto head = this->head();
				auto tail = this->tail();

				if (tail != nullptr) {
					return tail->point.distanceToSquared(head->point);
				}

				return -1;
			}
			HalfEdge& setTwin(HalfEdge* edge) {
				this->twin = edge;
				edge->twin = this;

				return *this;
			}
		};

		class Face {
		public:
			Vector3 normal;

			Vector3 midPoint;

			float area = 0;

			float constant = 0;

			VertexNode* outside = nullptr;

			int mark=0;

			HalfEdge* edge = nullptr;

			
			Face() {}
			~Face() {
				auto e0 = this->edge;
				auto e1 = e0->next;
				auto e2 = e0->prev;

				if(e2) delete e2;
				if(e1) delete e1;
				if(e0) delete e0;

			}
			
			static Face* create(VertexNode* a, VertexNode* b, VertexNode* c) {
				auto face = new Face();

				auto e0 = new HalfEdge(a, face);
				auto e1 = new HalfEdge(b, face);
				auto e2 = new HalfEdge(c, face);

				e0->next = e2->prev = e1;
				e1->next = e0->prev = e2;
				e2->next = e1->prev = e0;

				face->edge = e0;

				face->compute();

				return face;
			}
			
			HalfEdge* getEdge(int i) {				
				auto tempEdge = this->edge;
				while (i > 0) {
					tempEdge = tempEdge->next;
					i--;
				}
				while (i < 0) {
					tempEdge = tempEdge->prev;
					i++;
				}
				return tempEdge;
			}

			Face& compute() {
				Triangle triangle;

				auto a = this->edge->tail();
				auto b = this->edge->head();
				auto c = this->edge->next->head();

				triangle.set(a->point, b->point, c->point);

				triangle.getNormal(&normal);

				triangle.getMidpoint(&midPoint);

				area = triangle.getArea();

				constant = normal.dot(midPoint);

				return *this;
			}
			float distanceToPoint(Vector3& point) {
				return normal.dot(point) - constant;
			}
		};
		
		class ConvexHull {
		private :
			Vector3 v1;
		public:
			int visible = 0;
			int deleted = 1;
			float tolerance = -1;

			std::vector<Face*> faces;
			std::vector<Face*> newFaces;
			VertexList assigned;
			VertexList unassigned;

			std::vector<VertexNode*> vertices;

			ConvexHull() {}
			~ConvexHull() {
				if (faces.size() > 0) {
					for (auto face : faces) {
						if (face) delete face;													
					}
				}
				if (newFaces.size() > 0) {
					for (auto face : newFaces) {
						if (face) delete face;
					}
				}
				if (vertices.size() > 0) {
					for (auto vertex : vertices)
						if (vertex) delete vertex;
				}
			}
			ConvexHull& setFromPoints(std::vector<Vector3>& points) {

				assert(points.size() > 4);
				

				this->makeEmpty();

				for (int i = 0;i < points.size();i++)
				{
					vertices.push_back(new VertexNode(points[i]));
				}

				this->compute();

				return *this;
			}
			
			ConvexHull& setFromObject(Object3D& object3D)
			{

				std::vector<Vector3> points;

				object3D.updateMatrixWorld(true);

				object3D.traverse([&](Object3D& node) {

					Vector3 point;

					auto geometry = node.geometry;

					if (geometry != nullptr)
					{

						if (geometry->type=="Geometry")
						{

							auto vertices = geometry->vertices;

							for (int i = 0;i < vertices.size(); i++)
							{

								point = (Vector3)vertices[i].clone();
								point.applyMatrix4(node.matrixWorld);

								points.push_back(point);

							}

						}
						else if (geometry->type == "BufferGeometry")
						{
							BufferGeometry::ptr bufferGeometry = std::dynamic_pointer_cast<BufferGeometry>(geometry);
							auto attribute = bufferGeometry->getAttribute(AttributeName::position);

							if (attribute != nullptr)
							{
								for (int i = 0;i < attribute->count;i++)
								{
									point = Vector3();
									point.fromBufferAttribute<float>(*attribute, i).applyMatrix4(node.matrixWorld);

									points.push_back(point);

								}
							}
						}
					}
				});

				return this->setFromPoints(points);

			}
			ConvexHull& makeEmpty()
			{
				if (faces.size() > 0) {
					for (auto face : faces) {
						if (face) delete face;
					}
				}
				faces.clear();
				if (vertices.size() > 0) {
					for (auto vertex : vertices)
						if (vertex) delete vertex;
				}
				vertices.clear();

				return *this;

			}
			bool containsPoint(Vector3& point)
			{
				for (int i = 0;i < faces.size();i++)
				{

					auto face = faces[i];

					// compute signed distance and check on what half space the point lies

					if (face->distanceToPoint(point) > this->tolerance) return false;

				}

				return true;

			}
			// Adds a vertex to the 'assigned' list of vertices and assigns it to the given face

			ConvexHull& addVertexToFace(VertexNode* vertex, Face* face)
			{

				vertex->face = face;

				if (face->outside == nullptr)
				{

					assigned.append(vertex);

				}
				else
				{

					assigned.insertBefore(face->outside, vertex);

				}

				face->outside = vertex;

				return *this;

			}

			// Removes a vertex from the 'assigned' list of vertices and from the given face

			ConvexHull& removeVertexFromFace(VertexNode* vertex, Face* face)
			{

				if (vertex == face->outside)
				{

					// fix face.outside link

					if (vertex->next != nullptr && vertex->next->face == face)
					{

						// face has at least 2 outside vertices, move the 'outside' reference

						face->outside = vertex->next;

					}
					else
					{

						// vertex was the only outside vertex that face had

						face->outside = nullptr;

					}

				}

				assigned.remove(vertex);

				return *this;

			}

			// Removes all the visible vertices that a given face is able to see which are stored in the 'assigned' vertext list

			VertexNode* removeAllVerticesFromFace(Face* face)
			{

				if (face->outside != nullptr)
				{

					// reference to the first and last vertex of this face

					auto start = face->outside;
					auto end = face->outside;

					while (end->next != nullptr && end->next->face == face)
					{

						end = end->next;

					}

					assigned.removeSubList(start, end);

					// fix references

					start->prev = end->next = nullptr;
					face->outside = nullptr;

					return start;

				}
				return nullptr;

			}

			// Removes all the visible vertices that 'face' is able to see

			ConvexHull& deleteFaceVertices(Face* face, Face* absorbingFace = nullptr)
			{

				auto faceVertices = removeAllVerticesFromFace(face);

				if (faceVertices != nullptr)
				{

					if (absorbingFace == nullptr)
					{

						// mark the vertices to be reassigned to some other face

						unassigned.appendChain(faceVertices);


					}
					else
					{

						// if there's an absorbing face try to assign as many vertices as possible to it

						auto vertex = faceVertices;

						do
						{

							// we need to buffer the subsequent vertex at this point because the 'vertex.next' reference
							// will be changed by upcoming method calls

							auto nextVertex = vertex->next;

							auto distance = absorbingFace->distanceToPoint(vertex->point);

							// check if 'vertex' is able to see 'absorbingFace'

							if (distance > tolerance)
							{

								addVertexToFace(vertex, absorbingFace);

							}
							else
							{

								unassigned.append(vertex);

							}

							// now assign next vertex

							vertex = nextVertex;

						} while (vertex != nullptr);

					}

				}

				return *this;

			}

			// Reassigns as many vertices as possible from the unassigned list to the new faces

			ConvexHull& resolveUnassignedPoints(std::vector<Face*>& newFaces)
			{

				if (unassigned.isEmpty() == false)
				{

					auto vertex = unassigned.first();

					do
					{

						// buffer 'next' reference, see .deleteFaceVertices()

						auto nextVertex = vertex->next;

						auto maxDistance = this->tolerance;

						Face* maxFace = nullptr;

						for (auto i = 0; i < newFaces.size(); i++)
						{

							auto face = newFaces[i];

							if (face->mark == visible)
							{

								auto distance = face->distanceToPoint(vertex->point);

								if (distance > maxDistance)
								{

									maxDistance = distance;
									maxFace = face;

								}

								if (maxDistance > 1000 * this->tolerance) break;

							}

						}

						// 'maxFace' can be nullptr e.g. if there are identical vertices

						if (maxFace != nullptr)
						{

							this->addVertexToFace(vertex, maxFace);

						}

						vertex = nextVertex;

					} while (vertex != nullptr);

				}

				return* this;

			}

			// Computes the extremes of a simplex which will be the initial hull

			std::vector<std::vector<VertexNode*>> computeExtremes()
			{

				Vector3 min;
				Vector3 max;

				std::vector<VertexNode*> minVertices;
				std::vector<VertexNode*> maxVertices;

				int i, l, j;

				// initially assume that the first vertex is the min/max

				for (i = 0; i < 3; i++)
				{

					minVertices.push_back(this->vertices[0]);
					maxVertices.push_back(this->vertices[0]);

				}

				min.copy(this->vertices[0]->point);
				max.copy(this->vertices[0]->point);

				// compute the min/max vertex on all six directions

				for (i = 0;i < this->vertices.size();i++)
				{

					auto vertex = this->vertices[i];
					auto point = vertex->point;

					// update the min coordinates

					for (j = 0; j < 3; j++)
					{

						if (point.getComponent(j) < min.getComponent(j))
						{

							min.setComponent(j, point.getComponent(j));
							minVertices[j] = vertex;

						}

					}

					// update the max coordinates

					for (j = 0; j < 3; j++)
					{

						if (point.getComponent(j) > max.getComponent(j))
						{

							max.setComponent(j, point.getComponent(j));
							maxVertices[j] = vertex;

						}

					}

				}

				// use min/max vectors to compute an optimal epsilon

				this->tolerance = 3 * std::numeric_limits<float>::epsilon() * (
					std::max(abs(min.x), abs(max.x)) +
					std::max(abs(min.y), abs(max.y)) +
					std::max(abs(min.z), abs(max.z))

					);
				std::vector<std::vector<VertexNode*>> result = { minVertices,maxVertices };

				return result;

			}

			// Computes the initial simplex assigning to its faces all the points
			// that are candidates to form part of the hull

		    ConvexHull& computeInitialHull()
			{
				Line3 line3;
				Plane plane;
				Vector3 closestPoint;

				VertexNode* vertex=nullptr;

				

				auto extremes = this->computeExtremes();

				auto min = extremes[0];

				auto max = extremes[1];

				VertexNode* v0;
				VertexNode* v1;
				VertexNode* v2;
				VertexNode* v3;
				v0 = v1 = v2 = v3 = nullptr;
				int i, l, j;

				// 1. Find the two vertices 'v0' and 'v1' with the greatest 1d separation
				// (max.x - min.x)
				// (max.y - min.y)
				// (max.z - min.z)

				float distance, maxDistance = 0;
				int index = 0;

				for (i = 0; i < 3; i++)
				{

					distance = max[i]->point.getComponent(i) - min[i]->point.getComponent(i);

					if (distance > maxDistance)
					{

						maxDistance = distance;
						index = i;

					}

				}

				v0 = min[index];
				v1 = max[index];

				// 2. The next vertex 'v2' is the one farthest to the line formed by 'v0' and 'v1'

				maxDistance = 0;
				line3.set(v0->point, v1->point);

				for (i = 0;i < this->vertices.size(); i++)
				{

					vertex = vertices[i];

					if (vertex != v0 && vertex != v1)
					{

						line3.closestPointToPoint(vertex->point, true, &closestPoint);

						distance = closestPoint.distanceToSquared(vertex->point);

						if (distance > maxDistance)
						{

							maxDistance = distance;
							v2 = vertex;

						}

					}

				}

				// 3. The next vertex 'v3' is the one farthest to the plane 'v0', 'v1', 'v2'

				maxDistance = -1;
				plane.setFromCoplanarPoints(v0->point, v1->point, v2->point);

				for (i = 0;i < this->vertices.size(); i++)
				{

					vertex = vertices[i];

					if (vertex != v0 && vertex != v1 && vertex != v2)
					{

						distance = abs(plane.distanceToPoint(vertex->point));

						if (distance > maxDistance)
						{

							maxDistance = distance;
							v3 = vertex;

						}

					}

				}

				std::vector<Face*>  _faces;

				if (plane.distanceToPoint(v3->point) < 0)
				{

					// the face is not able to see the point so 'plane.normal' is pointing outside the tetrahedron

					_faces.push_back(Face::create(v0, v1, v2));
					_faces.push_back(Face::create(v3, v1, v0));
					_faces.push_back(Face::create(v3, v2, v1));
					_faces.push_back(Face::create(v3, v0, v2));
					

					// set the twin edge

					for (i = 0; i < 3; i++)
					{

						j = (i + 1) % 3;

						// join face[ i ] i > 0, with the first face

						_faces[i + 1]->getEdge(2)->setTwin(_faces[0]->getEdge(j));

						// join face[ i ] with face[ i + 1 ], 1 <= i <= 3

						_faces[i + 1]->getEdge(1)->setTwin(_faces[j + 1]->getEdge(0));

					}

				}
				else
				{

					// the face is able to see the point so 'plane.normal' is pointing inside the tetrahedron				
					
					_faces.push_back(Face::create(v0, v2, v1));
					_faces.push_back(Face::create(v3, v0, v1));
					_faces.push_back(Face::create(v3, v1, v2));
					_faces.push_back(Face::create(v3, v2, v0));
					

					// set the twin edge

					for (i = 0; i < 3; i++)
					{

						j = (i + 1) % 3;

						// join face[ i ] i > 0, with the first face

						_faces[i + 1]->getEdge(2)->setTwin(_faces[0]->getEdge((3 - i) % 3));

						// join face[ i ] with face[ i + 1 ]

						_faces[i + 1]->getEdge(0)->setTwin(_faces[j + 1]->getEdge(1));

					}

				}

				// the initial hull is the tetrahedron

				for (i = 0; i < 4; i++)
				{

					this->faces.push_back(_faces[i]);

				}

				// initial assignment of vertices to the faces of the tetrahedron

				for (i = 0;i < vertices.size(); i++)
				{

					vertex = vertices[i];

					if (vertex != v0 && vertex != v1 && vertex != v2 && vertex != v3)
					{

						maxDistance = this->tolerance;
						Face* maxFace = nullptr;

						for (j = 0; j < 4; j++)
						{

							distance = this->faces[j]->distanceToPoint(vertex->point);

							if (distance > maxDistance)
							{

								maxDistance = distance;
								maxFace = this->faces[j];

							}

						}

						if (maxFace != nullptr)
						{

							this->addVertexToFace(vertex, maxFace);

						}

					}

				}

				return *this;

			}


			// Removes inactive faces

			ConvexHull& reindexFaces()
			{

				std::vector<Face*> activeFaces;;

				for (auto i = 0; i < this->faces.size(); i++)
				{

					auto face = this->faces[i];

					if (face->mark == this->visible)
					{

						activeFaces.push_back(face);

					}

				}

				this->faces.clear();
				this->faces.shrink_to_fit();
				this->faces.resize(activeFaces.size());
				std::copy(activeFaces.begin(), activeFaces.end(), this->faces.begin());

				return *this;

			}

			// Finds the next vertex to create faces with the current hull

			VertexNode* nextVertexToAdd()
			{

				// if the 'assigned' list of vertices is empty, no vertices are left. return with 'undefined'

				if (this->assigned.isEmpty() == false)
				{

					VertexNode* eyeVertex = nullptr;
					float maxDistance = 0;

					// grap the first available face and start with the first visible vertex of that face

					auto eyeFace = this->assigned.first()->face;
					auto vertex = eyeFace->outside;

					// now calculate the farthest vertex that face can see

					do
					{

						auto distance = eyeFace->distanceToPoint(vertex->point);

						if (distance > maxDistance)
						{

							maxDistance = distance;
							eyeVertex = vertex;

						}

						vertex = vertex->next;

					} while (vertex != nullptr && vertex->face == eyeFace);

					return eyeVertex;

				}
				return nullptr;

			}

			// Computes a chain of half edges in CCW order called the 'horizon'.
			// For an edge to be part of the horizon it must join a face that can see
			// 'eyePoint' and a face that cannot see 'eyePoint'.

			ConvexHull& computeHorizon(Vector3& eyePoint, HalfEdge* crossEdge, Face* face, std::vector<HalfEdge*>& horizon)
			{

				// moves face's vertices to the 'unassigned' vertex list

				this->deleteFaceVertices(face);

				face->mark = deleted;

				HalfEdge* edge;

				if (crossEdge == nullptr)
				{

					edge = crossEdge = face->getEdge(0);

				}
				else
				{

					// start from the next edge since 'crossEdge' was already analyzed
					// (actually 'crossEdge.twin' was the edge who called this method recursively)

					edge = crossEdge->next;

				}

				do
				{

					HalfEdge* twinEdge = edge->twin;
					//if (twinEdge == nullptr) break;
					auto oppositeFace = twinEdge->face;

					if (oppositeFace->mark == visible)
					{

						if (oppositeFace->distanceToPoint(eyePoint) > this->tolerance)
						{

							// the opposite face can see the vertex, so proceed with next edge

							this->computeHorizon(eyePoint, twinEdge, oppositeFace, horizon);

						}
						else
						{

							// the opposite face can't see the vertex, so this edge is part of the horizon

							horizon.push_back(edge);

						}

					}

					edge = edge->next;

				} while (edge != crossEdge);

				return *this;

			}

			// Creates a face with the vertices 'eyeVertex.point', 'horizonEdge.tail' and 'horizonEdge.head' in CCW order

			HalfEdge* addAdjoiningFace(VertexNode* eyeVertex, HalfEdge* horizonEdge)
			{

				// all the half edges are created in ccw order thus the face is always pointing outside the hull

				auto face = Face::create(eyeVertex, horizonEdge->tail(), horizonEdge->head());

				this->faces.push_back(face);

				// join face.getEdge( - 1 ) with the horizon's opposite edge face.getEdge( - 1 ) = face.getEdge( 2 )

				face->getEdge(-1)->setTwin(horizonEdge->twin);

				return face->getEdge(0); // the half edge whose vertex is the eyeVertex


			}

			//  Adds 'horizon.size()' faces to the hull, each face will be linked with the
			//  horizon opposite face and the face on the left/right

			ConvexHull& addNewFaces(VertexNode* eyeVertex, std::vector<HalfEdge*> horizon)
			{

				this->newFaces.clear();

				HalfEdge* firstSideEdge = nullptr;
				HalfEdge* previousSideEdge = nullptr;

				for (auto i = 0; i < horizon.size(); i++)
				{

					auto horizonEdge = horizon[i];

					// returns the right side edge

					auto sideEdge = this->addAdjoiningFace(eyeVertex, horizonEdge);

					if (firstSideEdge == nullptr)
					{

						firstSideEdge = sideEdge;

					}
					else
					{

						// joins face.getEdge( 1 ) with previousFace.getEdge( 0 )

						sideEdge->next->setTwin(previousSideEdge);

					}

					this->newFaces.push_back(sideEdge->face);
					previousSideEdge = sideEdge;

				}

				// perform final join of new faces

				firstSideEdge->next->setTwin(previousSideEdge);

				return *this;

			}

			// Adds a vertex to the hull

			ConvexHull& addVertexToHull(VertexNode* eyeVertex)
			{

				std::vector<HalfEdge*> horizon;

				this->unassigned.clear();

				// remove 'eyeVertex' from 'eyeVertex->face' so that it can't be added to the 'unassigned' vertex list

				this->removeVertexFromFace(eyeVertex, eyeVertex->face);

				this->computeHorizon(eyeVertex->point, nullptr, eyeVertex->face, horizon);

				this->addNewFaces(eyeVertex, horizon);

				// reassign 'unassigned' vertices to the new faces

				this->resolveUnassignedPoints(this->newFaces);

				return *this;

			}

			ConvexHull& Cleanup()
			{

				this->assigned.clear();
				this->unassigned.clear();
				this->newFaces.clear();

				return *this;

			}

			ConvexHull& compute()
			{

				VertexNode* vertex;

				this->computeInitialHull();

				// add all available vertices gradually to the hull

				while ((vertex = this->nextVertexToAdd()) != nullptr)
				{

					this->addVertexToHull(vertex);

				}

				this->reindexFaces();

				this->Cleanup();

				return *this;

			}

		};
	}
}
#endif