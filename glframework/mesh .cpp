#include "mesh.h"

Mesh::Mesh(Geometry* geometry, Meterial* meterial)
{
	mGemetry = geometry;
	mMeterial = meterial;
}
Mesh::~Mesh()
{

}