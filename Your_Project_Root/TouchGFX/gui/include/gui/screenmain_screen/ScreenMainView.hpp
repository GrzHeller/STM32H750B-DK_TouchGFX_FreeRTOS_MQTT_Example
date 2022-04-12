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

    // functions called indirectly from Model.cpp
    virtual void updateImage(uint8_t data);
    virtual void updateStatus(uint8_t connection_status);

    // functions called on button presses
    virtual void functionConnect();
    virtual void functionDisconnect();

protected:
    uint8_t MQTTConnectionStatus;
};

#endif // SCREENMAINVIEW_HPP
