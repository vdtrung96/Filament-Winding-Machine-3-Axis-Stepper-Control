// ===============================
// Motor 1 Continuous Control
// Real-time direction + RPM
// ===============================

#define STEP1 2
#define DIR1  3
#define ENA1  4

int stepsPerRevolution = 200;
int microstep = 8;

float rpm = 20;

bool motorRunning = true;
bool direction = true;

unsigned long stepInterval;
unsigned long lastStepTime = 0;


// ===============================
// Calculate Step Interval
// ===============================

unsigned long calculateStepInterval(float rpmValue)
{
  long totalSteps = stepsPerRevolution * microstep;
  float stepsPerSecond = (rpmValue * totalSteps) / 60.0;
  return (unsigned long)(1000000.0 / stepsPerSecond);
}


// ===============================
// Setup
// ===============================

void setup()
{
  Serial.begin(9600);

  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(ENA1, OUTPUT);

  digitalWrite(ENA1, HIGH);   // disable driver
  digitalWrite(STEP1, LOW);

  digitalWrite(DIR1, direction);

  delay(10);

  digitalWrite(ENA1, LOW);    // enable driver

  stepInterval = calculateStepInterval(rpm);

  Serial.println("Motor Ready");
  Serial.println("Commands:");
  Serial.println("F = Forward");
  Serial.println("B = Reverse");
  Serial.println("S = Stop");
  Serial.println("R = Run");
  Serial.println("Mxxx = set RPM (example M120)");
}


// ===============================
// Loop
// ===============================

void loop()
{

  // ===== SERIAL COMMAND =====

  if (Serial.available())
  {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "F")
    {
      direction = true;
      digitalWrite(DIR1, direction);
      Serial.println("Direction: Forward");
    }

    else if (cmd == "B")
    {
      direction = false;
      digitalWrite(DIR1, direction);
      Serial.println("Direction: Reverse");
    }

    else if (cmd == "S")
    {
      motorRunning = false;
      Serial.println("Motor Stopped");
    }

    else if (cmd == "R")
    {
      motorRunning = true;
      Serial.println("Motor Running");
    }

    else if (cmd.startsWith("M"))
    {
      rpm = cmd.substring(1).toFloat();
      stepInterval = calculateStepInterval(rpm);

      Serial.print("RPM = ");
      Serial.println(rpm);
    }
  }


  // ===== MOTOR STEP =====

  if (motorRunning)
  {
    unsigned long currentTime = micros();

    if (currentTime - lastStepTime >= stepInterval)
    {
      lastStepTime = currentTime;

      digitalWrite(STEP1, HIGH);
      delayMicroseconds(3);
      digitalWrite(STEP1, LOW);
    }
  }
}