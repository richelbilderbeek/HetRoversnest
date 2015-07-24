#ifndef QTGAMEDIALOG_H
#define QTGAMEDIALOG_H

#include <QDialog>
#include "option.h"
#include "character.h"

namespace Ui { class QtGameDialog; }

class QtGameDialog : public QDialog
{
  Q_OBJECT

public:
  explicit QtGameDialog(QWidget *parent = 0);
  ~QtGameDialog();


protected:
  void keyPressEvent(QKeyEvent *);

private slots:
  void Start();

private:
  Ui::QtGameDialog *ui;

  Character m_character;
  bool m_has_lost;
  bool m_has_won;

  int m_key_pressed;

  std::vector<Option> m_options;

  void DoChapter();

  void SlotWait();

  void SlotShowText(const std::string& text);
  Option SlotRequestOption(const std::vector<Option>& options);

  void UpdateStats();
};

#endif // QTGAMEDIALOG_H
