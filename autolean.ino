#include <ProTrinketKeyboard.h>  // Ensure the library is installed
#include <stdint.h>

static const uint8_t lean_left  = KEYCODE_Q;
static const uint8_t lean_right = KEYCODE_E;
static const uint8_t unlean     = 0;
static const int x_pin = 1;
static const int y_pin = 2;
static const int z_pin = 3;
static const int led = 13;
static const int data_size = 5;

uint8_t current_key = unlean;
uint32_t t = 0;
uint32_t b = 0;
int z[5];
int index = 0;

void setup()
{
  TrinketKeyboard.begin();
  pinMode(led, OUTPUT);
  t = millis();
  b = millis();
}

void loop()
{
  // Must call every 10ms
  TrinketKeyboard.poll();
  if (millis() > b)
  {
    b += 500;
    digitalWrite(led, !digitalRead(led));
  }
  if (millis() > t)
  {
    t += 10;
    z[index] = (analogRead(z_pin) - 357);
    index = (index == data_size - 1) ? 0 : index + 1;
    int avg = 0;
    for (int i = 0;i < data_size;i++)
    {
      avg += z[i];
    }
    avg /= data_size;
    switch (current_key)
    {
      case unlean:
      {
        if (avg > 10)
        {
          TrinketKeyboard.pressKey(0, lean_left);
          current_key = lean_left;
        }
        else if (avg < -10)
        {
          TrinketKeyboard.pressKey(0, lean_right);    
          current_key = lean_right;
        }
        break;
      }
      case lean_left:
      {
        if (avg <= 10)
        {
          TrinketKeyboard.pressKey(0, unlean);    
          current_key = unlean;
        }
        break;
      }
      case lean_right:
      {
        if (avg >= -10)
        {
          TrinketKeyboard.pressKey(0, unlean);    
          current_key = unlean;
        }
        break;
      }
    }
  }
}
