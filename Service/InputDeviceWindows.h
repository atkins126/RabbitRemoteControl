#ifndef CINPUTDEVICEWINDOWS_H
#define CINPUTDEVICEWINDOWS_H

#include "InputDevice.h"

class CInputDeviceWindows : public CInputDevice
{
public:
    explicit CInputDeviceWindows();
    virtual ~CInputDeviceWindows();
    
    virtual int KeyEvent(quint32 keysym, quint32 keycode, bool down = true);
    virtual int MouseEvent(MouseButtons buttons, QPoint pos);
    virtual int MouseEvent(MouseButtons buttons, int x, int y);
};

#endif // CINPUTDEVICEWINDOWS_H
