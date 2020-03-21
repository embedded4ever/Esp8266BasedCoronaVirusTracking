/*
 * -----------------------------------------------------------
 * <vvolkanunal@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff.
 * -----------------------------------------------------------
*/
extern "C" {
#include "user_interface.h"
#include "espconn.h"
}
#include <ESP8266WiFi.h>

bool bGotIpFlag = true;
static uint32_t ulStart = 0;

#define DEBUG 

#ifdef DEBUG
#define PROTOCOL_PRINT(...)\
do {\
  Serial.println(__VA_ARGS__);\
}while(0);
#else
#define PROTOCOL_PRINT(...) do{ }while(0);
#endif 

char* 
ConnectStatus2String(uint8_t ConnectStatusRet)
{
  switch (ConnectStatusRet)
  {
    case 0: return "STATION_IDLE"; break;
    case 1: return "STATION_CONNECTING"; break;
    case 2: return "STATION_WRONG_PASSWORD"; break;
    case 3: return "STATION_NO_AP_FOUND"; break;
    case 4: return "STATION_CONNECT_FAIL"; break;
    case 5: return "STATION_GOT_IP"; break;
    default: return "UNKNOWN"; break;
  }
}

void ICACHE_FLASH_ATTR 
vConnWifiNetworkViaSdk(void)
{
  ulStart = millis();
  
  wifi_set_opmode(STATION_MODE);
  
  char ssid[32] = "yourssid";
  char password[64] = "yourpass";
  
  struct station_config stationConf;
  stationConf.bssid_set = 0;
  os_memcpy(&stationConf.ssid, ssid, 32);
  os_memcpy(&stationConf.password, password, 64);

  wifi_station_set_config(&stationConf);
}

bool 
bGotIp(void)
{
  uint32_t ulElapsedTime = 0;
  
  struct ip_info ipconfig;
  char ipBuf[40];
  int ipAddres[4];
  
  const char *ipstr = "%s%d.%d.%d.%d";
  
  wifi_get_ip_info(STATION_IF, &ipconfig);
  PROTOCOL_PRINT(ConnectStatus2String(wifi_station_get_connect_status()));

  
  if (wifi_station_get_connect_status() == STATION_GOT_IP)
  {
    bGotIpFlag = false;
    
    ipAddres[0] = (ipconfig.ip.addr >> 0) & 0xFF;
    ipAddres[1] = (ipconfig.ip.addr >> 8 ) & 0xFF;
    ipAddres[2] = (ipconfig.ip.addr >> 16 ) & 0xFF;
    ipAddres[3] = (ipconfig.ip.addr >> 24 ) & 0xFF;
    
    sprintf(ipBuf, ipstr, "IP: ", ipAddres[0], ipAddres[1], ipAddres[2], ipAddres[3]);
    PROTOCOL_PRINT(ipBuf);
    
    ulElapsedTime = millis() - ulStart;

    PROTOCOL_PRINT("Elapsed Time : ");
    PROTOCOL_PRINT(ulElapsedTime);
    
  }
}
