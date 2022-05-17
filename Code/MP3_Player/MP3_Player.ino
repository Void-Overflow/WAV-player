#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"
#include <Adafruit_SSD1306.h>

#define SD_ChipSelectPin 8
#define led 10

TMRpcm tmrpcm;
Adafruit_SSD1306 display = Adafruit_SSD1306();

int totalIndicies = 0;
int currentIndex = 1;

bool displayed = false;

void setup()
{
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setTextColor(WHITE);
  display.setTextSize(2.99999);
  display.clearDisplay();

  tmrpcm.speakerPin = 9;
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  if (!SD.begin(SD_ChipSelectPin))
  {
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
  displayed = false;
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
      // Do input handing ; song skipping, repeat, etc...
    }
    else
    {
      if (currentIndex > totalIndicies)
        currentIndex = 1;

      digitalWrite(led, LOW);

      display.clearDisplay();
      display.setTextSize(0.999);
      display.setCursor(0, 0);
      display.print("Now Playing");
      display.setCursor(105, 0);
      display.print(String(currentIndex) + "/" + String(totalIndicies));
      display.setTextSize(3.5);
      display.setCursor(0, 12);
      display.print("Audio" + String(currentIndex));
      display.display();

      String file = "Audio" + String(currentIndex) + ".wav";
      tmrpcm.play((char *)file.c_str());

      digitalWrite(led, HIGH);
      currentIndex++;
    }
  }
}
