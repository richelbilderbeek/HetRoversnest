#include "qtgamedialog.h"

#include <QKeyEvent>
#include <QTimer>
#include <QTextCursor>

#include <boost/lexical_cast.hpp>

#include "ui_qtgamedialog.h"
#include "chapter.h"
#include "helper.h"
#include "dice.h"

QtGameDialog::QtGameDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QtGameDialog),
  m_character(6,12,12,Item::luck_potion),
  m_has_lost{false},
  m_has_won{false},
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
  assert(m_options.size() < 8);
  int key_pressed = e->key();
  if (key_pressed == Qt::Key_Escape) close();
  if (m_options.size() > 0 && key_pressed == Qt::Key_0) { m_key_pressed = 0; return; }
  if (m_options.size() > 1 && key_pressed == Qt::Key_1) { m_key_pressed = 1; return; }
  if (m_options.size() > 2 && key_pressed == Qt::Key_2) { m_key_pressed = 2; return; }
  if (m_options.size() > 3 && key_pressed == Qt::Key_3) { m_key_pressed = 3; return; }
  if (m_options.size() > 4 && key_pressed == Qt::Key_4) { m_key_pressed = 4; return; }
  if (m_options.size() > 5 && key_pressed == Qt::Key_5) { m_key_pressed = 5; return; }
  if (m_options.size() > 6 && key_pressed == Qt::Key_6) { m_key_pressed = 6; return; }
}

void QtGameDialog::Start()
{
  while (1)
  {
    UpdateStats();

    const int chapter_number{m_character.GetCurrentChapter()};
    const Chapter chapter(chapter_number);

    chapter.m_signal_request_option.connect(
      boost::bind(&QtGameDialog::SlotRequestOption,this,_1)
    );
    chapter.m_signal_wait.connect(
      boost::bind(&QtGameDialog::SlotWait,this)
    );
    chapter.m_signal_show_text.connect(
      boost::bind(&QtGameDialog::SlotShowText,this,_1)
    );

    chapter.Do(m_character);

    if (chapter.GetType() == ChapterType::game_won) { m_has_won = true; }
    if (m_character.IsDead())  { m_has_lost = true; }
    if (m_has_lost || m_has_won) return;
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
    Wait(0.001);
    ui->plainTextEdit->moveCursor(QTextCursor::End);
    qApp->processEvents();
  }
  UpdateStats();
}

void QtGameDialog::SlotWait()
{
  return;
}

QtGameDialog::~QtGameDialog()
{
  delete ui;
}

void QtGameDialog::UpdateStats()
{
  ui->label_condition->setText(
    (
      std::string("Condition: ")
    + std::to_string(m_character.GetStamina())
    + "/"
    + std::to_string(m_character.GetInitialStamina())
    ).c_str()
  );
  ui->label_skill->setText(
    (
      std::string("Skill: ")
    + std::to_string(m_character.GetDexterity())
    + "/"
    + std::to_string(m_character.GetInitialDexterity())
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

}
