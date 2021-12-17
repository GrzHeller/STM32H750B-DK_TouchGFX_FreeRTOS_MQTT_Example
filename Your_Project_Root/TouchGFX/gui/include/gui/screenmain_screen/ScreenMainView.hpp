#ifndef SCREENMAINVIEW_HPP
#define SCREENMAINVIEW_HPP

#include <gui_generated/screenmain_screen/ScreenMainViewBase.hpp>
#include <gui/screenmain_screen/ScreenMainPresenter.hpp>

#include "stdint.h"

class ScreenMainView : public ScreenMainViewBase
{
public:
    ScreenMainView();
    virtual ~ScreenMainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void updateImage(uint8_t data);
    virtual void updateStatus(uint8_t connection_status);

    virtual void functionSubscribe();
    virtual void functionUnsubscribe();

protected:
    uint8_t MQTTConnectionStatus;
};

#endif // SCREENMAINVIEW_HPP
