const int buttonPin = 2;
const int ledPin = 3;

// 0 恆亮、1 慢閃、2 中速閃、3 快閃
int mode = 0;

int lastReading = LOW;
int stableButton = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 40;

unsigned long lastBlinkTime = 0;
bool ledOn = true;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // RGB 模組共用腳接 3.3V 時，通常 LOW 會亮
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long now = millis();
  int reading = digitalRead(buttonPin);

  // 按鈕防彈跳
  if (reading != lastReading) {
    lastDebounceTime = now;
  }

  if (now - lastDebounceTime > debounceDelay) {
    if (reading != stableButton) {
      stableButton = reading;

      // 這種三腳按鈕模組通常按下讀到 HIGH
      if (stableButton == HIGH) {
        mode = (mode + 1) % 4;
        ledOn = true;
        lastBlinkTime = now;
      }
    }
  }

  lastReading = reading;

  if (mode == 0) {
    ledOn = true;  // 恆亮
  } else {
    unsigned long interval;

    if (mode == 1) {
      interval = 500;  // 慢閃
    } else if (mode == 2) {
      interval = 250;  // 中速閃
    } else {
      interval = 100;  // 快閃
    }

    if (now - lastBlinkTime >= interval) {
      lastBlinkTime = now;
      ledOn = !ledOn;
    }
  }

  // 共用腳接 3.3V 的 RGB 模組通常是 LOW 亮、HIGH 滅
  digitalWrite(ledPin, ledOn ? LOW : HIGH);
}