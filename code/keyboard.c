#include "keyboard.h"
#include "eeprom.h"
#include "password.h"
#include "random.h"

#include "blinkmode.h"

static uint8_t random;
static uint8_t state;
static uint8_t blink_state;
static uint8_t button;

#define BTN1_SHORT 0
#define BTN1_LONG 1

uint8_t send_key_ready;

enum states {
  STATE_DEFAULT,
  STATE_SENDKEY,
  STATE_RECONFIG, 
};

#define RED_LED PD1
#define GREEN_LED PD0
#define YELLOW_LED PB5
#define BLUE_LED PB4

// set and clear functions for leds
static void set_red(void)
{
  PORTD |= _BV(RED_LED);
}

static void set_green(void)
{
  PORTD |= _BV(GREEN_LED);
}

static void set_blue(void)
{
  PORTB |= _BV(BLUE_LED);
}

static void set_yellow(void)
{
  PORTB |= _BV(YELLOW_LED);
}

static void clr_red(void)
{
  PORTD &= ~_BV(RED_LED);
}

static void clr_green(void)
{
  PORTD &= ~_BV(GREEN_LED);
}

static void clr_blue(void)
{
  PORTB &= ~_BV(BLUE_LED);
}

static void clr_yellow(void)
{
  PORTB &= ~_BV(YELLOW_LED);
}


/** Indicates what report mode the host has requested, true for normal HID reporting mode, \c false for special boot
 *  protocol reporting mode.
 */
static bool UsingReportProtocol = true;

/** Current Idle period. This is set by the host via a Set Idle HID class request to silence the device's reports
 *  for either the entire idle duration, or until the report status changes (e.g. the user presses a key).
 */
static uint16_t IdleCount = 500;

/** Current Idle period remaining. When the IdleCount value is set, this tracks the remaining number of idle
 *  milliseconds. This is separate to the IdleCount timer and is incremented and compared as the host may request
 *  the current idle period via a Get Idle HID class request, thus its value must be preserved.
 */
static uint16_t IdleMSRemaining = 0;

uint8_t pattern0[] = {0x0F};
struct Blinkmode bm0 = {
  50,
  sizeof(pattern0),
  pattern0
};

uint8_t pattern[] = {0x01,0x02};
struct Blinkmode bm1 = {
  50,
  sizeof(pattern),
  pattern
};

uint8_t pattern2[] = {0x01,0x03,0x07,0x0F,0x0E,0x0C,0x08,0x00};
struct Blinkmode bm2 = {
  10,
  sizeof(pattern2),
  pattern2
};

uint8_t pattern_fast_all[] = {0x0F,0x00};
struct Blinkmode bm_fast_all = {
  5,
  sizeof(pattern_fast_all),
  pattern_fast_all
};

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{

  clock_prescale_set(clock_div_1);

  // setup ports
  DDRD |= _BV(RED_LED) | _BV(GREEN_LED);
  DDRB |= _BV(BLUE_LED) | _BV(YELLOW_LED);
  
  blinkmode_setset(&set_red,0);
  blinkmode_setclr(&clr_red,0);

  blinkmode_setset(&set_green,1);
  blinkmode_setclr(&clr_green,1);

  blinkmode_setset(&set_blue,2);
  blinkmode_setclr(&clr_blue,2);

  blinkmode_setset(&set_yellow,3);
  blinkmode_setclr(&clr_yellow,3);

    // default state
  state = STATE_DEFAULT; 
  blinkmode_set_pattern(&bm1);
  
  USB_Init();
  
  GlobalInterruptEnable();

  PORTD |= _BV(PD4); // pull-up button

  // Setup Timer
  // prescaler
  //TCCR0B |= _BV(CS02) | _BV(CS00); // 1024
  TCCR0B |= _BV(CS02); // 256
  // overflow interrupt, every (256=4ms) (1024=32.768ms)
  TIMSK0 |= _BV(TOIE0);

  // password storage
  password_init(99);
  uint8_t * pp = password_getpointer();
  eeprom_read(pp);
  
  // random init
  // timer on, no prescaler
  TCCR1B |= _BV(CS10);
  // get random values from lower part of timer
  random_init(&TCNT1L);
  // falling edge interrupt on int5
  EICRB |= _BV(ISC51);
  // enable int5 interrupt
  EIMSK |= _BV(INT5);
  
  for (;;)
    {
      HID_Task();
      USB_USBTask();
    }
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
  /* Default to report protocol on connect */
  UsingReportProtocol = true;
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs.
 */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host sets the current configuration
 *  of the USB device after enumeration, and configures the keyboard device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
  bool ConfigSuccess = true;

  /* Setup HID Report Endpoints */
  ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_IN_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);
  ConfigSuccess &= Endpoint_ConfigureEndpoint(KEYBOARD_OUT_EPADDR, EP_TYPE_INTERRUPT, KEYBOARD_EPSIZE, 1);

  /* Turn on Start-of-Frame events for tracking HID report period expiry */
  USB_Device_EnableSOFEvents();

}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
  /* Handle HID Class specific requests */
  switch (USB_ControlRequest.bRequest)
    {
    case HID_REQ_GetReport:
      if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
	{
	  USB_KeyboardReport_Data_t KeyboardReportData;

	  /* Create the next keyboard report for transmission to the host */
	  CreateKeyboardReport(&KeyboardReportData);

	  Endpoint_ClearSETUP();

	  /* Write the report data to the control endpoint */
	  Endpoint_Write_Control_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData));
	  Endpoint_ClearOUT();
	}

      break;
    case HID_REQ_SetReport:
      if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
	{
	  Endpoint_ClearSETUP();

	  /* Wait until the LED report has been sent by the host */
	  while (!(Endpoint_IsOUTReceived()))
	    {
	      if (USB_DeviceState == DEVICE_STATE_Unattached)
		return;
	    }

	  /* Read in the LED report from the host */
	  //uint8_t LEDStatus = Endpoint_Read_8();

	  Endpoint_ClearOUT();
	  Endpoint_ClearStatusStage();

	}

      break;
    case HID_REQ_GetProtocol:
      if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
	{
	  Endpoint_ClearSETUP();

	  /* Write the current protocol flag to the host */
	  Endpoint_Write_8(UsingReportProtocol);

	  Endpoint_ClearIN();
	  Endpoint_ClearStatusStage();
	}

      break;
    case HID_REQ_SetProtocol:
      if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
	{
	  Endpoint_ClearSETUP();
	  Endpoint_ClearStatusStage();

	  /* Set or clear the flag depending on what the host indicates that the current Protocol should be */
	  UsingReportProtocol = (USB_ControlRequest.wValue != 0);
	}

      break;
    case HID_REQ_SetIdle:
      if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
	{
	  Endpoint_ClearSETUP();
	  Endpoint_ClearStatusStage();

	  /* Get idle period in MSB, IdleCount must be multiplied by 4 to get number of milliseconds */
	  IdleCount = ((USB_ControlRequest.wValue & 0xFF00) >> 6);
	}

      break;
    case HID_REQ_GetIdle:
      if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
	{
	  Endpoint_ClearSETUP();

	  /* Write the current idle duration to the host, must be divided by 4 before sent to host */
	  Endpoint_Write_8(IdleCount >> 2);

	  Endpoint_ClearIN();
	  Endpoint_ClearStatusStage();
	}

      break;
    }
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
  /* One millisecond has elapsed, decrement the idle time remaining counter if it has not already elapsed */
  if (IdleMSRemaining)
    IdleMSRemaining--;
}

/** Fills the given HID report data structure with the next HID report to send to the host.
 *
 *  \param[out] ReportData  Pointer to a HID report data structure to be filled
 */
void CreateKeyboardReport(USB_KeyboardReport_Data_t* const ReportData)
{
  uint8_t UsedKeyCodes = 0;
  
  /* Clear the report contents */
  memset(ReportData, 0, sizeof(USB_KeyboardReport_Data_t));

  if (state == STATE_SENDKEY && send_key_ready == 0)
    {
      // 2 equals 30ms
      send_key_ready = 2;
      
      uint8_t c = password_getchar();
      
      if (c)
	{
	  uint8_t sc = scancode(c);

	  if ( sc & 0x80)
	    {
	      ReportData->Modifier = HID_KEYBOARD_MODIFIER_LEFTSHIFT;
	      sc &= ~0x80;
	    }
	  
	  ReportData->KeyCode[UsedKeyCodes++] = sc;
	  //ReportData->KeyCode[UsedKeyCodes++] = scancode(random_get(25)+97);
	  
	}
      else
	{
	  state = STATE_DEFAULT;
	}
    }
}

/** Sends the next HID report to the host, via the keyboard data endpoint. */
void SendNextReport(void)
{
  static USB_KeyboardReport_Data_t PrevKeyboardReportData;
  USB_KeyboardReport_Data_t        KeyboardReportData;
  bool                             SendReport = false;

  /* Create the next keyboard report for transmission to the host */
  CreateKeyboardReport(&KeyboardReportData);

  /* Check if the idle period is set and has elapsed */
  if (IdleCount && (!(IdleMSRemaining)))
    {
      /* Reset the idle time remaining counter */
      IdleMSRemaining = IdleCount;

      /* Idle period is set and has elapsed, must send a report to the host */
      SendReport = true;
    }
  else
    {
      /* Check to see if the report data has changed - if so a report MUST be sent */
      SendReport = (memcmp(&PrevKeyboardReportData, &KeyboardReportData, sizeof(USB_KeyboardReport_Data_t)) != 0);
    }

  /* Select the Keyboard Report Endpoint */
  Endpoint_SelectEndpoint(KEYBOARD_IN_EPADDR);

  /* Check if Keyboard Endpoint Ready for Read/Write and if we should send a new report */
  if (Endpoint_IsReadWriteAllowed() && SendReport)
    {
      /* Save the current report data for later comparison to check for changes */
      PrevKeyboardReportData = KeyboardReportData;

      /* Write Keyboard Report Data */
      Endpoint_Write_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData), NULL);

      /* Finalize the stream transfer to send the last packet */
      Endpoint_ClearIN();
    }
}


/** Function to manage HID report generation and transmission to the host, when in report mode. */
void HID_Task(void)
{
  /* Device must be connected and configured for the task to run */
  if (USB_DeviceState != DEVICE_STATE_Configured)
    {
      return;
    }
  
  /* Send the next keypress report to the host */
  SendNextReport();
}

ISR(INT5_vect)
{
  // sample timer to get a random number
  random = random_get(95);
  // reset timer to avoid collision with other timer interrupts. 
  TCNT1 = 0; 
}

ISR(TIMER0_OVF_vect)
{
  // counter for led blinks
  //static uint8_t counter;
  static uint8_t button_counter; 

  if (send_key_ready)
    {
      send_key_ready--;
    }

  blinkmode_tick();

  // set blinkstate
  if (state != blink_state)
    {
      switch(state)
	{
	case STATE_DEFAULT:
	  blinkmode_set_pattern(&bm1);
	  break;
	case STATE_RECONFIG:
	  blinkmode_set_pattern(&bm2);
	  break;
	default:
	  break;
	}
      blink_state = state;
    }

  // if button is up
  if (PIND & _BV(PD4))
    {
      // button was down previously
      if (button & _BV(BTN1_LONG))
	{
	  //if (state == STATE_RECONFIG)
	  //{

	  //}
	}
      else if (button & _BV(BTN1_SHORT))
	{
	  blinkmode_set_temp(&bm_fast_all,2);
	  
	  if (state == STATE_DEFAULT)
	    {
	      state = STATE_SENDKEY;
	      password_begin();
	    }
	  else if (state == STATE_RECONFIG)
	    {
	      password_add(random+32);
	    }
	}
      // reset flag
      button = 0;
      // reset counter
      button_counter = 0; 
    }
  // if button is down
  else
    {
      // count
      if (button_counter > 0)
	{
	  // button short press flag
	  button |= _BV(BTN1_SHORT);

	  // count 2 seconds
	  if (button_counter > 240)
	    {
	      // long press detected switch state
	      if (!(button & _BV(BTN1_LONG)))
		{
		  // button long press flag
		  button |= _BV(BTN1_LONG);

		  if (state != STATE_RECONFIG)
		    {
		      state = STATE_RECONFIG;
		      password_init(100);
		    }
		  else
		    {
		      uint8_t * pp = password_getpointer();
		      eeprom_write(pp);

		      state = STATE_DEFAULT;
		    } 
		}
	    }
	  else
	    {
	      button_counter++;
	    }
	}
      else
	{
	  button_counter++;
	}
    }
}
