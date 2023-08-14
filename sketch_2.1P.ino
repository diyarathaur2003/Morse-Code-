// Define a constant for the maximum expected name length
const int maxNameLength = 50;

// Define the pin number for the push button
int pushButton = 3;

// Initialize the state of the push button to HIGH (not pressed)
int pushButtonState = HIGH;

// Flag to track whether the button is pressed
bool isButtonPressed = false;

// Flag to track if the name has been entered
bool nameEntered = false;

// Setup function runs once at the start
void setup() {
  // Set the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pushButton, INPUT_PULLUP);

  // Initialize Serial communication with a baud rate of 9600
  Serial.begin(9600);
}

// Function to create a "dot" in Morse code
void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

// Function to create a "dash" in Morse code
void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

// Function to blink Morse code for a given letter
void blinkLetter(char letter) {
  // Convert the letter to uppercase
  char uppercaseChar = toupper(letter);

  // Check if the character is a valid letter (A-Z)
  if (uppercaseChar >= 'A' && uppercaseChar <= 'Z') {
    // Calculate the index in the Morse code array
    int index = uppercaseChar - 'A';

    // Define an array of Morse code sequences for each letter
    const char *morseCode[] = {
      ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
      "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
      "..-", "...-", ".--", "-..-", "-.--", "--.."
    };

    // Call the function 
    blinkMorseCode(morseCode[index]);
  }
}

// Function to blink a Morse code sequence
void blinkMorseCode(const char *morseCode) {
  // Loop through each symbol in the Morse code sequence
  for (int i = 0; morseCode[i] != '\0'; i++) {
    // Check if the symbol is a dot or a dash and call the corresponding function
    if (morseCode[i] == '.') {
      dot();
      delay(500);
    } else if (morseCode[i] == '-') {
      dash();
      delay(500);
    }
  }
}

// Loop function runs repeatedly after setup
void loop() {
  // Read the state of the push button
  pushButtonState = digitalRead(pushButton);

  // Check if the push button is pressed and not previously detected
  if (pushButtonState == LOW && !isButtonPressed) {
    isButtonPressed = true;
    Serial.println("Enter your name:");
    nameEntered = false;
  } else if (pushButtonState == HIGH && isButtonPressed) {
    // Reset the button press detection when the button is released
    isButtonPressed = false;
    nameEntered = true;
  }

  // Check if the name has been entered
  if (nameEntered) {
    // Wait for serial input to become available
    if (!Serial.available()) {
      return;  // Continue waiting
    }

    // Read the input string from Serial and convert to uppercase
    String input = Serial.readString();
    input.toUpperCase();

    // Display the entered name
    Serial.println("Your name: " + input);

    // Dynamically allocate memory for the name array
    char name[maxNameLength + 1];
    input.toCharArray(name, maxNameLength);

    // Loop through each character in the name and blink Morse code
    for (int i = 0; name[i] != '\0'; i++) {
      blinkLetter(name[i]);
      delay(1500);  // Delay between letters
    }

    // Reset the nameEntered flag
    nameEntered = false;
  }
}
