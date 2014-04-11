#ifndef BLUETOOTHPROCESS_H
#define BLUETOOTHPROCESS_H


#include <QThread>
#include <QImage>

#include "bluetoothprocess.h"
#include <sys/time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <bluetooth/bluetooth.h>
#include <cwiid.h>


class BluetoothProcess: public QThread
{
    Q_OBJECT
public:
    BluetoothProcess();
    void set_led_state(cwiid_wiimote_t *wiimote, unsigned char led_state);
    void set_rpt_mode(cwiid_wiimote_t *wiimote, unsigned char rpt_mode);
    void print_state(struct cwiid_state *state);
    void stopProcess();
signals:
    void sendData( int x, int y );
protected:
    void run();

private:
    volatile bool stopped;

};



#endif // BLUETOOTHPROCESS_H
