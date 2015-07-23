#include "qtdialog.h"
#include "ui_qtdialog.h"



#include <algorithm>
#include <cassert>


#include <QTimer>

#include "chapter.h"
#include "dice.h"
#include "helper.h"

QtDialog::QtDialog(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::QtDialog),
    m_character(6,12,12,Item::luck_potion),
    m_has_lost{false},
    m_has_won{false},
    m_user_input{-1}
{
  ui->setupUi(this);

  Dice::Get()->SetSeed(42);

  QTimer::singleShot(1000,Qt::VeryCoarseTimer,this,SLOT(StartChapter()));
}

QtDialog::~QtDialog()
{
  delete ui;
}

void QtDialog::DoChapter()
{
  if (m_has_lost || m_has_won) return;

  const int chapter_number{m_character.GetCurrentChapter()};
  if (chapter_number == 400)
  {
    m_has_won = true;
    return;
  }

  const Chapter chapter(chapter_number);

  chapter.m_signal_request_input.connect(
    boost::bind(&QtDialog::SlotRequestInput,this,_1)
  );
  chapter.m_signal_wait.connect(
    boost::bind(&QtDialog::SlotWait,this)
  );
  chapter.m_signal_show_text.connect(
    boost::bind(&QtDialog::SlotShowText,this,_1)
  );


  chapter.Do(m_character);

  if (m_character.IsDead()) { m_has_lost = true; }

}

void QtDialog::StartChapter()
{
  DoChapter();
}

int QtDialog::SlotRequestInput(const std::vector<int> valid_inputs)
{
  while (1)
  {
    qApp->processEvents();
    if (std::find(std::begin(valid_inputs),std::end(valid_inputs),m_user_input) != std::end(valid_inputs))
    {
      const int input{m_user_input};
      m_user_input = -1;
      return input;
    }
  }

  /*
  #ifndef NDEBUG
  assert(valid_inputs.size() >= 2);
  //for (const int s: valid_inputs) { std::cerr << s << " "; }
  //std::cerr << std::endl;
  return valid_inputs[0] == 0 ? valid_inputs[1] : valid_inputs[0];
  #else
  #endif
  */
}

void QtDialog::SlotShowText(const std::string text)
{
  ui->plainTextEdit->appendPlainText(text.c_str());
}

void QtDialog::SlotWait()
{
  return;
}

void QtDialog::on_lineEdit_returnPressed()
{

  const std::string s{ui->lineEdit->text().toStdString()};
  m_user_input = (IsInt(s)) ? std::stoi(s) : -1;
  ui->lineEdit->setText("");
}
