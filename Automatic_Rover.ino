#include <Servo.h>

// ===== Pins =====
#define TRIG_PIN A1
#define ECHO_PIN A2
#define SERVO_PIN 10

// Motor driver (L293D breakout)
#define IN1 2
#define IN2 3
#define IN3 4
#define IN4 7

Servo servo;

// ===== Parameters =====
const int SERVO_LEFT = 180;
const int SERVO_CENTER = 90;
const int SERVO_RIGHT = 0;
const int obstacleDistance = 20; // cm

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  servo.attach(SERVO_PIN);
  servo.write(SERVO_CENTER);

  Serial.println("Obstacle Avoiding Car Ready!");
}

// ===== Motor Control =====
void forward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void leftTurn() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rightTurn() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ===== Ultrasonic Distance =====
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  return distance;
}

// ===== Main Loop =====
void loop() {
  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < obstacleDistance) {
    stopMotor();
    delay(100);

    servo.write(SERVO_LEFT);
    delay(400);
    long leftDist = getDistance();

    servo.write(SERVO_RIGHT);
    delay(400);
    long rightDist = getDistance();

    servo.write(SERVO_CENTER);
    delay(200);

    if (leftDist > rightDist) {
      leftTurn();
      delay(500);
    } else {
      rightTurn();
      delay(500);
    }
  } else {
    forward();
  }

  delay(50);
}
