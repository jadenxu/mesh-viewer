#include "viewer.h"
#include <QtGui/QApplication>

int main(int argc, char** argv)
{
	QApplication application(argc,argv);

  string meshFile = "../data/Spider-Man.obj";
	Viewer viewer(meshFile);
	viewer.setWindowTitle("Mesh Viewer");
	viewer.show();

	return application.exec();
}
