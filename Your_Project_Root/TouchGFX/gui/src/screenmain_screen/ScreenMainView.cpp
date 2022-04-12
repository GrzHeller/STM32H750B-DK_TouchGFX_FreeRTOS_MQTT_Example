#include <gui/screenmain_screen/ScreenMainView.hpp>

#ifndef SIMULATOR
extern "C" {
#include "cmsis_os2.h"
extern osMessageQueueId_t queueSubscriptionStatusHandle;
}
#endif

ScreenMainView::ScreenMainView() :
		MQTTConnectionStatus(0) {

}

void ScreenMainView::setupScreen() {
	ScreenMainViewBase::setupScreen();
}

void ScreenMainView::tearDownScreen() {
	ScreenMainViewBase::tearDownScreen();
}

// here we change the image displayed
void ScreenMainView::updateImage(uint8_t data) {
	if (data == '0') {
		ScreenMainViewBase::imageOne.setVisible(false);
		ScreenMainViewBase::imageZero.setVisible(true);
	} else if (data == '1') {
		ScreenMainViewBase::imageZero.setVisible(false);
		ScreenMainViewBase::imageOne.setVisible(true);
	}
	ScreenMainViewBase::imageZero.invalidate();
	ScreenMainViewBase::imageOne.invalidate();
}

// send connect request over queue
void ScreenMainView::functionConnect() {
#ifndef SIMULATOR
	MQTTConnectionStatus = 1;
	osMessageQueuePut(queueSubscriptionStatusHandle, &MQTTConnectionStatus, 0,
			0);
#endif
}

// send disconnect request
void ScreenMainView::functionDisconnect() {
#ifndef SIMULATOR
	MQTTConnectionStatus = 0;
	osMessageQueuePut(queueSubscriptionStatusHandle, &MQTTConnectionStatus, 0,
			0);
#endif
}

//update text
void ScreenMainView::updateStatus(uint8_t connection_status) {
	if (connection_status == 1) {
		Unicode::strncpy(textAreaBuffer, "yes\0", Unicode::strlen(textAreaBuffer) + 1);
	} else {
		Unicode::strncpy(textAreaBuffer, "no\0", Unicode::strlen(textAreaBuffer) + 1);
	}
	invalidate();
}
