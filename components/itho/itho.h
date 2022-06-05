#pragma once

#include "esphome/core/component.h"

#include <Arduino.h>
#include <Ticker.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "IthoSystem.h"
#include "IthoQueue.h"
#include "SystemConfig.h"

namespace esphome
{
  namespace itho
  {

#define WIFILED 17
#define STATUSPIN 16
#define ITHOSTATUS 13

#define STACK_SIZE 4096
#define TASK_MAIN_PRIO 5

    class Itho : public Component
    {
    public:
      Itho();
      ~Itho();

      static void TaskSysControl( void *pvParameter );

      void setup() override;
      void loop() override;
      void dump_config() override;
      float get_setup_priority() const { return setup_priority::DATA; }

      bool ithoI2CCommand(uint8_t remoteIndex, const std::string &command);

      bool setIthoSpeed(uint16_t speed);

      double getIthoTemperature() { return ithoSystem->getIthoTemperature(); }
      double getIthoHumidity() { return ithoSystem->getIthoHumidity(); }
      uint16_t getIthoSpeed() { return ithoSystem->getIthoSpeed(); }
      std::string getIthoFanInfo() { return ithoSystem->getIthoFanInfo(); }

      void setSysSHT30(uint8_t value) { systemConfig->setSysSHT30(value); }
      void setSysSHT30_Address(uint8_t value) { systemConfig->setSysSHT30_Address(value); }
      void setI2C_SDA_Pin(uint8_t value) { systemConfig->setI2C_SDA_Pin(value); }
      void setI2C_SCL_Pin(uint8_t value) { systemConfig->setI2C_SCL_Pin(value); }

    private:
      void execSystemControlTasks();
      bool ithoInitCheck();
      bool ithoExecCommand(const char *command);
      bool ithoI2CCommand(uint8_t remoteIndex, const char *command);
      bool loadVirtualRemotesConfig();

      SemaphoreHandle_t mutexI2Ctask;
      TaskHandle_t xTaskSysControlHandle = NULL;
      
      IthoRemote virtualRemotes;
      IthoSystem *ithoSystem;
      SystemConfig *systemConfig;
      IthoQueue *ithoQueue;

      bool IthoInit = false;
      bool i2cStartCommands = false;
      bool joinSend = false;
      unsigned long lastI2CinitRequest = 0;
      int8_t ithoInitResult = 0;
      unsigned long lastVersionCheck;
      unsigned long query2401time = 0;
      bool stateSCL;
      unsigned long lastSCLLowTime;
    };

  } // namespace itho
} // namespace esphome
