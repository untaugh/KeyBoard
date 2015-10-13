#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "Descriptors.h"
#include <LUFA/Drivers/USB/USB.h>

void HID_Task(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

void CreateKeyboardReport(USB_KeyboardReport_Data_t* const ReportData);
void SendNextReport(void);

#endif // KEYBOARD_H

