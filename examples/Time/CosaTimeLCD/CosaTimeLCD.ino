/**
 * @file CosaTimeLCD.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014-2015, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * @section Description
 * Cosa Time and LCD demo with internal RTC.
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/Time.hh"
#include "Cosa/RTC.hh"
#include "Cosa/AnalogPin.hh"
#include "Cosa/Watchdog.hh"
#include "Cosa/IOStream.hh"

// Select port type to use with the LCD device driver.
// LCD and communication port
#include <HD44780.h>

// HD44780 driver built-in adapters
HD44780::Port4b port;
// HD44780::SR3W port;
// HD44780::SR3WSPI port;
// HD44780::SR4W port;

// I2C expander io port based adapters
// #include <PCF8574.h>
// #include <MJKDZ_LCD_Module.h>
// MJKDZ_LCD_Module port;
// MJKDZ_LCD_Module port(0);
// #include <GY_IICLCD.h>
// GY_IICLCD port;
// #include <DFRobot_IIC_LCD_Module.h>
// DFRobot_IIC_LCD_Module port;
// #include <SainSmart_LCD2004.h>
// SainSmart_LCD2004 port;

// HD44780 based LCD with support for serial communication
// #include <ERM1602_5.h>
// ERM1602_5 port;

// HD44780 variants; 16X1, 16X2, 16X4, 20X4, default 16X2
// HD44780 lcd(&port, 20, 4);
// HD44780 lcd(&port, 16, 4);
HD44780 lcd(&port);

// Bind the lcd to an io-stream
IOStream cout(&lcd);

// Start time (set to 30 seconds before New Years 2014)
clock_t epoch;

void setup()
{
  RTC::begin();
  Watchdog::begin();
  lcd.begin();
  cout << PSTR("CosaTimeLCD: started");

  // Set start time
  time_t now;
  now.seconds = 0x30;
  now.minutes = 0x59;
  now.hours = 0x23;
  now.date = 0x31;
  now.month = 0x12;
  now.year = 0x13;
  epoch = now;
  RTC::time(epoch);
}

void loop()
{
  // Read internal RTC time
  clock_t clock = RTC::time();
  time_t now(clock);

  // First line with date and seconds since epoch. Use BCD format output
  cout << clear
       << bcd << now.year << '-'
       << bcd << now.month << '-'
       << bcd << now.date << ' '
       << clock - epoch;

  // Second line with time and battery status
  lcd.set_cursor(0, 1);
  cout << bcd << now.hours << ':'
       << bcd << now.minutes << ':'
       << bcd << now.seconds << ' '
       << AnalogPin::bandgap(1100) << PSTR(" mV");

  // Take a nap until seconds update
  while (clock == RTC::time()) yield();
}
