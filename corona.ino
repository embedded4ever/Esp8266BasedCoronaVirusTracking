#include <ESP8266HTTPClient.h> 
#include <LiquidCrystal_I2C.h>
#include "json_parser.h"
#include "WifiConnect.h"

#define s2ms(second) (second*1000)
unsigned long long prev_millis(0);

#define country_code "Turkey"

LiquidCrystal_I2C lcd(0x3f, 16, 2);

int interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
bool bFirstKickMillis = false;

extern bool bGotIpFlag;

static String build_url_from_country(String country)
{
  String url = "http://coronavirus-19-api.herokuapp.com/countries/";
  url = url + country;
  return url;
}

void setup(void)
{ 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Covid-19 Watch");

  #if defined JSON_DEBUG
    Serial.begin(9600);
  #endif
  
  JSON_LOG("Connecting...");
  
  vConnWifiNetworkViaSdk();
}

void loop()
{
  if(bGotIpFlag) bGotIp();
  
  if(bFirstKickMillis) CurrentMillis = millis();
  
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval) 
  {
    if(!bFirstKickMillis) CurrentMillis = 0;
    
    bFirstKickMillis = true;
    
    PreviousMillis = CurrentMillis;
    
    HTTPClient http; 
    http.begin(build_url_from_country(country_code));
    
    int httpCode = http.GET(); 
  
    if(httpCode > 0) 
    {
      String payload = http.getString();
       
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      
      payload.toCharArray(JsonArray, payload.length() + 1);
      
      JSON_LOG(JsonArray);
      
      if (json_validate(JsonArray))
      {
        int confirmed = (int)get_json_value(JsonArray, "cases", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
      
        JSON_LOG(confirmed);
        JSON_LOG(deaths);
        JSON_LOG(recovered);
        
        lcd.clear();
        lcd.print("Cnfrmd");
        lcd.setCursor(7,0);
        lcd.print("dths");
        lcd.setCursor(12,0);
        lcd.print("rcvd");
        lcd.setCursor(2, 1);
        lcd.print(confirmed);
        lcd.setCursor(8, 1);
        lcd.print(deaths);
        lcd.setCursor(14, 1);
        lcd.print(recovered);
        
      }
      
      free(JsonArray);
    }
    
    http.end(); 
  }
}
