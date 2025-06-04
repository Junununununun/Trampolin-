const int trigPin = 9;
const int echoPin = 10;

const int NO_PERSON_THRESHOLD = 200; // über diesem Wert (cm) = keine Person
const int MIN_HEIGHT_CM = 140;       // Mindesthöhe zur Erkennung

int baseHeight = 0;
bool personPresent = false;
bool baseCaptured = false;
int lastLevel = 0;

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

// MIDI Note senden
void sendMIDINote(byte note, byte velocity) {
  Serial.write((byte)0x90); // Note On, Channel 1
  Serial.write(note);
  Serial.write(velocity);
}

// MIDI Note ausschalten
void sendMIDINoteOff(byte note) {
  Serial.write((byte)0x80); // Note Off, Channel 1
  Serial.write(note);
  Serial.write((byte)0x00);
}

void loop() {
  long distance = getDistanceCM();

  if (distance >= NO_PERSON_THRESHOLD) {
    // Keine Person
    if (personPresent) {
      personPresent = false;
      baseCaptured = false;

      // Letzte Note ausschalten
      sendMIDINoteOff((byte)(60 + lastLevel));
      lastLevel = 0;
    }
  } else {
    // Person erkannt
    if (!personPresent) {
      personPresent = true;
      baseCaptured = false;
    }

    if (!baseCaptured) {
      if (distance <= MIN_HEIGHT_CM) {
        baseHeight = distance;
        baseCaptured = true;
      } else {
        // Person zu klein
        sendMIDINoteOff((byte)(60 + lastLevel));
        lastLevel = 0;
        delay(50);
        return;
      }
    }

    int delta = baseHeight - distance;
    int level = 0;

    if (delta >= 10 && delta < 20) {
      level = 1;
    } else if (delta >= 20 && delta < 30) {
      level = 2;
    } else if (delta >= 30) {
      level = 3;
    } else {
      level = 0;
    }

    if (level != lastLevel) {
      // Alte Note aus
      sendMIDINoteOff((byte)(60 + lastLevel));
      // Neue Note an
      sendMIDINote((byte)(60 + level), 100);
      lastLevel = level;
    }
  }

  delay(50);
}
