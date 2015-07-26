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
  m_key_pressed{-1},
  m_options{}
{
  ui->setupUi(this);

  Dice::Get()->SetSeed(42);

  QTimer::singleShot(100,Qt::VeryCoarseTimer,this,SLOT(Start()));
}

QtGameDialog::~QtGameDialog()
{
  delete ui;
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
  if (m_options.size() > 10 && key_pressed == Qt::Key_A) { m_key_pressed = 10; return; }
  if (m_options.size() > 11 && key_pressed == Qt::Key_B) { m_key_pressed = 11; return; }
  if (m_options.size() > 12 && key_pressed == Qt::Key_C) { m_key_pressed = 12; return; }
  if (m_options.size() > 13 && key_pressed == Qt::Key_D) { m_key_pressed = 13; return; }
  if (m_options.size() > 14 && key_pressed == Qt::Key_E) { m_key_pressed = 14; return; }
  if (m_options.size() > 15 && key_pressed == Qt::Key_F) { m_key_pressed = 15; return; }
  if (m_options.size() > 16 && key_pressed == Qt::Key_G) { m_key_pressed = 16; return; }
  if (m_options.size() > 17 && key_pressed == Qt::Key_H) { m_key_pressed = 17; return; }
  assert(m_options.size() < 19);
}

void QtGameDialog::SlotCharacterChanged(const Character &character)
{
  ui->label_condition->setText(
    (
      std::string("Condition: ")
    + std::to_string(character.GetCondition())
    + "/"
    + std::to_string(character.GetInitialCondition())
    ).c_str()
  );
  ui->label_skill->setText(
    (
      std::string("Skill: ")
    + std::to_string(character.GetSkill())
    + "/"
    + std::to_string(character.GetInitialSkill())
    ).c_str()
  );
  ui->label_luck->setText(
    (
      std::string("Luck: ")
    + std::to_string(character.GetLuck())
    + "/"
    + std::to_string(character.GetInitialLuck())
    ).c_str()
  );
  ui->label_gold->setText(
    (
      std::string("Gold: ")
    + std::to_string(character.GetGold())
    ).c_str()
  );
  ui->label_provisions->setText(
    (
      std::string("Provisions: ")
    + std::to_string(character.GetProvisions())
    ).c_str()
  );
  {
    std::stringstream text;
    text << "Items:\n";
    for (const auto item: character.GetItems())
    {
      if (static_cast<int>(item) < 100)
      {
        text << " * " << ToPrettyStr(item) << "\n";
      }
    }
    std::string s{text.str()}; s.pop_back(); //Remove newline at end
    ui->label_items->setText(s.c_str());
  }
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
  if (n_options == 1) { return options[0]; }

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
}

void QtGameDialog::SlotWait()
{
  #ifdef NDEBUG
  Wait(1.0); //Only have suspense in release mode
  #endif
}

void QtGameDialog::Start()
{
  MenuDialog menu;

  menu.m_signal_character_has_changed.connect(
    boost::bind(&QtGameDialog::SlotCharacterChanged,this,_1)
  );

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
