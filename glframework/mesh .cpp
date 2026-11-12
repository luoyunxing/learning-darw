#include "mesh.h"

Mesh::Mesh(Geometry* geometry, Meterial* meterial)
{
	mGemetry = geometry;
	mMeterial = meterial;

	mType = ObjectType::Mesh;
}
Mesh::~Mesh()
{

}