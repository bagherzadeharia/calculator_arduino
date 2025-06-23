#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const byte ROWS = 4;
const byte COLS = 6;
char keys[ROWS][COLS] = {
  {'o', '7', '8', '9', '*', '/'},
  {'N', '4', '5', '6', '-', 'M'},
  {'%', '1', '2', '3', '+', 'm'},
  {'R', '0', '.', '=', '+', 'p'} 
};
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {0, 1, 2, 3, 4, 5};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String currentInput = "0";
String previousInput = "";
char currentOperation = ' ';
double memoryValue = 0.0;
bool newOperand = true;

void updateDisplay();
void displayOnLcd(String line1, String line2 = "", int delayMs = 0);
void processDigit(char digit);
void processOperator(char op);
void processEquals();
void processClear();
void processSpecialFunction(char funcKey);
void handleMemory(char memKey);
double calculate(double num1, double num2, char op);
String formatFloat(double num, int precision = 4);

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);
  displayOnLcd("Smart Calc", "Ready...", 1500);
  processClear();
  updateDisplay();
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    if (isdigit(key) || key == '.') {
      processDigit(key);
    } else if (strchr("+-*/", key)) {
      processOperator(key);
    } else if (key == '=') {
      processEquals();
    } else if (key == 'o') {
      processClear();
    } else if (key == 'N' || key == '%' || key == 'R') {
      processSpecialFunction(key);
    } else if (key == 'M' || key == 'm' || key == 'p') {
      handleMemory(key);
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

  if (currentInput.length() < LCD_COLS - 1) currentInput += digit;
}

void processOperator(char op) {
  if (currentInput.length() > 0 && !newOperand) {
    if (previousInput.length() > 0 && currentOperation != ' ') {
      processEquals();
      previousInput = currentInput;
      currentInput = "";
    } else {
      previousInput = currentInput;
      currentInput = "";
    }
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
    previousInput = "";
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
  if (currentInput == "0") return;

  double num = currentInput.toFloat();
  double result = 0.0;

  switch (funcKey) {
    case 'N':
      currentInput = (currentInput.startsWith("-")) ? currentInput.substring(1) : "-" + currentInput;
      return;
    case '%':
      result = num / 100.0;
      break;
    case 'R':
      if (num >= 0) {
        result = sqrt(num);
      } else {
        displayOnLcd("Error:", "Neg Sqrt", 1000);
        return;
      }
      break;
  }
  currentInput = formatFloat(result);
  newOperand = true;
}

void handleMemory(char memKey) {
  double val = currentInput.toFloat();
  switch (memKey) {
    case 'p': memoryValue += val; break;
    case 'm': memoryValue -= val; break;
    case 'M':
      if (!newOperand && val == memoryValue) {
        memoryValue = 0.0;
        displayOnLcd("Memory Cleared", "", 1000);
      } else {
        currentInput = formatFloat(memoryValue);
      }
      break;
  }
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
    default: return num2;
  }
}

void updateDisplay() {
  lcd.clear();
  String line1 = (previousInput.length() > 0) ? previousInput + currentOperation : "";
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

