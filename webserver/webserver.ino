/* Web_AjaxRGB.pde - example sketch for Webduino library */

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

// CHANGE THIS TO YOUR OWN UNIQUE VALUE
byte mac[] = { 0x02, 0xAA, 0xBB, 0xCC, 0x00, 0x22 };

// IP
IPAddress ip(192, 168, 1, 210);
// GATEWAY
byte gateway[] = { 192, 168, 1, 1 };
//SUBNET
byte subnet[] = { 255, 255, 255, 0 };

int on[8] ;
int pinActual;
//How many of the shift registers - change this
#define number_of_74hc595s 1 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

#define PREFIX ""

WebServer webserver(PREFIX, 80);

#define LED_PIN 8


/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void indexCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();

  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    P(htmlHead) =
    "<html>"
    "<head>"
    "<title>Arduino Web Server</title>"
    "<link href='http://twitter.github.io/bootstrap/assets/css/bootstrap.css' rel='stylesheet'>"
        "<link href='http://twitter.github.io/bootstrap/assets/css/bootstrap-responsive.css' rel='stylesheet'>"
    "<style type='text/css'>body {padding-top: 40px;font-size:20px;}</style>"
    "</head>"
    "<body>"
    "<div class='container'>"
    "<form id=\"form1\" name=\"form1\" method=\"post\" action=\"/form\"><fieldset>"
    "<legend>Led</legend>"
    "<label class='radio'>"
    "<input type=\"radio\" value=\"1\" name=\"l\" id=\"l\" />"  
    "ON</label>"
    "<label class='radio'>"
    "<input type=\"radio\" value=\"0\" name=\"l\" id=\"l\" />"  
    "OFF</label>"
    "<input name=\"Enviar\" type=\"submit\" class='btn' />"
    "</fieldset></form>"
    "</div></body></html>"
    ;        
    

    /* this is a special form of print that outputs from PROGMEM */
    server.printP(htmlHead);
  }
}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}


void formCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  
 if (type == WebServer::POST)
  {
    //digitalWrite(LED_PIN, HIGH);
    bool repeat;
    char name[16], value[16];
    do
    {
      repeat = server.readPOSTparam(name, 16, value, 16);
      if (name[0] == 'l')
      {

        //on[LED_PIN] = !on[LED_PIN];      
        //int valor =  on[pinActual] ? HIGH : LOW;
        int valor = strtoul(value, NULL, 10);
        Serial.print(valor);        
        digitalWrite(LED_PIN, valor? HIGH : LOW);
      }
    }while (repeat);
    
        
    server.httpSeeOther(PREFIX "/index.html");
  } 
}

void setup()
{
    Serial.begin(9600);


  pinMode(LED_PIN, OUTPUT); 
  
  /* initialize the Ethernet adapter */
  Ethernet.begin(mac, ip, gateway, subnet);
     
  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &indexCmd);
  webserver.addCommand("form", &formCmd);


  /* start the server to wait for connections */
  webserver.begin();
}





void loop()
{
  char buff[64];
  int len = 64;

  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
}
