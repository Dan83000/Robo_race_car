/*
 * ============================================================
 *   WiFi Robot Car Controller
 *   Hardware : NodeMCU ESP8266 + L298N + 4x DC Motor (12V 200RPM)
 *   WiFi     : SSID = rsmaurya  |  Password = 123456788
 *   Control  : Web Browser (Mobile / PC)
 *   Features : Forward / Backward / Left / Right / Stop
 *              Speed Control (PWM 0-100%)
 * ============================================================
 *  PIN MAPPING
 *  NodeMCU D1 (GPIO5)  --> L298N IN1
 *  NodeMCU D2 (GPIO4)  --> L298N IN2
 *  NodeMCU D3 (GPIO0)  --> L298N IN3
 *  NodeMCU D4 (GPIO2)  --> L298N IN4
 *  NodeMCU D5 (GPIO14) --> L298N ENA  (PWM - Left Speed)
 *  NodeMCU D6 (GPIO12) --> L298N ENB  (PWM - Right Speed)
 * ============================================================
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* ─── WiFi Credentials ──────────────────────────────────────── */
const char* WIFI_SSID     = "rsmaurya";
const char* WIFI_PASSWORD = "123456788";

/* ─── Motor Driver Pins ─────────────────────────────────────── */
#define IN1  5   // D1 - Left  Motor Direction A
#define IN2  4   // D2 - Left  Motor Direction B
#define IN3  0   // D3 - Right Motor Direction A
#define IN4  2   // D4 - Right Motor Direction B
#define ENA  14  // D5 - Left  Motor PWM Speed
#define ENB  12  // D6 - Right Motor PWM Speed

/* ─── PWM Settings ──────────────────────────────────────────── */
#define PWM_FREQ    1000   // 1kHz PWM frequency
#define PWM_RANGE   255    // 8-bit resolution (0-255)

/* ─── Global Speed (0-255) ──────────────────────────────────── */
int motorSpeed = 200;      // Default speed ~78%

/* ─── Web Server on port 80 ─────────────────────────────────── */
ESP8266WebServer server(80);

/* ══════════════════════════════════════════════════════════════
 *  MOTOR CONTROL FUNCTIONS
 * ══════════════════════════════════════════════════════════════ */

void setSpeed(int spd) {
  motorSpeed = constrain(spd, 0, 255);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void motorStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moveForward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH);  // Left  forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Right forward
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);   // Left  backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);   // Right backward
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);   // Left  backward
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  // Right forward
  digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, HIGH);  // Left  forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Right backward
  digitalWrite(IN4, HIGH);
}

/* Soft turn (one side stops) */
void softLeft() {
  analogWrite(ENA, 0);
  analogWrite(ENB, motorSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void softRight() {
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

/* ══════════════════════════════════════════════════════════════
 *  HTML WEB PAGE (Mobile Responsive Controller)
 * ══════════════════════════════════════════════════════════════ */
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>RS Maurya Robot Car</title>
  <style>
    * { margin:0; padding:0; box-sizing:border-box; touch-action:manipulation; }
    body {
      background: linear-gradient(135deg, #0f0c29, #302b63, #24243e);
      min-height: 100vh;
      font-family: 'Arial', sans-serif;
      color: white;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 15px;
    }
    h1 {
      font-size: 1.4em;
      margin: 10px 0 5px;
      color: #00e5ff;
      text-shadow: 0 0 10px #00e5ff88;
      letter-spacing: 2px;
    }
    .status-bar {
      background: rgba(255,255,255,0.08);
      border: 1px solid #00e5ff44;
      border-radius: 20px;
      padding: 6px 18px;
      font-size: 0.8em;
      margin-bottom: 12px;
      color: #80deea;
    }
    #cmd-display {
      background: rgba(0,229,255,0.1);
      border: 1px solid #00e5ff;
      border-radius: 10px;
      padding: 8px 20px;
      font-size: 1.1em;
      font-weight: bold;
      color: #00e5ff;
      margin-bottom: 15px;
      letter-spacing: 3px;
      min-width: 200px;
      text-align: center;
    }

    /* ── Speed Slider ── */
    .speed-section {
      width: 100%;
      max-width: 380px;
      background: rgba(255,255,255,0.06);
      border-radius: 16px;
      padding: 14px 20px;
      margin-bottom: 18px;
      border: 1px solid rgba(255,255,255,0.1);
    }
    .speed-label {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 8px;
      font-size: 0.9em;
      color: #b0bec5;
    }
    #speed-val {
      font-size: 1.2em;
      font-weight: bold;
      color: #69f0ae;
    }
    input[type=range] {
      width: 100%;
      height: 8px;
      border-radius: 4px;
      background: linear-gradient(to right, #69f0ae, #00e5ff);
      outline: none;
      -webkit-appearance: none;
    }
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 24px; height: 24px;
      border-radius: 50%;
      background: white;
      box-shadow: 0 2px 8px rgba(0,0,0,0.4);
      cursor: pointer;
    }

    /* ── D-Pad Buttons ── */
    .dpad {
      display: grid;
      grid-template-columns: repeat(3, 85px);
      grid-template-rows: repeat(3, 85px);
      gap: 8px;
      margin-bottom: 20px;
    }
    .btn {
      border: none;
      border-radius: 14px;
      font-size: 1.8em;
      font-weight: bold;
      cursor: pointer;
      display: flex;
      align-items: center;
      justify-content: center;
      flex-direction: column;
      gap: 2px;
      transition: transform 0.08s, box-shadow 0.08s;
      user-select: none;
      -webkit-user-select: none;
    }
    .btn span {
      font-size: 0.35em;
      letter-spacing: 1px;
      font-weight: normal;
    }
    .btn:active { transform: scale(0.91); box-shadow: 0 2px 8px rgba(0,0,0,0.5); }

    .btn-fwd  { background: linear-gradient(145deg,#00c853,#1b5e20); color:white; grid-column:2; grid-row:1; }
    .btn-bwd  { background: linear-gradient(145deg,#d50000,#7f0000); color:white; grid-column:2; grid-row:3; }
    .btn-left { background: linear-gradient(145deg,#2979ff,#0d47a1); color:white; grid-column:1; grid-row:2; }
    .btn-right{ background: linear-gradient(145deg,#2979ff,#0d47a1); color:white; grid-column:3; grid-row:2; }
    .btn-stop { background: linear-gradient(145deg,#ff6f00,#e65100); color:white; grid-column:2; grid-row:2; font-size:1.4em; }

    /* Diagonal soft turn */
    .btn-sl   { background: rgba(41,121,255,0.35); color:#90caf9; grid-column:1; grid-row:1; font-size:1.3em; }
    .btn-sr   { background: rgba(41,121,255,0.35); color:#90caf9; grid-column:3; grid-row:1; font-size:1.3em; }
    .btn-blank{ grid-column:1; grid-row:3; }
    .btn-blank2{ grid-column:3; grid-row:3; }

    /* ── Extra row ── */
    .extra-row {
      display: flex;
      gap: 10px;
      margin-bottom: 15px;
    }
    .btn-extra {
      padding: 12px 28px;
      border: none;
      border-radius: 12px;
      font-size: 0.9em;
      font-weight: bold;
      cursor: pointer;
      letter-spacing: 1px;
    }
    .btn-slow { background: #37474f; color:#b0bec5; }
    .btn-med  { background: #1565c0; color:white; }
    .btn-fast { background: #b71c1c; color:white; }

    .footer { font-size: 0.7em; color: #546e7a; margin-top: 10px; }
  </style>
</head>
<body>
  <h1>&#127947; RS MAURYA ROBOT CAR</h1>
  <div class="status-bar">WiFi: rsmaurya &nbsp;|&nbsp; IP: <span id="ip-addr">Loading...</span></div>
  <div id="cmd-display">STOP</div>

  <!-- Speed Control -->
  <div class="speed-section">
    <div class="speed-label">
      <span>&#9889; SPEED CONTROL</span>
      <span id="speed-val">78%</span>
    </div>
    <input type="range" id="speedSlider" min="0" max="100" value="78"
           oninput="updateSpeed(this.value)">
  </div>

  <!-- D-Pad -->
  <div class="dpad">
    <button class="btn btn-sl"    ontouchstart="sendCmd('softleft')"  onmousedown="sendCmd('softleft')"  ontouchend="sendCmd('stop')" onmouseup="sendCmd('stop')">&#8598;<span>SOFT L</span></button>
    <button class="btn btn-fwd"   ontouchstart="sendCmd('forward')"   onmousedown="sendCmd('forward')"   ontouchend="sendCmd('stop')" onmouseup="sendCmd('stop')">&#9650;<span>FORWARD</span></button>
    <button class="btn btn-sr"    ontouchstart="sendCmd('softright')" onmousedown="sendCmd('softright')" ontouchend="sendCmd('stop')" onmouseup="sendCmd('stop')">&#8599;<span>SOFT R</span></button>
    <button class="btn btn-left"  ontouchstart="sendCmd('left')"      onmousedown="sendCmd('left')"      ontouchend="sendCmd('stop')" onmouseup="sendCmd('stop')">&#9664;<span>LEFT</span></button>
    <button class="btn btn-stop"  onclick="sendCmd('stop')">&#9646;&#9646;<span>STOP</span></button>
    <button class="btn btn-right" ontouchstart="sendCmd('right')"     onmousedown="sendCmd('right')"     ontouchend="sendCmd('stop')" onmouseup="sendCmd('stop')">&#9654;<span>RIGHT</span></button>
    <div class="btn-blank"></div>
    <button class="btn btn-bwd"   ontouchstart="sendCmd('backward')"  onmousedown="sendCmd('backward')"  ontouchend="sendCmd('stop')" onmouseup="sendCmd('stop')">&#9660;<span>BACKWARD</span></button>
    <div class="btn-blank2"></div>
  </div>

  <!-- Quick Speed Presets -->
  <div class="extra-row">
    <button class="btn-extra btn-slow" onclick="setPreset(30)">&#128034; SLOW</button>
    <button class="btn-extra btn-med"  onclick="setPreset(60)">&#128008; MEDIUM</button>
    <button class="btn-extra btn-fast" onclick="setPreset(100)">&#128007; FAST</button>
  </div>

  <div class="footer">NodeMCU ESP8266 | L298N | 12V 200RPM | RS Maurya</div>

  <script>
    // Set IP in status bar
    document.getElementById('ip-addr').textContent = window.location.hostname;

    const cmdDisplay = document.getElementById('cmd-display');
    const cmdNames = {
      forward:'FORWARD', backward:'BACKWARD', left:'TURN LEFT',
      right:'TURN RIGHT', stop:'STOP', softleft:'SOFT LEFT', softright:'SOFT RIGHT'
    };

    function sendCmd(cmd) {
      cmdDisplay.textContent = cmdNames[cmd] || cmd.toUpperCase();
      fetch('/cmd?action=' + cmd)
        .catch(e => console.log('err:', e));
    }

    function updateSpeed(val) {
      document.getElementById('speed-val').textContent = val + '%';
      fetch('/speed?val=' + val)
        .catch(e => console.log('err:', e));
    }

    function setPreset(val) {
      document.getElementById('speedSlider').value = val;
      updateSpeed(val);
    }

    // Keyboard control (PC)
    const keyMap = {
      ArrowUp:'forward', ArrowDown:'backward',
      ArrowLeft:'left', ArrowRight:'right', ' ':'stop',
      w:'forward', s:'backward', a:'left', d:'right'
    };
    document.addEventListener('keydown', e => {
      if (keyMap[e.key]) { e.preventDefault(); sendCmd(keyMap[e.key]); }
    });
    document.addEventListener('keyup', e => {
      if (keyMap[e.key] && keyMap[e.key] !== 'stop') sendCmd('stop');
    });
  </script>
</body>
</html>
)rawliteral";

/* ══════════════════════════════════════════════════════════════
 *  WEB SERVER ROUTE HANDLERS
 * ══════════════════════════════════════════════════════════════ */

void handleRoot() {
  server.send_P(200, "text/html", HTML_PAGE);
}

void handleCommand() {
  if (!server.hasArg("action")) {
    server.send(400, "text/plain", "Missing action");
    return;
  }
  String action = server.arg("action");

  if      (action == "forward")   moveForward();
  else if (action == "backward")  moveBackward();
  else if (action == "left")      turnLeft();
  else if (action == "right")     turnRight();
  else if (action == "stop")      motorStop();
  else if (action == "softleft")  softLeft();
  else if (action == "softright") softRight();

  server.send(200, "text/plain", "OK:" + action);

  Serial.print("[CMD] ");
  Serial.println(action);
}

void handleSpeed() {
  if (!server.hasArg("val")) {
    server.send(400, "text/plain", "Missing val");
    return;
  }
  int percent = server.arg("val").toInt();
  int pwmVal  = map(constrain(percent, 0, 100), 0, 100, 0, 255);
  setSpeed(pwmVal);

  server.send(200, "text/plain", "Speed:" + String(percent) + "%");

  Serial.print("[SPEED] ");
  Serial.print(percent);
  Serial.print("% -> PWM:");
  Serial.println(pwmVal);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

/* ══════════════════════════════════════════════════════════════
 *  SETUP
 * ══════════════════════════════════════════════════════════════ */
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("========================================");
  Serial.println("  RS Maurya WiFi Robot Car Starting...");
  Serial.println("========================================");

  /* Motor pins setup */
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  motorStop();  // Safety: motors off at start

  /* PWM setup */
  analogWriteFreq(PWM_FREQ);
  analogWriteRange(PWM_RANGE);

  /* Connect to WiFi */
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 40) {
      Serial.println("\n[ERROR] WiFi connect failed! Check credentials.");
      Serial.println("Restarting in 5s...");
      delay(5000);
      ESP.restart();
    }
  }

  Serial.println("\n[OK] WiFi Connected!");
  Serial.print("[IP] Open browser: http://");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------------");

  /* Register web routes */
  server.on("/",       handleRoot);
  server.on("/cmd",    handleCommand);
  server.on("/speed",  handleSpeed);
  server.onNotFound(   handleNotFound);

  server.begin();
  Serial.println("[SERVER] Web server started on port 80");
  Serial.println("========================================");
  Serial.println("  Controls:");
  Serial.println("  Keyboard: W/S/A/D or Arrow Keys");
  Serial.println("  Mobile:   Touch buttons on webpage");
  Serial.println("========================================");
}

/* ══════════════════════════════════════════════════════════════
 *  LOOP
 * ══════════════════════════════════════════════════════════════ */
void loop() {
  server.handleClient();  // Handle incoming HTTP requests
}

/* ══════════════════════════════════════════════════════════════
 *  END OF FILE
 * ══════════════════════════════════════════════════════════════
 *
 *  UPLOAD INSTRUCTIONS:
 *  1. Arduino IDE open karo
 *  2. Board: "NodeMCU 1.0 (ESP-12E Module)" select karo
 *  3. Libraries install karo:
 *     - ESP8266WiFi      (ESP8266 board package me included)
 *     - ESP8266WebServer (ESP8266 board package me included)
 *  4. Board Manager me ESP8266 add karo:
 *     URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
 *  5. COM port select karo
 *  6. Upload karo (115200 baud)
 *  7. Serial Monitor kholo → IP address dekho
 *  8. Browser me IP open karo → Car control karo!
 *
 * ══════════════════════════════════════════════════════════════ */
