#include <WiFi.h>
#include <Time.h>
#include <Adafruit_NeoPixel.h>

/****************************************************************** 
 * WIFI 
 ******************************************************************/
const char* ssid = "Udfret.Ragnarsont";
const char* pw = "68253114913504835470";
int connection_idle_counter = 0;

void setupWIFI()
{
  WiFi.begin(ssid,pw);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    displayWIFIStatus();    
    delay(500);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/****************************************************************** 
 * NTP 
 ******************************************************************/
const char* NTP_SERVER = "de.pool.ntp.org";
const char* TZ_INFO = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";
tm local_time;
time_t epoch_time;

int last_local_time_h = 0;
int last_local_time_m_10 = 0;
int last_local_time_m_1 = 0;

void setupNTP()
{
  configTime(0, 0, NTP_SERVER);
  setenv("TZ", TZ_INFO, 1);
}

void getNTPTime()
{
  time(&epoch_time);
  localtime_r(&epoch_time, &local_time);
}

void printTime(const tm& local_time)
{
  Serial.print(local_time.tm_hour);
  Serial.print(':');
  Serial.print(local_time.tm_min);
  Serial.print(':');
  Serial.println(local_time.tm_sec);
}

/****************************************************************** 
 * LEDs 
 ******************************************************************/
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        14 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMROWS 5
#define NUMCOLS 5
#define BRIGHTNESS 50
#define NUMPIXELS 25 // Popular NeoPixel ring size
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setupLEDs()
{
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();  // Turn OFF all pixels ASAP
}

void clearLED(int i)
{
  pixels.setPixelColor(i, pixels.Color(0, 0, 0));
}

void clearLEDs()
{
  pixels.clear(); // Set all pixels in RAM to 0 (off)
  pixels.show();  // Update strip with new contents
}

void setLED(int i)
{
  pixels.setPixelColor(i, pixels.Color(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS));
}

void setLED(int row, int col)
{
  setLED(row * NUMCOLS + col);
}

void displayWIFIStatus()
{
  connection_idle_counter = (connection_idle_counter + 1) % NUMPIXELS;
  clearLEDs();
  for (int i = 0; i <= connection_idle_counter; i++)
  {
    setLED(i); 
  }
  pixels.show();
  Serial.println(connection_idle_counter);
}

void displayTime(const tm& local_time)
{
  int h = local_time.tm_hour;
  int m = local_time.tm_min;
  
  int m_10 = m / 10;
  int m_1 = m % 10;

  if ((last_local_time_h != h) || (last_local_time_m_10 != m_10) || (last_local_time_m_1 != m_1))
  {
    last_local_time_h = h;
    last_local_time_m_10 = m_10;
    last_local_time_m_1 = m_1;
  }
  else
  {
    return;
  }
  
  switch (m_1) {
  case 1 ... 3:
      m_1 = 0;
      break;
  case 4 ... 6:
      m_1 = 5;
      break;
  case 7 ... 9:
      m_1 = 0;
      m_10 += 1;
      break;
  }

  if (m_10 >= 6) {
      m_10 = 0;
      h += 1;
  }

  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.print(m_10);
  Serial.print(":");
  Serial.println(m_1);
  
  clearLEDs();

  if ((h == 0 || h == 24) && (m_10 + m_1) == 0) {
      setLED(3, 0); setLED(3, 1);
      return;
  }

  if (h == 12 && (m_10 + m_1) == 0) {
      setLED(3, 1); setLED(4, 1);
      return;
  }

  if (h > 12) h -= 12;
  switch (h) {
  case 0:
  case 12:
      setLED(0, 0); setLED(1, 0); setLED(2, 4);
      break;
  case 1:
      setLED(0, 1); setLED(2, 4);
      break;
  case 2:
      setLED(1, 0); setLED(2, 4);
      break;
  case 3:
      setLED(0, 3); setLED(2, 4);
      break;
  case 4:
      setLED(0, 4); setLED(2, 4);
      break;
  case 5:
      setLED(0, 2); setLED(1, 2); setLED(2, 4);
      break;
  case 6:
      setLED(1, 1); setLED(1, 2); setLED(2, 4);
      break;
  case 7:
      setLED(1, 3); setLED(1, 4); setLED(2, 4);
      break;
  case 8:
      setLED(2, 0); setLED(2, 1); setLED(2, 4);
      break;
  case 9:
      setLED(2, 2); setLED(2, 3); setLED(2, 4);
      break;
  case 10:
      setLED(0, 0); setLED(2, 4);
      break;
  case 11:
      setLED(0, 0); setLED(0, 1); setLED(2, 4);
      break;
  }

  if (m == 0) {
      pixels.show();
      return;
  }

  switch (m_10) {
  case 1:
      setLED(3, 4); setLED(4, 4);
      break;
  case 2:
      setLED(3, 2); setLED(4, 2); setLED(4, 4);
      break;
  case 3:
      setLED(3, 3); setLED(3, 4); setLED(4, 4);
      break;
  case 4:
      setLED(4, 0); setLED(4, 2); setLED(4, 4);
      break;
  case 5:
      setLED(4, 1); setLED(4, 2); setLED(4, 4);
      break;
  }

  if (m_1 == 5) {
      setLED(4, 3); setLED(4, 4);
  }

  pixels.show();
}

void setup() {
  Serial.begin(115200);
  setupLEDs();
  setupWIFI();
  setupNTP();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {
    getNTPTime();
    printTime(local_time);
    displayTime(local_time);
    delay(1000);
  }
}
