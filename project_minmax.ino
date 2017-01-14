
/* Buzzer notes */
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
/* Buzzer notes */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <dewpoint.h>
#include <dht11.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>
#include <EEPROM.h>
#include <Servo.h>

#define enablePartialUpdate 1

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 7 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 7, 8);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

static const unsigned char PROGMEM temp [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x07, 0xE1, 0xF8, 0x06, 0x63, 0x00, 0x06,
  0x66, 0x00, 0x06, 0x66, 0x00, 0x06, 0x66, 0x00, 0x06, 0x66, 0x00, 0x06, 0x63, 0x00, 0x06, 0x61,
  0xF8, 0x06, 0x60, 0x00, 0x07, 0xE0, 0x00, 0x0F, 0xF0, 0x00, 0x1F, 0xF8, 0x00, 0x1F, 0xF8, 0x00,
  0x3F, 0xFC, 0x00, 0x3F, 0xFC, 0x00, 0x3F, 0xFC, 0x00, 0x3F, 0xFC, 0x00, 0x1F, 0xF8, 0x00, 0x1F,
  0xF8, 0x00, 0x0F, 0xF0, 0x00, 0x03, 0xC0, 0x00, 
};

static const unsigned char PROGMEM hum [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x03, 0x83, 0x10, 0x03, 0x83, 0x20, 0x07,
  0xC0, 0x40, 0x07, 0xC0, 0x40, 0x0F, 0xE0, 0x98, 0x0F, 0xE1, 0x18, 0x1F, 0xF2, 0x00, 0x37, 0xF8,
  0x00, 0x67, 0xFC, 0x00, 0x6F, 0xFC, 0x00, 0xCF, 0xFE, 0x00, 0xDF, 0xFE, 0x00, 0xFF, 0xFE, 0x00,
  0xFF, 0xFE, 0x00, 0xFF, 0xFE, 0x00, 0x7F, 0xFC, 0x00, 0x7F, 0xFC, 0x00, 0x3F, 0xF8, 0x00, 0x1F,
  0xF0, 0x00, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 
};

static const unsigned char PROGMEM alarm [] = {
  0x63, 0x5D, 0x22, 0x49, 0x49, 0x45, 0x22, 0x1C, 
};

static const unsigned int PROGMEM melody [] = {
  NOTE_C3, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C3, 0, 0
};

static const unsigned int PROGMEM duration [] = {
  250, 125, 125, 250, 250, 250, 250, 250, 250, 250
};

static const int PROGMEM melody_length = ( sizeof(melody) / sizeof(melody[0]) );

#define SWITCH_DEBOUNCE_TIME       50 // 100 milliseconds

#define DHT11PIN                   4
#define BUZZERPIN                  5
#define BACKLIGHTPIN               6
#define SWITCH1PIN                 2
#define SWITCH2PIN                 3
#define SERVOPIN                   A3
#define LDRPIN                     A2

#define SERVO_MIN_VAL              1400
#define SERVO_MAX_VAL              1700

#define SWITCH1INTERRUPT           0
#define SWITCH2INTERRUPT           1

#define CONTRAST                   50
#define BRIGHTNESS                 180
#define BRIGHTNESS_DECAY           5

#define MIN_LDR_VALUE              100
#define MIN_BRIGHTNESS             40

#define DATA_INTERVAL                     120000 // 120 seconds
#define TIME_INTERVAL                     1000   // 1 second
#define LONGPRESS1_INTERVAL_SHOWTIME      2000   // 2 seconds
#define LONGPRESS1_INTERVAL_SETALARM      500    // 0.5 second
#define LONGPRESS1_INTERVAL_SHOWALARM     5000   // 5 seconds
#define LONGPRESS2_INTERVAL_SHOWTIME      1500   // 1.5 seconds
#define LONGPRESS2_INTERVAL_SETALARM      1000   // 1 second
#define BACKLIGHT_INTERVAL                3000   // 3 seconds
#define BACKLIGHT_DECAY_INTERVAL          50     // 0.05 seconds
#define SETALARM_INTERVAL                 5000   // 5 seconds
#define SETALARM_HELPER_INTERVAL          800    // 0.8 second
#define SHOWALARM_INTERVAL                1500   // 1.5 seconds
#define PENDULUM_INTERVAL                 500    // 0.5 second
#define LDR_DELAY                         3000  // 3 seconds

typedef enum _states
{
  STOPPED =            -1,
  SHOW_TIME =          0,
  SET_ALARM =          1,
  SHOW_ALARM =         2,
  PLAY_ALARM =         3,
  STATEGUARD =         4
} state_t;

typedef enum _setalarm_choices
{
  NIL =                -1,
  HOUR =               0,
  MIN =                1,
  MER =                2,
  DAY =                3,
  CHOICEGUARD =        4
} choice_t;

// TODO: create a class instead and use OOP
struct alarm_t
{
  bool is_set;
  int h;
  int m;
  bool mer; // pm is true, am is false
  int day; // sun - mon , 1 - 7. evryday is 0
  AlarmID_t alarm;
};

// ID of the settings block
#define CONFIG_VERSION "mnmx1"

// Tell it where to store your config data in EEPROM
#define CONFIG_START 32

struct StoreStruct {
  int h, m, day;
  bool mer;
  bool is_set;
  char version_of_program[6];
} settings = {
  // The default values
  0, 0, 0,
  false,
  false,
  CONFIG_VERSION
};

bool loadConfig() {
  size_t size_of_settings = sizeof(settings);
  if ( EEPROM.read(CONFIG_START + size_of_settings - 1) == settings.version_of_program[5] && // this is '\0'
       EEPROM.read(CONFIG_START + size_of_settings - 2) == settings.version_of_program[4] &&
       EEPROM.read(CONFIG_START + size_of_settings - 3) == settings.version_of_program[3] &&
       EEPROM.read(CONFIG_START + size_of_settings - 4) == settings.version_of_program[2] &&
       EEPROM.read(CONFIG_START + size_of_settings - 5) == settings.version_of_program[1] &&
       EEPROM.read(CONFIG_START + size_of_settings - 6) == settings.version_of_program[0] )
  { // reads settings from EEPROM
    for ( unsigned int t=0; t < size_of_settings; t++ )
      *((char*)&settings + t) = EEPROM.read(CONFIG_START + t);
    Serial.println("settings found");
  }
  else 
  {
    // settings aren't valid! will overwrite with default settings
    saveConfig();
    return false;
  }
  return true;
}

bool saveConfig() {
  for (unsigned int t=0; t<sizeof(settings); t++)
  { // writes to EEPROM if data is different
    if ( EEPROM.read(CONFIG_START + t) != (*((char*)&settings) + t) )
    {
      EEPROM.write(CONFIG_START + t, *((char*)&settings + t));
      // and verifies the data
      if (EEPROM.read(CONFIG_START + t) != *((char*)&settings + t))
      {
        // error writing to EEPROM
        Serial.println("error savng");
        return false;
      }
    }
  }
  return true;
}

unsigned int min_brightness = 0;
unsigned long prev_ldr_poll = 0;
bool prev_ldr_update = false;
bool low_light_mode = false;

unsigned long int curr_time = 0;
unsigned long int last_data_update = 0;
unsigned long int last_time_update = 0;

unsigned long int longpress_start_time1 = 0;
unsigned long int longpress_start_time2 = 0;

unsigned long int longpress_interval1 = LONGPRESS1_INTERVAL_SHOWTIME;
unsigned long int longpress_interval2 = LONGPRESS2_INTERVAL_SHOWTIME;

bool longpress_timer1 = false;
bool longpress_timer2 = false;

unsigned long int backlight_start_time = 0;
bool backlight_timer = false;
unsigned short backlight_decay_step = 0;

inline void backlightOff()
{
  analogWrite(BACKLIGHTPIN, min_brightness);
}

inline void backlightOn()
{
  analogWrite(BACKLIGHTPIN, BRIGHTNESS);
}

inline void backlightOn(unsigned int brightness)
{
  analogWrite(BACKLIGHTPIN, brightness);
}

void backlightTimerStart()
{
  backlight_timer = true;
  backlight_start_time = curr_time;
  backlight_decay_step = 1;
}

void backlightTimerStop()
{
  backlight_timer = false;
}

unsigned long int showalarm_start_time = 0;

unsigned long int setalarm_start_time = 0;
bool alarm_page_changed = false;
//bool alarm_is_set = false;
//bool alarm_change_minute = true;

unsigned long int pendulum_start_time = 0;
bool servo_dir = false;

unsigned int playalarm_minbrightness_save = 0;
unsigned long int playalarm_start_time = 0;
bool playalarm_timer = false;
unsigned int playalarm_tone_idx = 0;
unsigned long int playalarm_tone_interval = 0;

volatile bool switch1_down = false;
volatile bool switch1_up = false;

volatile bool switch2_down = false;
volatile bool switch2_up = false;

bool switch1_is_pressed = false;
bool switch2_is_pressed = false;

bool setalarm_minute_helper = false;
unsigned long setalarm_helper_time = 0;

unsigned long last_interrupt1_time = 0;
unsigned long last_interrupt2_time = 0;

state_t curr_state = STOPPED;
state_t prev_state = STOPPED;

// covnert to class and have methods
alarm_t the_alarm = { false, // is_set
                      0,     // hour
                      0,     // minute
                      false, // meridiem = AM
                      0,     // day of week = everyday
                      dtINVALID_ALARM_ID }; // alarmID
choice_t alarm_change_item = NIL;

void showSplashscreen()
{
  display.display();
  delay(2000);
  display.clearDisplay();
}

void setAlarm()
{
  Alarm.free(the_alarm.alarm);

  if ( the_alarm.day == 0 )
  {
    the_alarm.alarm = 
        Alarm.alarmRepeat( the_alarm.h + (static_cast<int>(the_alarm.mer) * 12),
                the_alarm.m,
                0,
                play_alarm_tone );
  }
  else
  {
    the_alarm.alarm = 
        Alarm.alarmRepeat( static_cast<timeDayOfWeek_t>(the_alarm.day),
                the_alarm.h + (static_cast<int>(the_alarm.mer) * 12),
                the_alarm.m,
                0,
                play_alarm_tone );
  }
}

void cancelAlarm()
{
  Alarm.free(the_alarm.alarm);
}

void updateTime(int h, int m)
{
  char buf[16];
  bool pm = false;

  if (h > 11)
  {
    pm = !pm;
    h %= 12;
  }

  display.setTextSize(2);
  //display.setCursor(10, 4); // time at 10, 4
  display.fillRect(10, 4, display.width() - 10, 14, WHITE); // donot erase alarm bitmap

  if ( h < 10 )
  {
   sprintf(buf, "%1d:%02d", h, m);
   display.setCursor(20, 4); // time at 10, 4
  }
  else
  {
   sprintf(buf, "%2d:%02d", h, m);
   display.setCursor(12, 4); // time at 10, 4
  }

  display.print(buf);

  display.fillCircle(78, 6 + ((int)pm * 9), 2, BLACK); // am - pm marker at 76, 8 / 74, 12 dia = 3

  display.display();
}

void drawTimePage()
{
  display.clearDisplay();
  //display.setTextSize(1);
  //display.setCursor(0, 24); // lables at 0, 24
  //display.print("TEMP  HUM  PRE");
  display.drawRect(7, 22, display.width() - 14, 1, BLACK);
  display.drawBitmap(0, 24, temp, 24, 24, BLACK);
  display.drawBitmap(42, 24, hum, 24, 24, BLACK);
  display.display();
}

void drawSetAlarmPage(bool update_only)
{
  char buf[16];
  sprintf(buf, "%02d:%02d", the_alarm.h, the_alarm.m);

  if ( update_only )
  {
    // erase hour, min, day and meridiem values only

    // erase hour and min
    display.fillRect(0, 8, display.width(), 18, WHITE);

    // erase meridiem and day
    display.fillRect(0, 30, display.width(), 11, WHITE);

  }
  else
  {
    // page is being drawn for first time. erase everything
    display.clearDisplay();
  }

  // display alarm time
  display.setCursor(13, 10);
  display.setTextSize(2);
  display.print(buf);

  // display AM or PM
  display.setCursor(16, 32);
  display.setTextSize(1);
  if ( the_alarm.mer )
  {
    display.print("PM");
  }
  else
  {
    display.print("AM");
  }

  // display day of week
  display.setCursor(54, 32);
  display.setTextSize(1);
  switch ( the_alarm.day )
  {
    case 0:
      display.print("EVR"); // everyday
      break;

    case 1:
      display.print("SUN");
      break;

    case 2:
      display.print("MON");
      break;

    case 3:
      display.print("TUE");
      break;

    case 4:
      display.print("WED");
      break;

    case 5:
      display.print("THU");
      break;

    case 6:
      display.print("FRI");
      break;

    case 7:
      display.print("SAT");
      break;

    default:
      ;
  }
  
  display.display();
}

void drawShowAlarmPage()
{
  drawSetAlarmPage(false);
}

void drawSelection()
{
  // draw selection recangle on item to be changed
  switch ( alarm_change_item )
  {
    case HOUR:
      display.drawRoundRect(11, 8, 26, 18, 2, BLACK);
      break;

    case MIN:
      display.drawRoundRect(47, 8, 26, 18, 2, BLACK);
      break;

    case MER:
      display.drawRoundRect(14, 30, 15, 11, 2, BLACK);
      break;

    case DAY:
      display.drawRoundRect(52, 30, 21, 11, 2, BLACK);
      break;

    default:
      ;
  }

  display.display();
}

void drawAlarmNoti(bool enabled)
{
  if ( enabled )
  {
    display.drawBitmap(2, 4, alarm, 8, 8, BLACK);
  }
  else
  {
    display.fillRect(2, 4, 8, 8, WHITE);
  }
  display.display();
}

void updateData(int t, int h, float p)
{
  char buf[16];
  //char temp[5];
  //char hum[5];
  char pres[5];
  //dtostrf(t, 3, 1, temp);
  //dtostrf(h, 3, 1, hum);
  dtostrf(p, 3, 1, pres);
  sprintf(buf, " %d   %d  %s", t, h, pres);

  display.setTextSize(1);
  //display.setCursor(2, 36);  // data at 2, 36
  display.fillRect(0, 36, display.width(), 7, WHITE);
  display.setCursor(2, 36);  // data at 2, 36
  display.print(buf);
  display.display();
}

void updateData(int t, int h)
{
  display.setTextSize(1);

  display.fillRect(26, 36, 14, 7, WHITE);
  display.setCursor(26, 36);
  display.print(t);

  display.fillRect(68, 36, 14, 7, WHITE);
  display.setCursor(68, 36);
  display.print(h);

  display.display();
}

void checkdht11(dht11& DHT11)
{
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK:
      Serial.println("OK");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
}

void switch1_handler()
{
  unsigned long interrupt1_time = millis();
  if ( ( interrupt1_time - last_interrupt1_time ) >= SWITCH_DEBOUNCE_TIME )
  {
    if ( digitalRead(SWITCH1PIN) == LOW )
    {
      switch1_down = true;
    }
    else
    {
      switch1_up = true;
    }

    last_interrupt1_time = interrupt1_time;
  }
}

void switch2_handler()
{
  unsigned long interrupt2_time = millis();
  if ( ( interrupt2_time - last_interrupt2_time ) >= SWITCH_DEBOUNCE_TIME )
  {
    if ( digitalRead(SWITCH2PIN) == LOW )
    {
      switch2_down = true;
    }
    else
    {
      switch2_up = true;
    }

    last_interrupt2_time = interrupt2_time;
  }
}

inline void enable_handler1()
{
  attachInterrupt(SWITCH1INTERRUPT, switch1_handler, CHANGE);
}

inline void enable_handler2()
{
  attachInterrupt(SWITCH2INTERRUPT, switch2_handler, CHANGE);
}

inline void disable_handler1()
{
  detachInterrupt(SWITCH1INTERRUPT);
}

inline void disable_handler2()
{
  detachInterrupt(SWITCH2INTERRUPT);
}

dht11 DHT11;
Servo pendulum;

void setup()   {
  Serial.begin(9600);
  while (!Serial);

  unsigned long int start_time = millis(); // millisecond resolution
  last_data_update = start_time;
  last_time_update = start_time;

  // init backlight
  pinMode(BACKLIGHTPIN, OUTPUT);

  // init LDR
  pinMode(LDRPIN, INPUT);

  // inti buzzer
  pinMode(BUZZERPIN, OUTPUT);

  display.begin();
  display.setContrast(CONTRAST);
  display.setTextColor(BLACK);

  //setTime(22, 35, 0, 2, 6, 15);
  delay(500);
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");

  now();

  pendulum.attach(SERVOPIN, 600, 2400);
  pendulum.writeMicroseconds(1550);
  //pendulum.detach();

  // set up switch handlers
  pinMode(SWITCH1PIN, INPUT_PULLUP);
  pinMode(SWITCH2PIN, INPUT_PULLUP);

  enable_handler1();
  enable_handler2();

  showSplashscreen();
  checkdht11(DHT11);

  //drawTimePage();
  //updateData(DHT11.temperature, DHT11.humidity);

  //updateTime(hour(), minute());

  // load alarm settings if saved else load default
  // if alarn time hasn't passed set alarm
  loadConfig();
  the_alarm.h = settings.h;
  the_alarm.m = settings.m;
  the_alarm.day = settings.day;
  the_alarm.mer = settings.mer;
  the_alarm.is_set = settings.is_set;
  if ( the_alarm.is_set ) setAlarm();
  
  // init state machine
  prev_state = STOPPED;
  curr_state = SHOW_TIME;
}

void play_alarm_tone()
{
  //Serial.println("Alarm fired");
  updateTime(hour(), minute());
  pendulum.attach(SERVOPIN, 600, 2400);

  playalarm_tone_idx = 0;
  playalarm_timer = true;

  // save min brightness value
  playalarm_minbrightness_save = min_brightness;
  min_brightness = 0;
}

void change_alarm_item()
{
  switch ( alarm_change_item )
  {
    case HOUR:
      the_alarm.h = ( the_alarm.h + 1 ) % 12;
      break;

    case MIN:
      the_alarm.m = ( the_alarm.m + 1 ) % 60;
      break;

    case MER:
      the_alarm.mer = !the_alarm.mer;
      break;

    case DAY:
      the_alarm.day = ( the_alarm.day + 1) % 8;
      break;

    default:
      ;
  }

  alarm_page_changed = true;
}

void switch1_down_showtime()
{
  backlightTimerStop();
  backlightOn();
}

void switch1_down_setalarm()
{
  // this variable should be renamed as setalarm timeout start time
  setalarm_start_time = curr_time;
}

void switch1_down_showalarm()
{
}

void switch1_up_showtime()
{
  backlightTimerStart(); 
}

void switch1_up_setalarm()
{
  if ( switch1_is_pressed )
  {
    // clear minute increment helper flag
    if ( setalarm_minute_helper )
    {
      setalarm_minute_helper = false;
    }
    else
    {
      change_alarm_item();
    }
  }
}

void switch1_up_showalarm()
{
}

void switch2_down_showtime()
{
  // switch on backlight and set timer
  analogWrite(BACKLIGHTPIN, BRIGHTNESS);


  prev_state = curr_state;
  curr_state = SHOW_ALARM;
}

void switch2_down_setalarm()
{
  // renew alarm start time
  setalarm_start_time = curr_time;
}

void switch2_down_showalarm()
{
}

void switch2_up_showtime()
{
}

void switch2_up_setalarm()
{
  // this check is necessary as we need to filter 
  // siwtch up event from when we enter this state
  if ( switch2_is_pressed )
  {
    // cycle items to be changed
    alarm_change_item = static_cast<choice_t>( ( static_cast<int>(alarm_change_item) + 1 ) % ( static_cast<int>(DAY) + 1) );

    alarm_page_changed = true;
  }
}

void switch2_up_showalarm()
{
}

void setalarm_helper_func()
{
  Serial.println("setalarm_helper_func");
  // increment minute by 10
  if ( alarm_change_item == MIN )
  {
    the_alarm.m = ( the_alarm.m + 10 ) % 60;
    alarm_page_changed = true;
  }

  // this variable should be renamed as setalarm timeout start time
  setalarm_start_time = curr_time;

  // reset helper counter
  setalarm_helper_time = curr_time;
}

void stopAlarm()
{
    /***********************************************/
    // TODO code here should got to a case statement or somewhere else
    // here we shut off the alarm tone when user presses button.
    // naked calls to noTone and clearing timer explicitly is not gooda.
    if ( playalarm_timer )
    {
      noTone(BUZZERPIN);
      // erase alarm notification if one shot alarm
      if ( the_alarm.day != 0 )
      {
          the_alarm.is_set = false;
          drawAlarmNoti(the_alarm.is_set);

          // as alarm has been fired we neet to update settings
          settings.is_set = the_alarm.is_set;
          saveConfig();
      }

      pendulum.writeMicroseconds(1550);
      //pendulum.detach();

      playalarm_timer = false;

      // restore min brightness
      min_brightness = playalarm_minbrightness_save;

      // backlight
      backlightTimerStop();
      backlightOn(min_brightness);
    }
    /***********************************************/
}

void loop() {
  /** any temporary variables for testing go here **/

  /** any temporary variables for testing go here **/

  curr_time = millis();

  // check for ambient light and set minimum backlight brightness
  if ( ( ( curr_time - prev_ldr_poll ) >= ( TIME_INTERVAL + ( static_cast<int>(prev_ldr_update) * LDR_DELAY ) ) ) && ( !playalarm_timer ) )
  {
    // clear the last time lighting returned to normal
    if ( prev_ldr_update ) prev_ldr_update = !prev_ldr_update;

    // poll LDR 
    if ( map(analogRead(LDRPIN), 0, 1023, 255, 0) <= MIN_LDR_VALUE )
    {
      if ( !low_light_mode )
      {
        low_light_mode = !low_light_mode;
        min_brightness = MIN_BRIGHTNESS;

        if ( !backlight_timer ) backlightOn(min_brightness);
      }
    }
    else if ( low_light_mode )
    {
      low_light_mode = !low_light_mode;
      min_brightness = 0;

      if ( !backlight_timer ) backlightOff();

      // save if ambient light increased. a delay is used so that
      // backlight doesnot go crazy with changing lighting
      // just set the flag as it remains cleared everytime till here
      prev_ldr_update = !prev_ldr_update;
    }

    prev_ldr_poll = curr_time;
  }

  // use PLAY_ALARM state or maybe discard the state
  // and use async event as being used
  if ( playalarm_timer )
  {
      // flashing blacklight
      if ( !backlight_timer )
      {
        backlightOn();
        backlightTimerStart();
      }

      // servo pendulum
      if ( ( curr_time - pendulum_start_time ) >= PENDULUM_INTERVAL )
      {
        if ( servo_dir ) pendulum.writeMicroseconds(SERVO_MIN_VAL);
        else pendulum.writeMicroseconds(SERVO_MAX_VAL);
        servo_dir = !servo_dir;
        pendulum_start_time = curr_time;
      } 

    if ( ( ( curr_time - playalarm_start_time ) >= playalarm_tone_interval ) || ( playalarm_tone_idx == 0 ) )
    {
      // if last note has been played then now we should end playing the alarm
      if ( playalarm_tone_idx == melody_length )
      {
        playalarm_tone_idx = 0;
      }
      else
      {
        unsigned int tone_to_play = static_cast<unsigned int>( pgm_read_byte( melody + playalarm_tone_idx ) );
        unsigned int tone_duration = static_cast<unsigned int>( pgm_read_byte( duration + playalarm_tone_idx ) );

        tone( BUZZERPIN, tone_to_play, tone_duration );
    
        playalarm_tone_interval = ( tone_duration * 1.30 );
        playalarm_start_time = millis();

        // debug
        //Serial.print(playalarm_tone_idx);
        //Serial.print(" ");
        ////Serial.print(tone_duration);
        //Serial.print(" ");
        //Serial.print(playalarm_tone_interval);
        //Serial.println();

        playalarm_tone_idx++;
      }
    }
  }

  if ( backlight_timer )
  {
    //Serial.println("backlight timer");

    if ( ( curr_time - backlight_start_time ) >= (backlight_decay_step * BACKLIGHT_DECAY_INTERVAL) )
    {
      unsigned int brightness = ( BRIGHTNESS - ( backlight_decay_step * BRIGHTNESS_DECAY ) );

      analogWrite( BACKLIGHTPIN, brightness );
      backlight_decay_step++;

      if ( brightness <= min_brightness ) backlight_timer = false;
    }
  }

  if ( longpress_timer1 )
  {
    //Serial.println("longpress timer1");
    if ( ( curr_time - longpress_start_time1 ) >= longpress_interval1 )
    {
      //Serial.println("longpress timer trips");

      switch ( curr_state )
      {
        case SHOW_TIME:
          break;

        case SET_ALARM:
          // set is pressed mode
          setalarm_minute_helper = true;
          setalarm_helper_func();

          break;

        case SHOW_ALARM:
          // cancel the alarm
          cancelAlarm;
          the_alarm.is_set = false;

          tone(BUZZERPIN, NOTE_A3, 100);

          break;

        default:
          ;
      }

      // clear is pressed flag for switch2
      switch2_is_pressed = false;

      // clear timer
      longpress_timer1 = false;
    }
  }

  if ( longpress_timer2 )
  {
    //Serial.println("longpress timer1");
    if ( ( curr_time - longpress_start_time2 ) >= longpress_interval2 )
    {
      //Serial.println("longpress timer trips");

      switch ( curr_state )
      {
        case SHOW_TIME:
        case SHOW_ALARM:
          // set state variables
          prev_state = curr_state;
          curr_state = SET_ALARM;
          
          // set alarm mode selected
          tone(BUZZERPIN, NOTE_B7, 100);

          break;

        case SET_ALARM:

          // set alarm
          setAlarm();

          // alarm is set
          the_alarm.is_set = true;
          tone(BUZZERPIN, NOTE_B7, 100);

          // save alarm settings to EEPROM
          settings.h = the_alarm.h;
          settings.m = the_alarm.m;
          settings.day = the_alarm.day;
          settings.mer = the_alarm.mer;
          settings.is_set = the_alarm.is_set;
          saveConfig();

          // change state
          prev_state = curr_state;
          curr_state = SHOW_TIME; 

          backlightTimerStart();

          break;

        default:
          ;
      }

      // clear is pressed flag for switch2
      switch2_is_pressed = false;

      // clear timer
      longpress_timer2 = false;
    }
  }

  if ( setalarm_minute_helper )
  {
    Serial.println("setalarm_minute_helper");
    if ( ( curr_time - setalarm_helper_time ) >= SETALARM_HELPER_INTERVAL)
    {
      switch ( curr_state )
      {
        case SHOW_TIME:
          break;

        case SET_ALARM:
          setalarm_helper_func();
          break;

        default:
          ;
      }
    }
  }

  if ( switch1_down )
  {
    Serial.println("switch down");

    // start longpress timer
    longpress_start_time1 = curr_time;
    longpress_timer1 = true;

    switch1_is_pressed = true;

    stopAlarm();

    switch ( curr_state )
    {
      case SHOW_TIME:
        switch1_down_showtime();
        break;

      case SET_ALARM:
        switch1_down_setalarm();
        break;

      case SHOW_ALARM:
        break;

      default:
        ;
    }

    // clear event flag
    switch1_down = false;
  }


  if ( switch1_up )
  {
    Serial.println("switch1 up");

    // clear longpress timer
    if ( longpress_timer1 ) longpress_timer1 = false;

    switch ( curr_state )
    {
      case SHOW_TIME:
        switch1_up_showtime();
        break;

      case SET_ALARM:
        switch1_up_setalarm();
        break;

      case SHOW_ALARM:
        break;

      default:
        ;
    }

    if ( switch1_is_pressed ) switch1_is_pressed = false; 

    // clear event flag
    switch1_up = false;
  }

  if ( switch2_down )
  {
    Serial.println("switch2 down");
    // start longpress timer
    longpress_start_time2 = curr_time;
    longpress_timer2 = true;
    
    switch2_is_pressed = true;

    stopAlarm();

    switch ( curr_state )
    {
      case SHOW_TIME:
        switch2_down_showtime();
        break;

      case SET_ALARM:
        switch2_down_setalarm();
        break;

      case SHOW_ALARM:
        break;

      default:
        ;
    }

    // clear event
    switch2_down = false;
  }

  if ( switch2_up )
  {
    Serial.println("switch2 up");

    // clear longpress timer
    if ( longpress_timer2 ) longpress_timer2 = false;


    switch ( curr_state )
    {
      case SHOW_TIME:
        switch2_up_showtime();
        break;

      case SET_ALARM:
        switch2_up_setalarm();
        break;

      default:
        ;
    }

    if ( switch2_is_pressed ) switch2_is_pressed = false; 

    // clear event
    switch2_up = false;
  }

  // periodic tasks

  switch ( curr_state )
  {
    case SHOW_TIME:
      if ( prev_state != curr_state)
      {
        longpress_interval1 = LONGPRESS1_INTERVAL_SHOWTIME;
        longpress_interval2 = LONGPRESS2_INTERVAL_SHOWTIME;
        drawTimePage();
        updateData(DHT11.temperature, DHT11.humidity);
        updateTime(hour(), minute());
        if ( the_alarm.is_set )
        {
            drawAlarmNoti(the_alarm.is_set);
        }
        prev_state = curr_state;

        break;
      }

      if ( ( curr_time - last_data_update ) >=  DATA_INTERVAL )
      {
        DHT11.read(DHT11PIN);
        updateData(DHT11.temperature, DHT11.humidity);
        last_data_update = curr_time;
      }

      if ( ( curr_time - last_time_update ) >= TIME_INTERVAL )
      {
        updateTime(hour(), minute());
        last_time_update = curr_time;
      }

      break;

    case SET_ALARM:
      if ( prev_state != curr_state )
      {
        longpress_interval1 = LONGPRESS1_INTERVAL_SETALARM;
        longpress_interval2 = LONGPRESS2_INTERVAL_SETALARM;
        alarm_change_item = HOUR;
        // draw entire page
        drawSetAlarmPage(false);
        drawSelection();
        prev_state = curr_state;
        setalarm_start_time = curr_time;

        backlightTimerStop();
        backlightOn();
        break;
      }
      
      // check for timeout first.
      // if user presses button start time gets updated
      if ( ( curr_time - setalarm_start_time ) >= SETALARM_INTERVAL )
      {
        // alarm was not set
        tone(BUZZERPIN, NOTE_A3, 100);

        // change state
        prev_state = curr_state;
        curr_state = SHOW_TIME;

        backlightTimerStart();
      }

      if ( alarm_page_changed )
      {
        // only update
        drawSetAlarmPage(true);
        drawSelection();
        alarm_page_changed = false;
      }

      break;

    case SHOW_ALARM:
      if ( prev_state != curr_state )
      {
        //disable longpress-timer1
        if ( longpress_timer1 ) longpress_timer1 = false;
        longpress_interval1 = LONGPRESS1_INTERVAL_SHOWALARM;
        drawShowAlarmPage();

        showalarm_start_time = curr_time;
        prev_state = curr_state;

        backlightTimerStop();
        backlightOn();
      }

      if ( ( curr_time - showalarm_start_time ) >= SHOWALARM_INTERVAL )
      {
        if ( !( switch1_is_pressed || switch2_is_pressed ) )
        {
          prev_state = curr_state;
          curr_state = SHOW_TIME;

          backlightTimerStart();
        }
      }
      
      break;

    default:
      ;
  }

  // required for alarms
  Alarm.userServiceAlarms();

}



