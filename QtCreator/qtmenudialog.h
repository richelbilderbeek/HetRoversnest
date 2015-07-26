#ifndef QTMENUDIALOG_H
#define QTMENUDIALOG_H

#include <QDialog>

namespace Ui {
class QtMenuDialog;
}

class QtMenuDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtMenuDialog(QWidget *parent = 0);
  ~QtMenuDialog();

private:
  Ui::QtMenuDialog *ui;


private slots:
/*
  void ShowAbout();
  void ShowIntroduction();
  void ShowManual();
  void ShowTeaser();
  void ShowHints();
  */
  void StartGame();
};

#endif // QTMENUDIALOG_H
