#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "WİFİ İSMİ";
const char *password = "PAROLASI";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>EBB Pico and ESP 8266 Keyboard</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">

    <title>ESP Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
</head>

<body>
    <style>
        .button {
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            cursor: pointer;
            background: black;
        }
    </style>
    </style>
    <div class="topnav">
        <h1>Keyboard</h1>
    </div>
    <div class="content">
        <div class="card">
            <div class="row">
                <p><button id="button" onclick=sendDate("q") class="button">Q</button>
                    <button id="button" onclick=sendDate('w') class="button">W</button>
                    <button id="button" onclick=sendDate('e') class="button">E</button>
                    <button id="button" onclick=sendDate('r') class="button">R</button>
                    <button id="button" onclick=sendDate('t') class="button">T</button>
                    <button id="button" onclick=sendDate('y') class="button">Y</button>
                    <button id="button" onclick=sendDate('u') class="button">U</button>
                    <button id="button" onclick=sendDate('i') class="button">I</button>
                    <button id="button" onclick=sendDate('o') class="button">O</button>
                    <button id="button" onclick=sendDate('p') class="button">P</button>
                </p>
            </div>
            <div class="row">
                <p>
                    <button id="button" onclick=sendDate('a') class="button">A</button>
                    <button id="button" onclick=sendDate('s') class="button">S</button>
                    <button id="button" onclick=sendDate('d') class="button">D</button>
                    <button id="button" onclick=sendDate('f') class="button">F</button>
                    <button id="button" onclick=sendDate('g') class="button">G</button>
                    <button id="button" onclick=sendDate('h') class="button">H</button>
                    <button id="button" onclick=sendDate('j') class="button">J</button>
                    <button id="button" onclick=sendDate('k') class="button">K</button>
                    <button id="button" onclick=sendDate('l') class="button">L</button>
                </p>
            </div>
            <div class="row">
                <p>
                    <button id="button" onclick=sendDate('z') class="button">Z</button>
                    <button id="button" onclick=sendDate('x') class="button">X</button>
                    <button id="button" onclick=sendDate('c') class="button">C</button>
                    <button id="button" onclick=sendDate('v') class="button">V</button>
                    <button id="button" onclick=sendDate('b') class="button">B</button>
                    <button id="button" onclick=sendDate('n') class="button">N</button>
                    <button id="button" onclick=sendDate('m') class="button">M</button>
                </p>
            </div>
        </div>
    </div>

    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        window.addEventListener('load', onLoad);

        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onopen = onOpen;
            websocket.onclose = onClose;
        }

        function onOpen(event) {
            console.log('Connection opened');
        }

        function onClose(event) {
            console.log('Connection closed');
            setTimeout(initWebSocket, 2000);
        }

        function sendDate(str) {
            websocket.send(str);
        }

        function onLoad(event) {
            initWebSocket();
        }
    </script>
</body>

</html>
)rawliteral";

int pinOutArray[8] = {16, 5, 4, 0, 2, 14, 12, 13};
void getkeyboardInfo(char cr)
{

  Serial.println(cr);
  int c = 0;

  int tempNum = cr;
  Serial.println(tempNum);
  int naber[8];

  for (int i = 0; i < 8; i++)
  {
    naber[i] = tempNum % 2;
    tempNum = tempNum / 2;
  }
  for (int i = 0; i < 8; i++)
  {
    if (naber[i] == 1)
    {
      digitalWrite(pinOutArray[i], HIGH);
    }
    else
    {
      digitalWrite(pinOutArray[i], LOW);
    }
  }
}

void notifyClients()
{
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (!strcmp((char *)data, "") == 0)
    {
      Serial.println((char *)data);
      getkeyboardInfo(((char *)data)[0]);
      notifyClients();

      for (int i = 0; i < 8; i++)
      {
        digitalWrite(pinOutArray[i], LOW);
      }
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String &var)
{
  Serial.println(var);
  return "Successfully";
}

void setup()
{

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  initWebSocket();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });
  server.begin();
}

void loop()
{
  ws.cleanupClients();
}