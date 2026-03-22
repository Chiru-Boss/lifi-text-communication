// --- LASER TRANSMITTER (Multi-char + Symbols + Official Animated Output) ---
// Laser on D3 (HIGH = ON)

const int laserPin = 3;
const int bitDelay = 30;      // ms per bit
const int gapDelay = 3 * bitDelay;  // gap between characters

// Encode table (A–Z, 0–9, space, symbols)
String encodeChar(char c) {
  switch (c) {
    // Letters
    case 'A': return "10000001"; case 'B': return "10000010"; case 'C': return "10000011";
    case 'D': return "10000100"; case 'E': return "10000101"; case 'F': return "10000110";
    case 'G': return "10000111"; case 'H': return "10001000"; case 'I': return "10001001";
    case 'J': return "10001010"; case 'K': return "10001011"; case 'L': return "10001100";
    case 'M': return "10001101"; case 'N': return "10001110"; case 'O': return "10001111";
    case 'P': return "10010000"; case 'Q': return "10010001"; case 'R': return "10010010";
    case 'S': return "10010011"; case 'T': return "10010100"; case 'U': return "10010101";
    case 'V': return "10010110"; case 'W': return "10010111"; case 'X': return "10011000";
    case 'Y': return "10011001"; case 'Z': return "10011010";

    // Digits
    case '0': return "11100000"; case '1': return "11100001"; case '2': return "11100010";
    case '3': return "11100100"; case '4': return "11101000"; case '5': return "11110000";
    case '6': return "11110001"; case '7': return "11110010"; case '8': return "11110100";
    case '9': return "11111000";

    // Space and symbols
    case ' ': return "11101110";  // space
    case '.': return "11101100";
    case ',': return "11101010";
    case '!': return "11110110";
    case '?': return "11111010";
    case ':': return "11101111";
    case ';': return "11111100";

    default:  return "11111111";  // undefined
  }
}

void showConnectingAnimation() {
  for (int i = 0; i < 3; i++) {
    Serial.print("📡");
    delay(300);
    Serial.print("📶");
    delay(300);
    Serial.print("🔁");
    delay(300);
    Serial.print("\r               \r"); // clear line
  }
  Serial.println("📡 Connected. Ready for input!");
}

void setup() {
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, LOW);
  Serial.begin(9600);
  Serial.println("📡 Initializing transmitter...");
  showConnectingAnimation();
  Serial.println("💬 Enter text (A–Z, 0–9, space, .,!?;:):");
}

void loop() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    if (msg.length() == 0) return;

    Serial.println("⏳ Preparing message...");
    delay(500);
    Serial.print("📤 Message queued: ");
    Serial.println(msg);
    Serial.println("🚀 Transmitting...");

    for (int i = 0; i < msg.length(); i++) {
      String bits = encodeChar(toupper(msg[i]));
      Serial.print("⚙️ Char ");
      Serial.print(msg[i]);
      Serial.print(" -> ");
      Serial.println(bits);

      for (int b = 0; b < bits.length(); b++) {
        if (bits[b] == '1') {
          digitalWrite(laserPin, HIGH);
          Serial.print("💡");
        } else {
          digitalWrite(laserPin, LOW);
          Serial.print("💤");
        }
        delay(bitDelay);
      }

      digitalWrite(laserPin, LOW);
      delay(gapDelay);
      Serial.println();
    }

    digitalWrite(laserPin, LOW);
    Serial.println("✅ Transmission complete!");
    Serial.print("📨 Complete message: ");
    Serial.println(msg);
    Serial.println("🔚 End of message");
  }
}
