#include "qtmenudialog.h"


#include "ui_qtmenudialog.h"
#include "qtgamedialog.h"

QtMenuDialog::QtMenuDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QtMenuDialog)
{
  ui->setupUi(this);

  QObject::connect(ui->button_start,SIGNAL(clicked()),this,SLOT(StartGame()));
}

QtMenuDialog::~QtMenuDialog()
{
  delete ui;
}

void QtMenuDialog::StartGame()
{
  QtGameDialog w;
  w.setWindowState(Qt::WindowFullScreen);
  w.exec();
}
