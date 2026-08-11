#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

Servo s12, s11, s10;
MPU6050 mpu;

const int joyY = A1;
const int joyX = A0;
const int joyZ = A2;
const int throttlePin = A3;
const int buttonPin = 2;

float fY, fX, fZ;
int cY, centerX, centerZ;

float alpha = 0.35;
int deadzone = 120;

int l12 = 90;
int l11 = 90;
int l10 = 90;

int16_t ax, ay, az;
int16_t gx, gy, gz;

float roll, pitch, yawRate;

void setup() {
    s12.attach(12);
    s11.attach(11);
    s10.attach(10);

    s12.write(90);
    s11.write(90);
    s10.write(90);

    Serial.begin(9600);
    pinMode(buttonPin, INPUT_PULLUP);

    Wire.begin();
    mpu.initialize();

    delay(1000);

    long sumY = 0, sumX = 0, sumZ = 0;
    for (int i = 0; i < 200; i++) {
        sumY += analogRead(joyY);
        sumX += analogRead(joyX);
        sumZ += analogRead(joyZ);
        delay(5);
    }

    cY = sumY / 200;
    centerX = sumX / 200;
    centerZ = sumZ / 200;

    fY = cY;
    fX = centerX;
    fZ = centerZ;
}

void loop() {
    int rY = analogRead(joyY);
    int rX = analogRead(joyX);
    int rZ = analogRead(joyZ);
    int throttle = analogRead(throttlePin);
    int btn = digitalRead(buttonPin);

    fY = (alpha * rY) + (1 - alpha) * fY;
    fX = (alpha * rX) + (1 - alpha) * fX;
    fZ = (alpha * rZ) + (1 - alpha) * fZ;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    roll  = atan2(ay, az) * 180 / PI;
    pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;
    yawRate = gz / 131.0;

    // ========================================================
    // FIXED SERIAL FORMAT: MATCHES THE PYTHON PARSER EXACTLY
    // ========================================================
    Serial.print("JOY X:");
    Serial.print(rX);
    Serial.print(" Y:");
    Serial.print(rY);
    Serial.print(" Z:");
    Serial.print(rZ);
    Serial.print(" T:");
    Serial.print(throttle);
    Serial.print(" B:");
    Serial.print(btn);
    Serial.print(" || MPU Roll:");
    Serial.print(roll);
    Serial.print(" Pitch:");
    Serial.print(pitch);
    Serial.print(" Yaw:");
    Serial.println(yawRate);

    // Servo Logic
    int o12 = 90, o11 = 90, o10 = 90;
    int dY = abs(fY - cY);
    int dX = abs(fX - centerX);
    int dZ = abs(fZ - centerZ);

    if (dY < deadzone && dX < deadzone && dZ < deadzone) {
        o12 = 90; o11 = 90; o10 = 90;
    }
    else if (dZ >= dY && dZ >= dX) {
        int move = map(fZ, 0, 1023, -70, 70);
        o12 = 90 + move; o10 = 90 + move; o11 = 90 - move;
    }
    else if (dY >= dX) {
        if (fY < cY) {
            int frontMove = map(fY, 0, cY, 180, 90);
            int rearMove  = map(fY, 0, cY, 0, 90);
            o12 = rearMove; o11 = rearMove; o10 = frontMove;
        } else {
            int frontMove = map(fY, cY, 1023, 90, 0);
            int rearMove  = map(fY, cY, 1023, 90, 180);
            o12 = rearMove; o11 = rearMove; o10 = frontMove;
        }
    }
    else {
        int moveX = map(fX, 0, 1023, -70, 70);
        o12 = 90 + moveX; o11 = 90 - moveX; o10 = 90 - moveX;
    }

    if (abs(o12 - l12) > 4) { l12 = l12 + (o12 > l12 ? 3 : -3); s12.write(l12); }
    if (abs(o11 - l11) > 4) { l11 = l11 + (o11 > l11 ? 3 : -3); s11.write(l11); }
    if (abs(o10 - l10) > 4) { l10 = l10 + (o10 > l10 ? 3 : -3); s10.write(l10); }

    delay(10);
}