#include "qtgamedialog.h"

#include <QKeyEvent>
#include <QTimer>
#include <QTextCursor>

#include <boost/lexical_cast.hpp>

#include "ui_qtgamedialog.h"
#include "chapter.h"
#include "menudialog.h"
#include "helper.h"
#include "dice.h"

QtGameDialog::QtGameDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QtGameDialog),
  //m_character(6,12,12,Item::luck_potion),
  //m_has_lost{false},
  //m_has_won{false},
  m_key_pressed{-1},
  m_options{}
{
  ui->setupUi(this);

  Dice::Get()->SetSeed(42);

  QTimer::singleShot(100,Qt::VeryCoarseTimer,this,SLOT(Start()));

  UpdateStats();
}

void QtGameDialog::keyPressEvent(QKeyEvent * e)
{
  int key_pressed = e->key();
  if (key_pressed == Qt::Key_Escape) close();
  if (m_options.size() > 0 && key_pressed == Qt::Key_0) { m_key_pressed = 0; return; }
  if (m_options.size() > 1 && key_pressed == Qt::Key_1) { m_key_pressed = 1; return; }
  if (m_options.size() > 2 && key_pressed == Qt::Key_2) { m_key_pressed = 2; return; }
  if (m_options.size() > 3 && key_pressed == Qt::Key_3) { m_key_pressed = 3; return; }
  if (m_options.size() > 4 && key_pressed == Qt::Key_4) { m_key_pressed = 4; return; }
  if (m_options.size() > 5 && key_pressed == Qt::Key_5) { m_key_pressed = 5; return; }
  if (m_options.size() > 6 && key_pressed == Qt::Key_6) { m_key_pressed = 6; return; }
  if (m_options.size() > 7 && key_pressed == Qt::Key_7) { m_key_pressed = 7; return; }
  if (m_options.size() > 8 && key_pressed == Qt::Key_8) { m_key_pressed = 8; return; }
  if (m_options.size() > 9 && key_pressed == Qt::Key_9) { m_key_pressed = 9; return; }
  assert(m_options.size() < 10);
}

void QtGameDialog::Start()
{
  MenuDialog menu;

  menu.m_signal_request_option.connect(
    boost::bind(&QtGameDialog::SlotRequestOption,this,_1)
  );
  menu.m_signal_wait.connect(
    boost::bind(&QtGameDialog::SlotWait,this)
  );
  menu.m_signal_show_text.connect(
    boost::bind(&QtGameDialog::SlotShowText,this,_1)
  );

  menu.Execute();

  close();
}

Option QtGameDialog::SlotRequestOption(const std::vector<Option>& options)
{
  m_options = options;
  std::vector<int> valid_indices;
  std::stringstream text;
  const int n_options{static_cast<int>(options.size())};
  for (int i=0; i!=n_options; ++i)
  {
    valid_indices.push_back(i);
    text << "[" << i << "] " << options[i].GetText() << '\n';
  }
  SlotShowText(text.str());

  while(1)
  {
    qApp->processEvents();
    if (m_key_pressed != -1)
    {
      int key_pressed = m_key_pressed;
      assert(key_pressed >= 0);
      assert(key_pressed < static_cast<int>(options.size()));
      m_key_pressed = -1;
      return options[key_pressed];
    }
  }
}

void QtGameDialog::SlotShowText(const std::string& text)
{
  QTextCursor d(ui->plainTextEdit->textCursor());
  for (const char c: text)
  {
    const std::string s{boost::lexical_cast<std::string>(c)};
    d.insertText(s.c_str());
    #ifdef NDEBUG
    Wait(0.001); //Only have suspense in release mode
    #endif
    ui->plainTextEdit->moveCursor(QTextCursor::End);
    qApp->processEvents();
  }
  UpdateStats();
}

void QtGameDialog::SlotWait()
{
  #ifdef NDEBUG
  Wait(1.0); //Only have suspense in release mode
  #endif
}

QtGameDialog::~QtGameDialog()
{
  delete ui;
}

void QtGameDialog::UpdateStats()
{
  /*
  ui->label_condition->setText(
    (
      std::string("Condition: ")
    + std::to_string(m_character.GetCondition())
    + "/"
    + std::to_string(m_character.GetInitialCondition())
    ).c_str()
  );
  ui->label_skill->setText(
    (
      std::string("Skill: ")
    + std::to_string(m_character.GetSkill())
    + "/"
    + std::to_string(m_character.GetInitialSkill())
    ).c_str()
  );
  ui->label_luck->setText(
    (
      std::string("Luck: ")
    + std::to_string(m_character.GetLuck())
    + "/"
    + std::to_string(m_character.GetInitialLuck())
    ).c_str()
  );
  ui->label_gold->setText(
    (
      std::string("Gold: ")
    + std::to_string(m_character.GetGold())
    ).c_str()
  );
  ui->label_provisions->setText(
    (
      std::string("Provisions: ")
    + std::to_string(m_character.GetProvisions())
    ).c_str()
  );
  */
}
