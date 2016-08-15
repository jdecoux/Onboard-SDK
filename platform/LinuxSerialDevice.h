/*! @file LinuxSerialDevice.h
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Serial device hardware implementation for Linux machines.
 *  This is a generic Linux serial device implementation along with basic memory lock implementations.
 *
 *  Use this in your own Linux-based DJI Onboard SDK implementations.  
 *
 *  @copyright
 *  2016 DJI. All rights reserved.
 * */

#ifndef LINUXSERIALDEVICE_H
#define LINUXSERIALDEVICE_H


#include <stdio.h>
#include <string>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <algorithm>
#include <iterator>

#include <DJI_Type.h>
#include <DJI_HardDriver.h>
#include <DJI_API.h>

namespace DJI {

namespace onboardSDK {

class LinuxSerialDevice : public HardDriver {

  public:
    LinuxSerialDevice(std::string device, unsigned int baudrate);
    ~LinuxSerialDevice();

    void init();
    void init(CoreAPI* api);

    bool getDeviceStatus();
    bool validateConnection(CoreAPI* api);

    void setBaudrate(unsigned int baudrate);
    int checkBaudRate(uint8_t (&buf)[BUFFER_SIZE]);
    void setDevice(std::string device);

    //! Start of HardDriver virtual function implementations
    size_t send(const uint8_t *buf, size_t len);
    size_t readall(uint8_t *buf, size_t maxlen);

    void lockMemory();
    void freeMemory();
    void lockMSG();
    void freeMSG();
    void lockACK();
    void freeACK();
    void notify();

    void wait(int timeoutInSeconds);
    //! End of HardDriver virtual function implementations

    //! Implemented here because ..
    DJI::time_ms getTimeStamp();

  private:
    std::string m_device;
    unsigned int m_baudrate;
    pthread_mutex_t m_memLock;
    pthread_mutex_t m_msgLock;
    pthread_mutex_t m_ackLock;
    pthread_cond_t m_ackRecvCv;

    int m_serial_fd;
    fd_set m_serial_fd_set;
    bool deviceStatus;

    bool _serialOpen(const char* dev);
    bool _serialClose();
    bool _serialFlush();
    bool _serialConfig(int baudrate, char data_bits, char parity_bits, char stop_bits, bool testForData = false);

    int _serialStart(const char *dev_name, int baud_rate);
    int _serialWrite(const unsigned char *buf, int len);
    int _serialRead(unsigned char *buf, int len);

};

}

}


#endif //LINUXSERIALDEVICE_H