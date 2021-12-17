#include <gui/screenmain_screen/ScreenMainView.hpp>

extern "C" {
#include "cmsis_os2.h"
extern osMessageQueueId_t queueSubscriptionStatusHandle;
}

ScreenMainView::ScreenMainView() : MQTTConnectionStatus(0) {

}

void ScreenMainView::setupScreen() {
	ScreenMainViewBase::setupScreen();
}

void ScreenMainView::tearDownScreen() {
	ScreenMainViewBase::tearDownScreen();
}

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

void ScreenMainView::functionSubscribe() {
	MQTTConnectionStatus = 1;
	osMessageQueuePut(queueSubscriptionStatusHandle, &MQTTConnectionStatus, 0,
			0);
}

void ScreenMainView::functionUnsubscribe() {
	MQTTConnectionStatus = 0;
	osMessageQueuePut(queueSubscriptionStatusHandle, &MQTTConnectionStatus, 0,
			0);
}

void ScreenMainView::updateStatus(uint8_t connection_status) {
	if (connection_status == 1) {
		Unicode::strncpy(textAreaBuffer, "yes", 3+1);
	} else {
		Unicode::strncpy(textAreaBuffer, "no", 2+1);
	}
	textArea.invalidate();
}
