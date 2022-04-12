/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MQTT_Interface.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GUITask */
osThreadId_t GUITaskHandle;
const osThreadAttr_t GUITask_attributes = {
  .name = "GUITask",
  .stack_size = 8192 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for videoTask */
osThreadId_t videoTaskHandle;
const osThreadAttr_t videoTask_attributes = {
  .name = "videoTask",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for queueZeroOne */
osMessageQueueId_t queueZeroOneHandle;
const osMessageQueueAttr_t queueZeroOne_attributes = {
  .name = "queueZeroOne"
};
/* Definitions for queueConnectionStatus */
osMessageQueueId_t queueConnectionStatusHandle;
const osMessageQueueAttr_t queueConnectionStatus_attributes = {
  .name = "queueConnectionStatus"
};
/* Definitions for queueSubscriptionStatus */
osMessageQueueId_t queueSubscriptionStatusHandle;
const osMessageQueueAttr_t queueSubscriptionStatus_attributes = {
  .name = "queueSubscriptionStatus"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern portBASE_TYPE IdleTaskHook(void *p);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void TouchGFX_Task(void *argument);
extern void videoTaskFunc(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook(void) {
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	 to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
	 task. It is essential that code added to this hook function never attempts
	 to block in any way (for example, call xQueueReceive() with a block time
	 specified, or call vTaskDelay()). If the application makes use of the
	 vTaskDelete() API function (as this demo application does) then it is also
	 important that vApplicationIdleHook() is permitted to return to its calling
	 function, because it is the responsibility of the idle task to clean up
	 memory allocated by the kernel to any task that has since been deleted. */

	vTaskSetApplicationTaskTag(NULL, IdleTaskHook);
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of queueZeroOne */
  queueZeroOneHandle = osMessageQueueNew (2, sizeof(uint8_t), &queueZeroOne_attributes);

  /* creation of queueConnectionStatus */
  queueConnectionStatusHandle = osMessageQueueNew (2, sizeof(uint8_t), &queueConnectionStatus_attributes);

  /* creation of queueSubscriptionStatus */
  queueSubscriptionStatusHandle = osMessageQueueNew (2, sizeof(uint8_t), &queueSubscriptionStatus_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of GUITask */
  GUITaskHandle = osThreadNew(TouchGFX_Task, NULL, &GUITask_attributes);

  /* creation of videoTask */
  videoTaskHandle = osThreadNew(videoTaskFunc, NULL, &videoTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
	osDelay(100);
	uint8_t data = 5;
	uint8_t connection_status = 0;
	uint8_t status = 0;
	TickType_t pingTick = osKernelGetTickCount();
	mqtt_client_t *client = mqtt_client_new();
	struct mqtt_connect_client_info_t ci;
	memset(&ci, 0, sizeof(ci));
	ci.client_id = "H750B-DK";
	ci.keep_alive = 4; // for some reason this doesn't work with keep alive = 0, this might be an issue with my Mosquitto configuration though
	/* Infinite loop */
	for (;;) {
		osMessageQueueGet(queueSubscriptionStatusHandle, &status, NULL, 0); // receive which button is pressed
		connection_status = mqtt_client_is_connected(client); // check if we are connected
		if (client != NULL && connection_status == 0 && status == 1) {
			interface_mqtt_connect(client, &ci, &data); // attempt connection
		}
		if (status == 0) {
			mqtt_disconnect(client); // disconnect if button pressed
		}
		if (data != 5) {
			osMessageQueuePut(queueZeroOneHandle, &data, 0, 0); // if data differs from the default value 5, put it into queue
			data = 5;
		}

		if (pingTick + ci.keep_alive * 1000 < osKernelGetTickCount()) {
			// here we "ping" TouchGFX with our connection status
			// se we can change Subscribe: no/yes
			osMessageQueuePut(queueConnectionStatusHandle, &connection_status, 0, 0);
			pingTick = osKernelGetTickCount();
		}
		osDelay(10);
	}
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

