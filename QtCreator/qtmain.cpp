#include <QApplication>
#include "qtdialog.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QtDialog w;
  w.setWindowState(Qt::WindowFullScreen);
  w.show();
  return a.exec();
}
