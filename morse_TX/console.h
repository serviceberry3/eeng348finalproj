  
/*
 * console.h
 *
 *  Created on: Jan 15, 2021
 *      Author: nodog
 */

#ifndef INCLUDE_CONSOLE_H_
#define INCLUDE_CONSOLE_H_

#include "lcd.h"
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define push1 8
#define push2 2

static const int RXPin = 4, TXPin = 3; //rxpin should connect to pin labeled tx on gps module, and vice versa
static const uint32_t GPSBaud = 9600; //GPS data rate

const String A = ".-", B = "-...", C = "-.-.", D = "-..",
             E = ".", F = "..-.", G = "--.", H = "....",
             I = "..", J = ".---", K = "-.-", L = ".-..",
             M = "--", N = "-.", O = "---", P = ".--.",
             Q = "--.-", R = ".-.", S = "...", T = "-",
             U = "..-", V = "...-", W = ".--", X = "-..-",
             Y = "-.--", Z = "--..";

void open_console(LiquidCrystal_I2C lcd);
void key_processor(LiquidCrystal_I2C lcd);
void setup_periphs();


#endif /* INCLUDE_CONSOLE_H_ */