#include <QApplication>
#include "qtgamedialog.h"
#include "qtmenudialog.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QtGameDialog w;
  //QtMenuDialog w;
  w.setWindowState(Qt::WindowFullScreen);
  w.show();
  return a.exec();
}


// GOOD FONTS:
// URW Chancery L
// UnPilgi
// eufm10

// BAD FONTS:
// jsMath-eufb10
