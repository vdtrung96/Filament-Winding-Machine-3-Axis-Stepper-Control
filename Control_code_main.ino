// ===============================
// 3 Stepper Motors + Potentiometers + HOME FUNCTION
// ===============================

// ===== MOTOR PINS =====
#define STEP1 2
#define DIR1 3
#define ENA1 4

#define STEP2 5
#define DIR2 6
#define ENA2 7

#define STEP3 8
#define DIR3 9
#define ENA3 10

// ===== POTENTIOMETERS =====
#define POT1 A0
#define POT2 A1
#define POT3 A2

int potValue1 = 0;
int potValue2 = 0;
int potValue3 = 0;

// ===== MOTOR SETTINGS =====
int stepsPerRevolution = 200;
int microstep = 8;

float rpm1 = 400;
float rpm2 = 100;
float rpm3 = 1.5;

bool dirMotor1 = true;
bool dirMotor2 = true;
bool dirMotor3 = true;

long stepCountMotor1 = 0;
long stepsToRunMotor1 = 0;

unsigned long stepInterval1;
unsigned long stepInterval2;
unsigned long stepInterval3;

unsigned long lastStepTime1 = 0;
unsigned long lastStepTime2 = 0;
unsigned long lastStepTime3 = 0;

bool stopMotor1 = false;
bool stopMotor2 = false;
bool stopMotor3 = false;


// ===============================
// Calculate Step Interval (SAFE)
// ===============================
unsigned long calculateStepInterval(float rpmValue)
{
    if (rpmValue < 0.1) return 1000000;

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

    pinMode(STEP2, OUTPUT);
    pinMode(DIR2, OUTPUT);
    pinMode(ENA2, OUTPUT);

    pinMode(STEP3, OUTPUT);
    pinMode(DIR3, OUTPUT);
    pinMode(ENA3, OUTPUT);

    pinMode(POT1, INPUT);
    pinMode(POT2, INPUT);
    pinMode(POT3, INPUT);

    digitalWrite(ENA1, HIGH);
    digitalWrite(ENA2, HIGH);
    digitalWrite(ENA3, HIGH);

    digitalWrite(STEP1, LOW);
    digitalWrite(STEP2, LOW);
    digitalWrite(STEP3, LOW);

    digitalWrite(DIR1, dirMotor1);
    digitalWrite(DIR2, dirMotor2);
    digitalWrite(DIR3, dirMotor3);

    delay(20);

    digitalWrite(ENA1, LOW);
    digitalWrite(ENA2, LOW);
    digitalWrite(ENA3, LOW);

    unsigned long now = micros();
    lastStepTime1 = now;
    lastStepTime2 = now;
    lastStepTime3 = now;

    int revolutions = 8;
    stepsToRunMotor1 = (long)stepsPerRevolution * microstep * revolutions;

    stepInterval1 = calculateStepInterval(rpm1);
    stepInterval2 = calculateStepInterval(rpm2);
    stepInterval3 = calculateStepInterval(rpm3);

    Serial.println("================================");
    Serial.println("3 Motor + Potentiometer Control Ready");
    Serial.println("Press S to HOME Motor1");
    Serial.println("================================");
}


// ===============================
// Loop
// ===============================
void loop()
{
    unsigned long currentTime = micros();

    // ===== SERIAL COMMAND =====
    if (Serial.available())
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        // ===== STOP + HOME =====
        if (cmd == "S")
        {
            stopMotor1 = true;
            stopMotor2 = true;
            stopMotor3 = true;

            digitalWrite(ENA2, HIGH);
            digitalWrite(ENA3, HIGH);

            digitalWrite(DIR1, LOW);

            Serial.println("Motor2 & Motor3 STOP");
            Serial.println("Motor1 returning HOME...");
        }

        // ===== RUN AGAIN =====
        else if (cmd == "R")
        {
            stopMotor1 = false;
            stopMotor2 = false;
            stopMotor3 = false;

            digitalWrite(ENA1, LOW);
            digitalWrite(ENA2, LOW);
            digitalWrite(ENA3, LOW);

            digitalWrite(DIR1, dirMotor1);

            unsigned long now = micros();
            lastStepTime1 = now;
            lastStepTime2 = now;
            lastStepTime3 = now;

            Serial.println("All motors RUNNING again");
        }
    }

    // ===== READ POTENTIOMETERS =====
    potValue1 = analogRead(POT1);
    potValue2 = analogRead(POT2);
    potValue3 = analogRead(POT3);

    float newRpm1 = map(potValue1, 0, 1023, 0, 100);
    float newRpm2 = map(potValue2, 0, 1023, 0, 50);
    float newRpm3 = map(potValue3, 0, 1023, 0, 10);

    // ===== SMOOTH =====
    rpm1 = 0.8 * rpm1 + 0.2 * newRpm1;
    rpm2 = 0.8 * rpm2 + 0.2 * newRpm2;
    rpm3 = 0.8 * rpm3 + 0.2 * newRpm3;

    // ===== DEAD ZONE =====
    if (rpm1 < 2) rpm1 = 0;
    if (rpm2 < 2) rpm2 = 0;
    if (rpm3 < 1) rpm3 = 0;

    // ===== LIMIT =====
    rpm1 = constrain(rpm1, 0, 80);
    rpm2 = constrain(rpm2, 0, 40);
    rpm3 = constrain(rpm3, 0, 10);

    // ===== UPDATE INTERVAL =====
    stepInterval1 = calculateStepInterval(rpm1);
    stepInterval2 = calculateStepInterval(rpm2);
    stepInterval3 = calculateStepInterval(rpm3);


    // ===== MOTOR 1 =====
    if (!stopMotor1)
    {
        if (currentTime - lastStepTime1 >= stepInterval1)
        {
            lastStepTime1 = currentTime;

            digitalWrite(STEP1, HIGH);
            delayMicroseconds(3);
            digitalWrite(STEP1, LOW);

            stepCountMotor1++;

            if (stepCountMotor1 >= stepsToRunMotor1)
            {
                dirMotor1 = !dirMotor1;
                digitalWrite(DIR1, dirMotor1);
                stepCountMotor1 = 0;
            }
        }
    }
    else
    {
        // ===== RETURN HOME =====
        if (stepCountMotor1 > 0)
        {
            if (currentTime - lastStepTime1 >= stepInterval1)
            {
                lastStepTime1 = currentTime;

                digitalWrite(STEP1, HIGH);
                delayMicroseconds(3);
                digitalWrite(STEP1, LOW);

                stepCountMotor1--;
            }
        }
        else
        {
            digitalWrite(ENA1, HIGH);
        }
    }

    // ===== MOTOR 2 =====
    if (!stopMotor2)
    {
        if (currentTime - lastStepTime2 >= stepInterval2)
        {
            lastStepTime2 = currentTime;

            digitalWrite(STEP2, HIGH);
            delayMicroseconds(3);
            digitalWrite(STEP2, LOW);
        }
    }

    // ===== MOTOR 3 =====
    if (!stopMotor3)
    {
        if (currentTime - lastStepTime3 >= stepInterval3)
        {
            lastStepTime3 = currentTime;

            digitalWrite(STEP3, HIGH);
            delayMicroseconds(3);
            digitalWrite(STEP3, LOW);
        }
    }
}