#include "viewer.h"
#include<GLUT/glut.h>
using namespace std;

Viewer::Viewer(string meshFile) {
  this->meshFile = meshFile;
}

void Viewer::init()
{
	restoreStateFromFile();

    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                   // Black Background
    glClearDepth(1.0f);								// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective 

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 500.0, 500.0, 500.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	//glShadeModel (GL_FLAT);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	setSceneRadius(1);          // scene has a 100 OpenGL units radius 
	setSceneCenter(qglviewer::Vec(0.0 ,0.0, 0.001) ); // with a center shifted by 400 units along X direction
	camera()->setUpVector(qglviewer::Vec(0,1,0));
	camera()->showEntireScene();
	readMesh();
	//norm_vertex();
}

void Viewer::draw_mesh()
{
	PolyMesh::FaceIter f_it;
	PolyMesh::FaceVertexIter fv_it;
	PolyMesh::Point tem_pt;
	PolyMesh::TexCoord2D tem_tex;
	PolyMesh::Normal tem_norm;
	glPushAttrib( GL_CURRENT_BIT );
	glColor4f(1.0, 1.0, 1.0, 1.0);
	for(f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		glBegin(GL_TRIANGLES);
		
		for(fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			tem_pt = mesh.point(*fv_it);
			tem_norm = mesh.normal(*fv_it);
			glNormal3f(tem_norm.data()[0], tem_norm.data()[1], tem_norm.data()[2]);
			glVertex3f(tem_pt.data()[0], tem_pt.data()[1], tem_pt.data()[2]);
		}
        glEnd();
	}
	glPopAttrib();
}

void Viewer::readMesh()
{
	OpenMesh::IO::Options opt;
	mesh.request_vertex_normals();
	mesh.request_vertex_texcoords2D();
	opt += OpenMesh::IO::Options::VertexNormal;
	opt += OpenMesh::IO::Options::VertexTexCoord;
	if (!OpenMesh::IO::read_mesh(mesh, meshFile, opt)) 
	{
		std::cerr << "read error\n"<<endl;
	}
}

void Viewer::norm_vertex()
{
	PolyMesh::VertexIter v_it;
	PolyMesh::Point tem_pt;
	PolyMesh::Point center_pt;
	float max_x = FLT_MIN, max_y = FLT_MIN, max_z = FLT_MIN, min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX;
	for(v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		tem_pt = mesh.point(*v_it);
		center_pt += tem_pt / mesh.n_vertices();
		max_x = max(max_x, tem_pt.data()[0]); max_y = max(max_y, tem_pt.data()[1]); max_z = max(max_z, tem_pt.data()[2]);
		min_x = min(min_x, tem_pt.data()[0]); min_y = min(min_y, tem_pt.data()[1]); min_z = min(min_z, tem_pt.data()[2]);
	}

	float scale = FLT_MIN;
	scale = max(max_x - min_x, max(max_y - min_y, max_z - min_z));
	for(v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		tem_pt = mesh.point(*v_it);
		tem_pt -= center_pt;
		tem_pt /= scale;
		mesh.set_point(*v_it, tem_pt);
	}
}

void Viewer::draw()
{
	draw_mesh();
}

Viewer::~Viewer() {
}
