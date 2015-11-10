#include "manipulatedFrame.h"
#include <qapplication.h>
#include <QtGui>

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Create a viewer, show it on screen.
  Viewer viewer;
  viewer.show();

  // Set the viewer as the application's main widget.
  //application.setMainWidget(&viewer);
  application.setActiveWindow(&viewer);

  // Run main loop.
  return application.exec();
}
