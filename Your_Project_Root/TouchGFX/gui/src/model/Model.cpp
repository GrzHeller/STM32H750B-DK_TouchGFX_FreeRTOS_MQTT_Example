#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// we have to wrap every FreeRTOS related code in this #ifndef, otherwise TouchGFX simulator will be disabled
#ifndef SIMULATOR
extern "C" {
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "stdint.h"
extern osMessageQueueId_t queueZeroOneHandle;
extern osMessageQueueId_t queueConnectionStatusHandle;
}
#endif

Model::Model() :
		modelListener(0) {

}

void Model::tick() {
#ifndef SIMULATOR
	uint8_t data;
	uint8_t connection_status;
	if (osMessageQueueGet(queueZeroOneHandle, &data, NULL, 0) == osOK) {
		// here we change image depending on data
		modelListener->setImage(data);
	}
	if (osMessageQueueGet(queueConnectionStatusHandle, &connection_status, NULL, 0) == osOK) {
		// here we change the text
		modelListener->setStatus(connection_status);
	}
#endif
}
