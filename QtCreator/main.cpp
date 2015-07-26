#include "terminal.h"
#include "menudialog.h"

int main()
{
  Terminal dialog;
  MenuDialog menu;
  menu.m_signal_request_option.connect(
    boost::bind(&Terminal::SlotRequestOption,dialog,_1)
  );
  menu.m_signal_wait.connect(
    boost::bind(&Terminal::SlotWait,dialog)
  );
  menu.m_signal_show_text.connect(
    boost::bind(&Terminal::SlotShowText,dialog,_1)
  );

  menu.ShowZanbarBone();

  menu.Execute();
}
