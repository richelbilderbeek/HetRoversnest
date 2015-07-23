#ifndef QTDIALOG_H
#define QTDIALOG_H

#include <QDialog>

#include "character.h"

namespace Ui { class QtDialog; }

class QtDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtDialog(QWidget *parent = 0);
  ~QtDialog();

private slots:
  void on_lineEdit_returnPressed();
  void StartChapter();

private:
  Ui::QtDialog *ui;

  Character m_character;
  bool m_has_lost;
  bool m_has_won;

  int m_user_input; //After pressing enter

  void DoChapter();

  int SlotRequestInput(const std::vector<int> valid_inputs);
  void SlotShowText(const std::string text);
  void SlotWait();

};

#endif // QTDIALOG_H
