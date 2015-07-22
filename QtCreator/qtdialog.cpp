#include "qtdialog.h"
#include "ui_qtdialog.h"

QtDialog::QtDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QtDialog)
{
  ui->setupUi(this);
}

QtDialog::~QtDialog()
{
  delete ui;
}
