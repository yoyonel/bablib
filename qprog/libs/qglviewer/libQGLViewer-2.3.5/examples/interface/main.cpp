/****************************************************************************

 Copyright (C) 2002-2008 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.3.5.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include <qapplication.h>

#if QT_VERSION >= 0x040000
# include "ui_viewerInterface.Qt4.h"
  class ViewerInterface : public QDialog, public Ui::Dialog
  {
  public:
    ViewerInterface() { setupUi(this); }
  };
#else
# include "interface.h"
# include "viewerInterface.Qt3.h"
#endif

int main(int argc, char** argv)
{
  QApplication application(argc,argv);

  ViewerInterface vi;

#if QT_VERSION < 0x040000
  application.setMainWidget(&vi);
#else
  vi.setWindowTitle("interface");
#endif

  vi.show();

  return application.exec();
}