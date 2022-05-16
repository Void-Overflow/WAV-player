#include "SD.h"
#include "SPI.h"
#include "TMRpcm.h"

#define SD_ChipSelectPin 8
#define led 10

TMRpcm tmrpcm;

int totalIndicies = 0;
int currentIndex = 1;

void setup()
{
  Serial.begin(9600);

  tmrpcm.speakerPin = 9;
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  if (!SD.begin(SD_ChipSelectPin))
    while (!SD.begin(SD_ChipSelectPin))
    {
    }

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

      String file = "Audio" + String(currentIndex) + ".wav";
      Serial.println(file + " is now being played.");
      tmrpcm.play((char *)file.c_str());

      digitalWrite(led, HIGH);
      currentIndex++;
    }
  }
}
