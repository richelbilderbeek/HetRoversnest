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
  m_key_pressed{-1}
{
  ui->setupUi(this);

  Dice::Get()->SetSeed(42);

  QTimer::singleShot(100,Qt::VeryCoarseTimer,this,SLOT(Start()));
}

void QtGameDialog::keyPressEvent(QKeyEvent * e)
{
  m_key_pressed = e->key();
  if (m_key_pressed == Qt::Key_Escape) close();
}

void QtGameDialog::Start()
{
  while (1)
  {
    if (m_has_lost || m_has_won) return;

    const int chapter_number{m_character.GetCurrentChapter()};
    if (chapter_number == 400)
    {
      m_has_won = true;
      return;
    }

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

    if (m_character.IsDead()) { m_has_lost = true; }
  }
}

Option QtGameDialog::SlotRequestOption(const std::vector<Option>& options)
{
  while(1)
  {
    qApp->processEvents();
    if (m_key_pressed != -1)
    {
      m_key_pressed = -1;
      return options[0];
    }
    //QtPressKeyDialog d;
    //d.exec();
    //return options[0];
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
}

void QtGameDialog::SlotWait()
{
  return;
}

QtGameDialog::~QtGameDialog()
{
  delete ui;
}
