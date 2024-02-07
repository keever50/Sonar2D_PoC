#include <stdio.h>
#include <logging.h>
#include <Arduino.h>

void logging_log(int severity, const char* msg)
{
    Serial.println(msg);
}