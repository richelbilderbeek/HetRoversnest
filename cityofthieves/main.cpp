#include "terminal.h"
#include "menudialog.h"

#include <iostream>
#include <stdexcept>

int main()
{
  try
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

    menu.Execute();
  }
  catch (std::logic_error& e)
  {
    std::cerr << e.what() << std::endl;
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
