// --- LASER RECEIVER (Multi-char + Live Output + Full Message) ---
// LM393 OUT -> D2 (LOW = laser ON = logic '1')

const int sensorPin = 2;
const int bitDelay = 30; // ms per bit (must match transmitter)
const int bitsPerChar = 8;
const int gapDelay = 3 * bitDelay; // must match transmitter

// Binary lookup decoder
char decodeBits(String bits) {
  // Letters
  if (bits == "10000001") return 'A'; if (bits == "10000010") return 'B'; if (bits == "10000011") return 'C';
  if (bits == "10000100") return 'D'; if (bits == "10000101") return 'E'; if (bits == "10000110") return 'F';
  if (bits == "10000111") return 'G'; if (bits == "10001000") return 'H'; if (bits == "10001001") return 'I';
  if (bits == "10001010") return 'J'; if (bits == "10001011") return 'K'; if (bits == "10001100") return 'L';
  if (bits == "10001101") return 'M'; if (bits == "10001110") return 'N'; if (bits == "10001111") return 'O';
  if (bits == "10010000") return 'P'; if (bits == "10010001") return 'Q'; if (bits == "10010010") return 'R';
  if (bits == "10010011") return 'S'; if (bits == "10010100") return 'T'; if (bits == "10010101") return 'U';
  if (bits == "10010110") return 'V'; if (bits == "10010111") return 'W'; if (bits == "10011000") return 'X';
  if (bits == "10011001") return 'Y'; if (bits == "10011010") return 'Z';

  // Digits
  if (bits == "11100000") return '0'; if (bits == "11100001") return '1';
  if (bits == "11100010") return '2'; if (bits == "11100100") return '3';
  if (bits == "11101000") return '4'; if (bits == "11110000") return '5';
  if (bits == "11110001") return '6'; if (bits == "11110010") return '7';
  if (bits == "11110100") return '8'; if (bits == "11111000") return '9';

  // Space and symbols
  if (bits == "11101110") return ' ';
  if (bits == "11101100") return '.';
  if (bits == "11101010") return ',';
  if (bits == "11110110") return '!';
  if (bits == "11111010") return '?';
  if (bits == "11101111") return ':';
  if (bits == "11111100") return ';';

  return '?';
}

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  Serial.println("📡 Receiver ready. Waiting for signal...");
}

void loop() {
  // Wait until laser (LOW) is detected
  if (digitalRead(sensorPin) == LOW) {
    Serial.println("📡 Signal detected!");
    Serial.println("🔗 Sync established");
    Serial.println("📥 Receiving live...");

    String bits = "";
    String decodedMessage = "";

    while (true) {
      // Wait for start of bit sequence
      while (digitalRead(sensorPin) == HIGH);

      bits = "";
      delay(bitDelay / 2); // center sampling for stable timing

      for (int i = 0; i < bitsPerChar; i++) {
        int val = digitalRead(sensorPin);
        bits += (val == LOW) ? '1' : '0';
        delay(bitDelay);
      }

      Serial.print("⚙️ Bits: ");
      Serial.println(bits);

      char ch = decodeBits(bits);
      decodedMessage += ch;

      Serial.print("🧩 Char: ");
      Serial.println(ch);

      // Detect end of message (gap)
      unsigned long gapStart = millis();
      while (digitalRead(sensorPin) == HIGH) {
        if (millis() - gapStart > gapDelay) {
          Serial.println("✅ Transmission complete!");
          Serial.print("📜 Full message: ");
          Serial.println(decodedMessage);
          Serial.println("🔚 End of message.\n");
          delay(1000);
          return;
        }
      }
    }
  }
}
