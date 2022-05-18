#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"
#include <Adafruit_SSD1306.h>

#define SD_ChipSelectPin 8
#define led 10
#define repeatBtn 4

uint8_t skipBtns[2] = {5, 6}; // back, forward

TMRpcm tmrpcm;
Adafruit_SSD1306 display = Adafruit_SSD1306();

uint8_t totalIndicies = 0;
uint8_t currentIndex = 1;

bool repeat = false;

void disp()
{
  if (currentIndex > totalIndicies)
    currentIndex = 1;
  else if (currentIndex == 0)
    currentIndex = totalIndicies;

  display.clearDisplay();
  display.setTextSize(0.999);
  display.setCursor(0, 0);
  display.print("Now Playing");
  if (repeat == true)
  {
    display.setTextSize(0.999);
    display.setCursor(85, 0);
    display.print("R");
    display.display();
  }
  display.setCursor(105, 0);
  display.print(String(currentIndex) + "/" + String(totalIndicies));
  display.setTextSize(3.5);
  display.setCursor(0, 12);
  display.print("Audio" + String(currentIndex));
  display.display();
}

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(2.99999);
  display.clearDisplay();

  tmrpcm.speakerPin = 9;
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  pinMode(repeatBtn, INPUT);
  pinMode(skipBtns[0], INPUT);
  pinMode(skipBtns[1], INPUT);

  if (!SD.begin(SD_ChipSelectPin))
  {
    bool displayed = false;
    while (!SD.begin(SD_ChipSelectPin))
    {
      if (displayed == false)
      {
        display.print("Insert SD Card");
        display.display();
        displayed = true;
      }
    }
  }
  display.clearDisplay();
  display.display();

  File root = SD.open("/");
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry)
      break;
    totalIndicies++;
    entry.close();
  }
  totalIndicies--;

  tmrpcm.volume(5);
}

void loop()
{
  if (totalIndicies != 0)
  {
    if (tmrpcm.isPlaying())
    {
      delay(1500);
      if (digitalRead(repeatBtn) == HIGH) // repeat
      {
        repeat = !repeat;

        if (repeat == true)
        {
          currentIndex--;
          disp();
        }
        else
        {
          disp();
          currentIndex++;
        }
      }

      else if (digitalRead(5) == HIGH && repeat == false) // backwards
      {
        currentIndex -= 2;
        tmrpcm.stopPlayback();
      }
      else if (digitalRead(6) == HIGH && repeat == false) // forwards
        tmrpcm.stopPlayback();
    }

    else
    {

      digitalWrite(led, LOW);

      disp();

      String file = "Audio" + String(currentIndex) + ".wav";
      tmrpcm.play((char *)file.c_str());

      digitalWrite(led, HIGH);
      if (repeat == false)
        currentIndex++;
    }
  }
}
