
#include <iostream>

#define MAX_TIMERS 3

class Timer;

class Timer
{
public:
  static Timer *timers[MAX_TIMERS];
  static Timer* getFreeTimer();
  static void updateTimers();

private:
  unsigned long interval;
  unsigned long start;
  bool repeat;
  void (*timerCallback)(void);

  inline unsigned long elapsed()
  {
    return millis();
  }

public:
  Timer(): interval(0),
           start(0),
           repeat(false),
           timerCallback(NULL) { };

  ~Timer() { };

  void set(unsigned long interval, bool repeat, void (*CB)(void))
  {
    this->interval = interval;
    this->repeat = repeat;
    this->timerCallback = CB;
    this->start = elapsed();
  }

  void changeInterval(unsigned long interval)
  {
    this->interval = interval;
  }

  void clear()
  {
    this->timerCallback = NULL;
  }

  void update()
  {
    if ( !timerCallback ) return;
    void (*CB)(void) = NULL;
    if ( ( elapsed() - this->start ) >= this->interval )
    {
      CB = timerCallback;

      if ( repeat ) this->start = elapsed();
      else this->timerCallback = NULL;

      if (CB) (*CB)();
    }
  }
};

Timer::timers[] = { 0, };

Timer* Timer::getFreeTimer()
{
  for (int i = 0; i < MAX_TIMERS; ++i)
  {
    if ( timers[i] && !timer[i]->timerCallback)
      return timers[i]; 
  }
  return NULL;
}

void Timer::updateTimers()
{
  for (int i = 0; i < MAX_TIMERS; ++i)
  {
    timers[i]->update();
  }
}

class Backlight
{
  Timer *blTimer;
  bool isOn;
  uint8_t pin;
  const uint8_t brightness = 180;
  const int timeout = 3000;
public:
  Backlight(): blTimer(NULL), isOn(false), pin(BACKLIGHTPIN),
  {
    pinMode(pin, OUTPUT);
  }

  ~Backlight() { };

  bool setTimer()
  {
    Timer* t = Timer::getFreeTimer();
    if (!t) return false;
    t->set(timeout, false, off);
    blTimer = t;
    return true;
  }

  inline void on()
  {
    if (isOn) return;

    analogWrite(pin, brightness);
    isOn = true;
  }

  inline void off()
  {
    if (!isOn) return;

    digitalWrite(pin, 0);
    isOn = false;
  }
}

class Buzzer
{
  Timer *buzTimer;
  uint8_t pin;
  bool isOn;
  unsigned int* melody;
  unsigned int currToneIdx;

public:
  Buzzer(): buzTimer(NULL), pin(BUZZERPIN), isOn(false), melody(NULL), currToneIdx(0)
  {
    pinMode(pin, OUTPUT);
  }

 ~Buzzer() { };

  bool start(unsigned int *melody, unsigned int *duration)
  {
    currToneIdx = 0;
    Timer *t = Timer::getFreeTimer();
    if (!t) return false;

    t->set( ( duration[currToneIdx] * 1.30 ), true, play );
    buzTimer = t;
    return true;
  }

  void play()
  {
    
  }
}

class Alarm
{
  AlarmID_t alarmID;
  Servo s;
  uint8_t buzzer;
}

int main()
{
  Timer t;
  return 0;
}
