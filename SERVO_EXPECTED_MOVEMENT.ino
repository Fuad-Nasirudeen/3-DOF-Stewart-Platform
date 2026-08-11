#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

void setup()
{
    servo1.attach(12);
    servo2.attach(11);
    servo3.attach(10);

    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
}

void loop()
{
   //Y AXIS
   servo1.write(180);
    servo2.write(180);
    servo3.write(180);

    delay(5000);
     servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(5000);

     servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    delay(5000);

     servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(5000);

    //X AXIS
    servo1.write(180);
    servo2.write(0);
    servo3.write(0);
    delay(5000);

     servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(5000);

    servo1.write(0);
    servo2.write(180);
    servo3.write(180);
    delay(5000);

    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(5000);

    //Z AXIS
    servo1.write(0);
    servo2.write(180);
    servo3.write(0);
    delay(5000);

     servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(5000);

    servo1.write(180);
    servo2.write(0);
    servo3.write(180);
    delay(5000);

    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    delay(5000);
}