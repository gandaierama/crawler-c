#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>


const char UrlCrawl[] = "www.google.com";
const char UrlPath[] = "/";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const int Timeout = 30*1000;
const int Delay = 1000;

void setup()
{

  Serial.begin(9600); 
  while (Ethernet.begin(mac) != 1)
  {
    Serial.println(UrlCrawl);
    delay(15000);
  }  
}

void loop()
{
  int err =0;
  Serial.print(UrlCrawl);
  Serial.print("http response code: ");
  
  EthernetClient c;
  HttpClient http(c);
  Serial.print("http response code: ");
  err = http.get(UrlCrawl, UrlPath);
  if (err == 0)
  {
    Serial.println("Iniciando a comunicação...");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("http response code: ");
      Serial.println(err);


      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");

        unsigned long timeoutStart = millis();
        char c;
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < Timeout) )
        {
            if (http.available())
            {
                c = http.read();
                Serial.print(c);
                bodyLen--;
                timeoutStart = millis();
            }
            else
            {
                delay(Delay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();

  while(1);
}
