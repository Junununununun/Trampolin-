const int trigPin = 9;
const int echoPin = 10;

const int NO_PERSON_THRESHOLD = 194; // 超过这个值认为无人
const int MIN_HEIGHT_CM = 140;       // 最低检测高度

int baseHeight = 0;
bool personPresent = false;
bool baseCaptured = false;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distanceCM = duration * 0.034 / 2;
  return distanceCM;
}

void loop() {
  long distance = getDistanceCM();

  if (distance >= NO_PERSON_THRESHOLD) {
    personPresent = false;
    baseCaptured = false;
  } else {
    if (!personPresent) {
      personPresent = true;
      baseCaptured = false;
    }

    if (!baseCaptured) {
      if (distance <= MIN_HEIGHT_CM) {
        baseHeight = distance;
        baseCaptured = true;
      }
    }
  }

  int delta = 0;
  if (personPresent && baseCaptured) {
    delta = baseHeight - distance;
  }

  int level = 0;
  if (delta >= 5 && delta < 10) {
    level = 1;
  } else if (delta >= 10 && delta < 15) {
    level = 2;
  } else if (delta >= 15) {
    level = 3;
  }

  Serial.print("baseHeight:");
  Serial.print(baseHeight);
  Serial.print(", distance:");
  Serial.print(distance);
  Serial.print(", delta:");
  Serial.print(delta);
  Serial.print(", level:");
  Serial.println(level);

  delay(50);
}
