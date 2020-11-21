//значения

#define LedsNum 50
#define LedsPin 3
#define MicrofonPin 0

//адресная лента
#include <microLED.h>

LEDdata leds [LedsNum];
microLED strip(leds, LedsNum, LedsPin);  // объект лента

int MaskToShow = 7;



//варианты масок (8 вариантов по 50 пикселей)
boolean MaskLayors[8][50]
{
  { false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,
  },
  { false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,

    false, true, true, true, true, true, true, true, true, false,

    false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,
  },
  { false, false, false, false, false, false, false, false, false, false,

    false, false, false, false, false, false, false, false, false, false,

    true, true, true, true, true, true, true, true, true, true,

    false, false, true, true, true, true, true, true, false, false,

    false, false, false, false, false, false, false, false, false, false,
  },
  { false, false, false, false, false, false, false, false, false, false,

    false, true, true, true, true, true, true, true, true, false,

    true, false, false, false, false, false, false, false, false, true,

    false, true, true, true, true, true, true, true, true, false,

    false, false, false, false, false, false, false, false, false, false,
  },
  { false, false, false, false, false, false, false, false, false, false,

    false, true, true, true, true, true, true, true, true, false,

    true, false, false, false, false, false, false, false, false, true,

    false, true, false, false, false, false, false, false, true, false,

    false, false, true, true, true, true, true, true, false, false,
  },
  { false, false, true, true, true, true, true, true, false, false,

    false, true, false, false, false, false, false, false, true, false,

    true, false, false, false, false, false, false, false, false, true,

    false, true, false, false, false, false, false, false, true, false,

    false, false, true, true, true, true, true, true, false, false,
  },
  { false, true, true, true, true, true, true, true, true, false,

    true, false, false, false, false, false, false, false, false, true,

    true, false, false, false, false, false, false, false, false, true,

    false, true, false, false, false, false, false, false, true, false,

    false, false, true, true, true, true, true, true, false, false,
  },
  { false, true, true, true, true, true, true, true, true, false,

    true, false, false, false, false, false, false, false, false, true,

    true, false, false, false, false, false, false, false, false, true,

    true, false, false, false, false, false, false, false, false, true,

    false, true, true, true, true, true, true, true, true, false,
  },
};

int ResultToShow;

//аудио
int Result;
int MicrofoneSignal;
float NoiceRate = 30;
float NoiceCorrectStep = 0.005;
int NoicePorogUpOnNoice = 5;
int i;
int MaxSignalRateOnLL;
int Zn;

void setup() //стартуем!!!
{
  delay(1000);

  strip.setBrightness(30);    // яркость (0-255) , яркость применяется при выводе .show() !

  strip.setRGB(1, 255, 255, 255);

  strip.show();

  delay(100);
  int N1 = analogRead(MicrofonPin);
  delay(50);
  int N2 = analogRead(MicrofonPin);
  delay(50);
  int N3 = analogRead(MicrofonPin);
  delay(50);
  int N4 = analogRead(MicrofonPin);
  delay(50);
  int N5 = analogRead(MicrofonPin);
  delay(50);

  NoiceRate = (N1 + N2 + N3 + N4 + N5) / 5;

  ResultToShow = 0;

  //отладка
  Serial.begin(9600);
}

void loop()
{
  //делаем выборку из значений за определённый промежуток времени-----
  i = 0;
  MaxSignalRateOnLL = 0;
  Zn = 0;
  while (i < 50)
  {
    Zn = analogRead(MicrofonPin);

    if (Zn > MaxSignalRateOnLL)
    {
      MaxSignalRateOnLL = Zn;
    }

    delay(3);
    i++;
  }
  MicrofoneSignal = MaxSignalRateOnLL;


  //корректируем значение шума-----
  if (MicrofoneSignal > NoiceRate)
  {
    NoiceRate = NoiceRate + NoiceCorrectStep;
  }
  else
  {
    NoiceRate = NoiceRate - NoiceCorrectStep;
  }
  MicrofoneSignal = MicrofoneSignal - NoiceRate;


  //получаем результат громкости по шкале от 0 до 7 (1 - 8)-----
  //6-30
  Result = 0;

  if (MicrofoneSignal > 8)
  {
    Result = 1;
  }

  if (MicrofoneSignal > 12)
  {
    Result = 2;
  }

  if (MicrofoneSignal > 16)
  {
    Result = 3;
  }

  if (MicrofoneSignal > 21)
  {
    Result = 4;
  }

  if (MicrofoneSignal > 27)
  {
    Result = 5;
  }

  if (MicrofoneSignal > 34)
  {
    Result = 6;
  }

  if (MicrofoneSignal > 40)
  {
    Result = 7;
  }

  //отладка -----
  //Serial.println(Result);
  //Serial.println(MicrofoneSignal);
  //Serial.println(NoiceRate);

  //MicrofoneSignal = analogRead(MicrofonPin);

  //отладка
  //Serial.println(MicrofoneSignal);

  //сглаживаем результат-----

  if (ResultToShow != Result)
  {
    if (Result > ResultToShow)
    {
      if (Result - 2 > ResultToShow)
      {
        ResultToShow = ResultToShow + 3;
      }
      else
      {
        if (Result - 1 > ResultToShow)
        {
          ResultToShow = ResultToShow + 2;
        }
        else
        {
          ResultToShow = ResultToShow + 1;
        }
      }
    }
    else
    {
      if (Result + 2 < ResultToShow)
      {
        ResultToShow = ResultToShow - 2;
      }
      else
      {
        if (Result + 1 < ResultToShow)
        {
          ResultToShow = ResultToShow - 1;
        }
        else
        {
          ResultToShow = ResultToShow - 1;
        }
      }
    }
  }

  if (ResultToShow > 7)
  {
    ResultToShow = 7;
    Serial.println("ERRORplus");
  }
  if (ResultToShow < 0)
  {
    ResultToShow = 0;
    Serial.println("ERRORminus");
  }

  Serial.println(ResultToShow);

  //выводим нужный вариант маски на экран
  MaskToShow = ResultToShow;
  for (int i = 0; i < LedsNum; i++)
  {
    boolean NastLedON = MaskLayors[MaskToShow][(LedsNum - 1) - i]; //считываем данные перевёрнутыми!!!!!
    if (NastLedON)
    {
      strip.setRGB(i, 255, 255, 255);
    }
    else
    {
      strip.setRGB(i, 0, 0, 0);
    }
  }
  strip.show();

  //ждём
  //delay(100);
}
