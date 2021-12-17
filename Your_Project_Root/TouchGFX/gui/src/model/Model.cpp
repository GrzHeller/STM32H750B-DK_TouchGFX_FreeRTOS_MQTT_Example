#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C" {
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "stdint.h"
extern osMessageQueueId_t queueZeroOneHandle;
extern osMessageQueueId_t queueConnectionStatusHandle;
}

Model::Model() : modelListener(0) {

}

void Model::tick() {
	uint8_t data;
	uint8_t connection_status;
	if (osMessageQueueGet(queueZeroOneHandle, &data, NULL, 0) == osOK) {
		modelListener->setImage(data);
	}
	if (osMessageQueueGet(queueConnectionStatusHandle, &connection_status, NULL, 0) == osOK) {
		modelListener->setStatus(connection_status);
	}
}
