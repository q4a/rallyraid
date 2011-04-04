
#include "MenuPageBase.h"

class MenuPageBase : public irr::IEventReceiver
{

MenuPageBase::MenuPageBase()
    : parentMenuPage(0)
      opened(false)
{
}

MenuPageBase::~MenuPageBase()
{
}

void MenuPageBase::openMenu(MenuPageBase* parent)
{
    if (!opened)
    {
        parentMenuPage = parent;
        open();
        opened = true;
        return true;
    }
    return false;
}

MenuPageBase* MenuPageBase::closeMenu()
{
    if (opened)
    {
        MenuPageBase* ret = parentMenuPage;
        close();
        parentMenuPage = 0;
        return ret;
    }
    return 0;
}
