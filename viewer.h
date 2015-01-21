#ifndef VIEWER_H
#define VIEWER_H

#include <QGLViewer/qglviewer.h>
#include <iostream>
#include <vector>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<>  PolyMesh;
typedef PolyMesh::Point Point3;
using namespace OpenMesh;
using namespace std;

class Viewer : public QGLViewer
{
public:
  Viewer(string meshFile);
  ~Viewer();
  void readMesh();
	void norm_vertex();
	void draw_mesh();
	void voxelize();

protected :
	virtual void draw();
	virtual void init();

private:
	PolyMesh mesh;
  string meshFile;
};

#endif
