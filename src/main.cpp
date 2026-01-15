#include <Arduino.h>

/// ===== LINE SENSOR PINS =====
#define LEFT_SENSOR   2
#define MIDDLE_SENSOR 4
#define RIGHT_SENSOR  10

/// ===== ULTRASONIC SENSOR PINS =====
#define TRIG_PIN A5    // Changed to A5
#define ECHO_PIN A1    // Changed to A1

/// ===== BUTTON PINS =====
#define BUTTON_1_PIN 3    // Line following mode
#define BUTTON_2_PIN A0   // Obstacle avoidance mode

/// ===== L298N MOTOR PINS (PWM) =====
// Left motor (Motor A)
#define ENA 6     // PWM
#define IN1 11
#define IN2 9

// Right motor (Motor B)
#define ENB 5     // PWM
#define IN3 8
#define IN4 7

/// ===== SPEED SETTINGS =====
#define MOTOR_SPEED 150
#define TURN_SPEED 100
#define BACKUP_SPEED 100
#define AVOID_SPEED 130

/// ===== ULTRASONIC SETTINGS =====
#define OBSTACLE_DISTANCE 15  // cm - distance to detect obstacle
#define BACKUP_DISTANCE 8     // cm - distance to backup
#define BACKUP_TIME 500       // ms - time to backup
#define TURN_TIME 300         // ms - time to turn during avoidance

/// ===== OPERATION MODES =====
enum Mode {
  MODE_IDLE,
  MODE_LINE_FOLLOWING,
  MODE_OBSTACLE_AVOIDANCE
};

Mode currentMode = MODE_IDLE;

// ===== FUNCTION DECLARATIONS =====
void forward();
void backward();
void turnLeft();
void turnRight();
void stopMotors();
void setMotorSpeed(int leftSpeed, int rightSpeed);
void checkLineSensors();
void checkButtons();
float getDistance();
void obstacleAvoidance();
void backupAndTurn();

void setup() {
  Serial.begin(9600);

  // Line sensors
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(MIDDLE_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Buttons
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
  Serial.println("System started. Press Button 1 for Line Following, Button 2 for Obstacle Avoidance");
}

void loop() {
  checkButtons();
  
  switch(currentMode) {
    case MODE_LINE_FOLLOWING:
      checkLineSensors();
      break;
      
    case MODE_OBSTACLE_AVOIDANCE:
      obstacleAvoidance();
      break;
      
    case MODE_IDLE:
    default:
      stopMotors();
      break;
  }
  
  delay(50);   // small delay for stability
}

/// ===== BUTTON CHECKING =====
void checkButtons() {
  static bool lastButton1 = HIGH;
  static bool lastButton2 = HIGH;
  
  bool currentButton1 = digitalRead(BUTTON_1_PIN);
  bool currentButton2 = digitalRead(BUTTON_2_PIN);
  
  // Button 1 pressed (active LOW due to INPUT_PULLUP)
  if (lastButton1 == HIGH && currentButton1 == LOW) {
    currentMode = MODE_LINE_FOLLOWING;
    Serial.println("Mode: Line Following");
    delay(300); // Debounce delay
  }
  
  // Button 2 pressed (active LOW due to INPUT_PULLUP)
  if (lastButton2 == HIGH && currentButton2 == LOW) {
    currentMode = MODE_OBSTACLE_AVOIDANCE;
    Serial.println("Mode: Obstacle Avoidance");
    delay(300); // Debounce delay
  }
  
  lastButton1 = currentButton1;
  lastButton2 = currentButton2;
}

/// ===== ULTRASONIC SENSOR FUNCTIONS =====
float getDistance() {
  // Clear trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10μs pulse to trigger
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance (cm)
  float distance = duration * 0.034 / 2;
  
  // Return distance or 0 if out of range
  return (distance > 0 && distance < 400) ? distance : 0;
}

void backupAndTurn() {
  Serial.println("Obstacle detected! Backing up and turning...");
  
  // Backup
  backward();
  delay(BACKUP_TIME);
  
  // Turn right
  turnRight();
  delay(TURN_TIME);
  
  stopMotors();
  delay(200);
}

void obstacleAvoidance() {
  float distance = getDistance();
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance > 0 && distance <= OBSTACLE_DISTANCE) {
    // Obstacle detected
    backupAndTurn();
    
    // Check again after turning
    delay(100);
    distance = getDistance();
    
    // If still obstructed, keep turning
    while (distance > 0 && distance <= OBSTACLE_DISTANCE) {
      Serial.println("Still obstructed, turning more...");
      turnRight();
      delay(TURN_TIME);
      stopMotors();
      delay(100);
      distance = getDistance();
    }
  } else {
    // No obstacle, move forward
    Serial.println("Clear path → Forward");
    forward();
    setMotorSpeed(AVOID_SPEED, AVOID_SPEED);
  }
}

/// ===== LINE SENSOR LOGIC =====
void checkLineSensors() {
  int left   = digitalRead(LEFT_SENSOR);
  int middle = digitalRead(MIDDLE_SENSOR);
  int right  = digitalRead(RIGHT_SENSOR);

  // Adjust HIGH / LOW if your sensors are inverted
  if (middle == HIGH ) {
    Serial.println("Middle → Forward");
    forward();
  }
  else if (left == HIGH) {
    Serial.println("Left → Turn Left");
    turnLeft();
  }
  else if (right == HIGH) {
    Serial.println("Right → Turn Right");
    turnRight();
  }
  else {
    Serial.println("No line → Stop");
    stopMotors();
  }
}

/// ===== MOTOR CONTROL =====
void setMotorSpeed(int leftSpeed, int rightSpeed) {
  leftSpeed  = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorSpeed(BACKUP_SPEED, BACKUP_SPEED);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  setMotorSpeed(BACKUP_SPEED, BACKUP_SPEED);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorSpeed(TURN_SPEED, MOTOR_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorSpeed(MOTOR_SPEED, TURN_SPEED);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  setMotorSpeed(0, 0);
}