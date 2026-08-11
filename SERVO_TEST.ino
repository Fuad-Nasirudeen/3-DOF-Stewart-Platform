#include <Servo.h>

Servo s12, s11, s10;

const int joyY = A1;
const int joyX = A0;
const int joyZ = A2; // Using A2 for forward/backward tilt

float fY, fX, fZ;
int cY, centerX, centerZ;

float alpha = 0.2;   // ✈️ Smoother for "Plane" feel
float speedLimit = 2.5; // Controls the "weight" of the plane
int deadzone = 50;   // Reduced to make it more responsive

float l12 = 90, l11 = 90, l10 = 90;

void setup() {
    s12.attach(12);
    s11.attach(11);
    s10.attach(10);

    s12.write(90);
    s11.write(90);
    s10.write(90);

    delay(1000);

    long sumY = 0, sumX = 0, sumZ = 0;
    for (int i = 0; i < 200; i++) {
        sumY += analogRead(joyY);
        sumX += analogRead(joyX);
        sumZ += analogRead(joyZ);
        delay(2);
    }
    cY = sumY / 200;
    centerX = sumX / 200;
    centerZ = sumZ / 200;

    fY = cY; fX = centerX; fZ = centerZ;
}

void loop() {
    int rY = analogRead(joyY);
    int rX = analogRead(joyX);
    int rZ = analogRead(joyZ);

    fY = (alpha * rY) + (1.0 - alpha) * fY;
    fX = (alpha * rX) + (1.0 - alpha) * fX;
    fZ = (alpha * rZ) + (1.0 - alpha) * fZ;

    float o12 = 90, o11 = 90, o10 = 90;

    // ✈️ PLANE LOGIC: Prioritize Tilt (Z-Axis)
    if (abs(fZ - centerZ) > deadzone) {
        // Map Z-axis to a tilt angle
        int tilt = map(fZ, 0, 1023, -50, 50); 
        o12 = 90 + tilt;
        o10 = 90 + tilt;
        o11 = 90 - tilt; 
    } 
    else if (abs(fX - centerX) > deadzone) {
        int roll = map(fX, 0, 1023, -40, 40);
        o12 = 90 + roll;
        o11 = 90 - roll;
        o10 = 90 - roll;
    }
    else if (abs(fY - cY) > deadzone) {
        o12 = o11 = o10 = map(fY, 0, 1023, 150, 30);
    }

    // ✈️ SMOOTH STEPPING (Stops Jitter)
    if (abs(o12 - l12) > 1.0) {
        l12 += (o12 > l12) ? speedLimit : -speedLimit;
        s12.write((int)l12);
    }
    if (abs(o11 - l11) > 1.0) {
        l11 += (o11 > l11) ? speedLimit : -speedLimit;
        s11.write((int)l11);
    }
    if (abs(o10 - l10) > 1.0) {
        l10 += (o10 > l10) ? speedLimit : -speedLimit;
        s10.write((int)l10);
    }

    delay(15); 
}