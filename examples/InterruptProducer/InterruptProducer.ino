#include <CircularBuffer.h>
#include <MsTimer2.h>

typedef uint32_t bufferType;
#define bufferSize (100)
static bufferType mem[bufferSize];

static CircularBuffer<bufferType> buff(mem, bufferSize);

static void timerInterruptHandler(void)
{
  static bufferType count = 1;
  for (uint8_t i = 0; i < bufferSize; ++i)
  {
    if (buff.pushFront(&count))
    {
      ++count;
    }
    else
    {
      return;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("** Cyclic buffer timer interrupt demo **");

  // Generate a timer interrupt every millisecond.
  // From this timer try to fill the whole buffer with counter values.
  MsTimer2::set(1 /*cycle, ms*/, timerInterruptHandler);
  MsTimer2::start();
}

void loop()
{
  static bufferType prev = 0;

  bufferType* back = buff.getBack();
  if (!back)
    return;

  Serial.print( buff.available() );
  Serial.print( "\t" );
  Serial.print( *back );

  if ((*back) != (prev + 1))
  {
    Serial.print("\t## FAILURE ##");
  }
  prev = *back;
  buff.popBack();
  Serial.println("");
}
