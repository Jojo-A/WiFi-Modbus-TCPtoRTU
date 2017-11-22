#include ".\h\trace.h"
#include "FS.h"

WiFiServer tcpTraceServer(23);
extern const char* ssid;
extern const char* pass;
extern String default_node_name;

/******************************************************************************/
void cTcpTrace::begin()
{
  tcpTraceServer.begin();
  menuState = 0;
}
/******************************************************************************/
void cTcpTrace::task(void)
{
  if (tcpTraceServer.hasClient())
  {
    //if (!tcpTraceClient || !tcpTraceClient.connected())
    {
      if(tcpTraceClient) tcpTraceClient.stop();
      tcpTraceClient = tcpTraceServer.available();
      println("Welcome to trace.");
    }
  }
  if (tcpTraceClient)
  {
     menu();
  }
}
/******************************************************************************/
size_t cTcpTrace::write(uint8_t c)
{
  if (tcpTraceClient)
  {
     return tcpTraceClient.write(c);
  }
  return 0;
}
/******************************************************************************/
size_t  cTcpTrace::write(const uint8_t *buf, size_t size)
{
  if (tcpTraceClient)
  {
     return tcpTraceClient.write(buf, size);
     tcpTraceClient.flush();
  }
  return 0;
}
/******************************************************************************/
int cTcpTrace::available(void)
{
  if (tcpTraceClient)
  {
    return tcpTraceClient.available();
  }
  else
    return 0;
}
/******************************************************************************/
int cTcpTrace::peek(void)
{
  if (tcpTraceClient)
  {
    return tcpTraceClient.peek();
  }
  else
    return 0;
}

/******************************************************************************/
int cTcpTrace::read(void)
{
  if (tcpTraceClient)
  {
    return tcpTraceClient.read();
  }
  else
    return 0;
}
/******************************************************************************/
void cTcpTrace::flush()
{
  if (tcpTraceClient)
  {
    tcpTraceClient.flush();
  }
}

/******************************************************************************/
void cTcpTrace::menu()
{
  static String dataIn = "";
  static String dataInChar = "";
  static String station_ssid = "";
  static String station_psk = "";
  static String station_name = "";

  switch (menuState)
  {
    case 0:
    {
      if (! loadConfig(&station_ssid, &station_psk, &station_name))
        {
          station_ssid = ssid;
          station_psk = pass;
          station_name = default_node_name;
          saveConfig(&station_ssid, &station_psk, &station_name);
          println("Save Config");
        }
        menuState=1;
        break;
      }
      case 1:
      {
      println(String()+ "********************************************************************************");
      println(String()+ "* My DHCP IP address:      "+ WiFi.localIP().toString());
      println(String()+ "* (T) Target SSID:         "+    station_ssid);
      println(String()+ "* (P) PSK for target SSID: "+    station_psk );
      println(String()+ "* (N) Node Name:           "+    station_name);
      println(String()+ "* (D) Delete config file(!)");
      println(String()+ "* (S) Save config!");
      println(String()+ "* (R) Restart");
      
      menuState = 2;
      break;
    }
    case 2:
    {
      while( available())
        {
          dataInChar = readString();
          print(dataInChar);
          dataIn += dataInChar;
        }
        int16_t len = dataIn.indexOf ("\r\n");
        if (len != -1)
        {
          println("");
          uint16_t comm = dataIn.charAt(0);
          switch (comm)
          {
            case 'T': { menuState = 20; print("Enter target SSID: "); break; }
            case 'P': { menuState = 30; print("Enter PSK for target SSID: "); break;}
            case 'N': { menuState = 40; print("Enter new node name: "); break;}
            case 'R': { ESP.restart(); break;}
            case 'S':
            { menuState = 0;
              println("Saving config...");
              saveConfig(&station_ssid, &station_psk, &station_name);
              break;
            }
            case 'D':
            { menuState = 1;
              if(SPIFFS.remove("/cl_conf.txt")) println("Config file successfully removed, please restart.");
              else                              println("Config file could not be removed.");
              break;
            }

            default:
            println("No command!!!" );
            println(comm, HEX);

          }
          dataIn = "";
        }
      break;
    }

    case 20:
    {
      while( available())
      {
        dataInChar = readString();
        print(dataInChar);
        dataIn += dataInChar;
      }
      int16_t len = dataIn.indexOf ("\r\n");
      if (len != -1)
      {
        station_ssid = dataIn.substring(0, len);
        dataIn = "";
        menuState = 1;
      }
      break;
    }

    case 30:
    {
      while( available())
      {
        dataInChar = readString();
        print(dataInChar);
        dataIn += dataInChar;
      }
      int16_t len = dataIn.indexOf ("\r\n");
      if (len != -1)
      {
        station_psk = dataIn.substring(0, len);
        dataIn = "";
        menuState = 1;
      }
      break;
    }
      
    case 40:
    {
      while( available())
      {
        dataInChar = readString();
        print(dataInChar);
        dataIn += dataInChar;
      }
      int16_t len = dataIn.indexOf ("\r\n");
      if (len != -1)
      {
        station_name = dataIn.substring(0, len);
        dataIn = "";
        menuState = 1;
      }
      break;
    }

    default:
      menuState = 0 ;
  }
}

cTcpTrace trace;
