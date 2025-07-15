#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int LCD_COLS = 16, LCD_ROWS = 2;
const byte ROWS_BASIC = 4, COLS_BASIC = 6;
const byte ROWS_SCI = 4, COLS_SCI = 4;

char keysBasic[ROWS_BASIC][COLS_BASIC] = {
  {'o', '7', '8', '9', '*', '/'},
  {'N', '4', '5', '6', '-', 'M'},
  {'%', '1', '2', '3', '+', 'v'},
  {'R', '0', '.', '=', '+', '^'}
};
byte rowPinsBasic[ROWS_BASIC] = {A0, A1, A2, A3};
byte colPinsBasic[COLS_BASIC] = {0, 1, 2, 3, 4, 5};

char keysSci[ROWS_SCI][COLS_SCI] = {
  {'S', 'C', 'T', 'G'},
  {'l', 'n', 'e', '!'},
  {'D', 'P', 'B', 'H'},
  {'I', 'O', 'A', 'M'}
};
byte rowPinsSci[ROWS_SCI] = {A0, A1, A2, A3};
byte colPinsSci[COLS_SCI] = {0, 1, 2, 3};

Keypad keypadBasic = Keypad(makeKeymap(keysBasic), rowPinsBasic, colPinsBasic, ROWS_BASIC, COLS_BASIC);
Keypad keypadSci = Keypad(makeKeymap(keysSci), rowPinsSci, colPinsSci, ROWS_SCI, COLS_SCI);

String currentInput = "0";
String previousInput = "";
char currentOperation = ' ';
double memoryValue = 0.0;
bool newOperand = true, isSciMode = false;

void updateDisplay();
void displayOnLcd(String line1, String line2 = "", int delayMs = 0);
void processDigit(char digit);
void processOperator(char op);
void processEquals();
void processClear();
void processSpecialFunction(char funcKey);
void processScientificKey(char key);
double calculate(double num1, double num2, char op);
String decToBin(int decNum);
String decToHex(int decNum);
String formatFloat(double num, int precision = 4);
void toggleMode();

void setup() {
  pinMode(6, OUTPUT);
  lcd.begin(LCD_COLS, LCD_ROWS);
  displayOnLcd("Smart Calc", "Ready...", 1500);
  processClear();
  updateDisplay();
}

void loop() {
  char key = isSciMode ? keypadSci.getKey() : keypadBasic.getKey();
  if (key) {
    if (key == 'M') {
      toggleMode();
    } else if (!isSciMode) {
      if (isdigit(key) || key == '.') processDigit(key);
      else if (strchr("+-*/", key) || key == '%' || key == '^') processOperator(key);
      else if (key == '=') processEquals();
      else if (key == 'o') processClear();
      else if (key == 'N' || key == 'R' || key == 'v' || key == 't') processSpecialFunction(key);
    } else {
      processScientificKey(key);
    }
    updateDisplay();
  }
}

void processDigit(char digit) {
  if (newOperand) {
    currentInput = (digit == '.') ? "0" : "";
    newOperand = false;
  }

  if (digit == '.' && currentInput.indexOf('.') != -1)
    return;

  if (currentInput.length() < LCD_COLS - 1)
    currentInput += digit;
}

void processOperator(char op) {
  if (currentInput.length() > 0 && !newOperand) {
    if (previousInput.length() > 0 && currentOperation != ' ') {
      processEquals();
    } else {
      previousInput = currentInput;
    }
    currentInput = "";
  }
  if (previousInput.length() > 0) {
    currentOperation = op;
    newOperand = true;
  }
}

void processEquals() {
  if (previousInput.length() > 0 && currentInput.length() > 0 && currentOperation != ' ') {
    double num1 = previousInput.toFloat();
    double num2 = currentInput.toFloat();
    double result = calculate(num1, num2, currentOperation);
    currentInput = formatFloat(result);
    previousInput = currentInput;
    currentOperation = ' ';
    newOperand = true;
  }
}

void processClear() {
  currentInput = "0";
  previousInput = "";
  currentOperation = ' ';
  newOperand = true;
}

void processSpecialFunction(char funcKey) {
  if (currentInput == "0")
    return;

  double num = currentInput.toFloat();
  double result = 0.0;

  switch (funcKey) {
    case 'N':
      currentInput = (currentInput.startsWith("-")) ? currentInput.substring(1) : "-" + currentInput;
      return;
    case 'R':
      if (num >= 0) result = sqrt(num);
      else {
        displayOnLcd("Error:", "Neg Sqrt", 1000);
        return;
      }
      break;
    case 'v': 
      if (num != 0.0) {
        result = 1 / num;
        break;
      } else {
        displayOnLcd("Error:", "Invalid Request", 1000);
        return;
      }
    case 't': result = num * num; break;
  }
  currentInput = formatFloat(result);
  previousInput = currentInput;
  currentOperation = ' ';
  newOperand = true;
}

double calculate(double num1, double num2, char op) {
  switch (op) {
    case '+': return num1 + num2;
    case '-': return num1 - num2;
    case '*': return num1 * num2;
    case '/':
      if (num2 != 0) return num1 / num2;
      displayOnLcd("Error:", "Div by Zero", 1500);
      processClear();
      return 0;
    case '%': return (num1 / 100.0) * num2;
    case '^': return pow(num1, num2);
    default: return num2;
  }
}

void processScientificKey(char key) {
  double num = currentInput.toFloat();
  double result = 0.0;

  switch (key) {
    case 'S': result = sin(num); break;
    case 'C': result = cos(num); break;
    case 'T': result = tan(num); break;
    case 'G': result = abs(num); break;
    case 'l':
      if (num > 0) result = log10(num);
      else { displayOnLcd("Error:", "log(x<=0)", 1000); return; }
      break;
    case 'n': result = log(num); break;
    case 'e': result = exp(num); break;
    case '!':
      if (num < 0 || floor(num) != num) {
        displayOnLcd("Error:", "Invalid !", 1000);
        return;
      }
      result = 1;
      for (int i = 1; i <= (int)num; i++) result *= i;
      break;
    case 'D': result = pow(10, num); break;
    case 'P': result = pow(num, 2); break;
    case 'B':
      displayOnLcd(decToBin(num), "", 5000); 
      processClear();
      updateDisplay();
      break;
    case 'H':
      displayOnLcd(decToHex(num), "", 5000); 
      processClear();
      updateDisplay();
      break;
    case 'I': result = asin(num); break;
    case 'O': result = acos(num); break;
    case 'A': result = atan(num); break;
    default:
      displayOnLcd("Unknown Fn", String(key), 1000);
      return;
  }

  currentInput = formatFloat(result);
  newOperand = true;
}

void toggleMode() {
  isSciMode = !isSciMode;
  displayOnLcd(isSciMode ? "Scientific Mode" : "Basic Mode", "", 1000);
  if (isSciMode){
    digitalWrite(6, HIGH);
  } else {
    digitalWrite(6, LOW);
  }
}

String decToBin(int decNum) {
  String binString = "";
  if (decNum == 0) {
    return "0";
  }
  while (decNum > 0) {
    int remainder = decNum % 2;
    binString = String(remainder) + binString;
    decNum = decNum / 2;
  }
  return binString;
}

String decToHex(int decNum) {
  String hexString = "";
  if (decNum == 0) {
    return "0";
  }
  while (decNum != 0) {
    int remainder = decNum % 16;
    char hexDigit;
    if (remainder < 10) {
      hexDigit = remainder + '0';
    } else {
      hexDigit = remainder - 10 + 'A';
    }
    hexString = hexDigit + hexString;
    decNum /= 16;
  }
  return hexString;
}

void updateDisplay() {
  lcd.clear();
  String line1 = (previousInput.length() > 0 && currentOperation != ' ') ? previousInput + currentOperation : "";
  String line2 = currentInput;

  if (line1.length() > LCD_COLS) line1 = line1.substring(line1.length() - LCD_COLS);
  if (line2.length() > LCD_COLS) line2 = line2.substring(line2.length() - LCD_COLS);

  lcd.setCursor(0, 0); lcd.print(line1);
  lcd.setCursor(0, 1); lcd.print(line2);
}

void displayOnLcd(String line1, String line2, int delayMs) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(line1);
  lcd.setCursor(0, 1); lcd.print(line2);
  if (delayMs > 0) delay(delayMs);
}

String formatFloat(double num, int precision) {
  char buf[17];
  dtostrf(num, 1, precision, buf);
  String str = String(buf);
  str.trim();
  return str;
}
