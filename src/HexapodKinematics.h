/**
 * S T E W A R T    P L A T F O R M    O N    E S P 3 2
 *
 * Based on
 * 6dof-stewduino
 * Copyright (C) 2018  Philippe Desrosiers
 * https://github.com/xoxota99/stewy
 *
 * Derived from the work of Daniel Waters
 * https://www.youtube.com/watch?v=1jrP3_1ML9M
 *
 * Modified by Nicolas Jeanmonod
 * ouilogique.com
 * March 2019
 *
 *
 * Copyright (C) 2019  Nicolas Jeanmonod
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef __HEXAPODKINEMATICS_H__
#define __HEXAPODKINEMATICS_H__

#ifdef ESP32
#include <Arduino.h>
#endif

#include "HexapodKinematicsConfig.h"

/*
   There are three axes of symmetry (AXIS1, AXIS2, AXIS3). Looking down on the
   platform from above (along the Y axis), with 0 degrees being the X-positive line, and traveling
   in a CC direction, these are at 30 degrees, 120 degrees, and 240 degrees. All
   the polar coordinates of pivot points, servo centers, etc. are calculated based on
   an axis, and an offset angle (positive or negative theta) from the axis.
 */

#define AXIS1 (PI / 6)  //  30 degrees.
#define AXIS2 (-PI / 2) // -90 degrees.
/*
   NOTE: We make an assumption of mirror symmetry for AXIS3 along the Y axis.
   That is, AXIS1 is at (e.g.) 30 degrees, and AXIS3 will be at 120 degrees
   We account for this by negating the value of x-coordinates generated based
   on this axis later on. This is potentially messy, and should maybe be refactored.
 */
#define AXIS3 AXIS1

/*
   Absolute angle that the servo arm plane of rotation is at (degrees), from the world-X axis.
 */
const double THETA_S_DEG[6] = {
    -60,
    120,
    180,
    0,
    60,
    -120};

const double THETA_S[6] = { //Servo arm angle (radians)
    radians(THETA_S_DEG[0]),
    radians(THETA_S_DEG[1]),
    radians(THETA_S_DEG[2]),
    radians(THETA_S_DEG[3]),
    radians(THETA_S_DEG[4]),
    radians(THETA_S_DEG[5])};

/*
   XY cartesian coordinates of the platform joints, based on the polar
   coordinates (platform radius P_RAD, radial axis AXIS[1|2\3], and offset THETA_P.
   These coordinates are in the plane of the platform itself.
 */
const double P_COORDS[6][2] = {
    {P_RAD * cos(AXIS1 + THETA_P), P_RAD *sin(AXIS1 + THETA_P)},
    {P_RAD * cos(AXIS1 - THETA_P), P_RAD *sin(AXIS1 - THETA_P)},
    {P_RAD * cos(AXIS2 + THETA_P), P_RAD *sin(AXIS2 + THETA_P)},
    {-P_RAD * cos(AXIS2 + THETA_P), P_RAD *sin(AXIS2 + THETA_P)},
    {-P_RAD * cos(AXIS3 - THETA_P), P_RAD *sin(AXIS3 - THETA_P)},
    {-P_RAD * cos(AXIS3 + THETA_P), P_RAD *sin(AXIS3 + THETA_P)}};

/*
   XY cartesian coordinates of the servo centers, based on the polar
   coordinates (base radius B_RAD, radial axis AXIS[1|2\3], and offset THETA_B.
   These coordinates are in the plane of the base itself.
 */
const double B_COORDS[6][2] = {
    {B_RAD * cos(AXIS1 + THETA_B), B_RAD *sin(AXIS1 + THETA_B)},
    {B_RAD * cos(AXIS1 - THETA_B), B_RAD *sin(AXIS1 - THETA_B)},
    {B_RAD * cos(AXIS2 + THETA_B), B_RAD *sin(AXIS2 + THETA_B)},
    {-B_RAD * cos(AXIS2 + THETA_B), B_RAD *sin(AXIS2 + THETA_B)},
    {-B_RAD * cos(AXIS3 - THETA_B), B_RAD *sin(AXIS3 - THETA_B)},
    {-B_RAD * cos(AXIS3 + THETA_B), B_RAD *sin(AXIS3 + THETA_B)}};

/**
 *
 */
class HexapodKinematics
{

private:
  // Setpoints (internal state)
  int _sp_sway = 0,  // sway (x) in mm
      _sp_surge = 0, // surge (y) in mm
      _sp_heave = 0; // heave (z) in mm

  float _sp_pitch = 0, // pitch (x) in radians
      _sp_roll = 0,    // roll (y) in radians
      _sp_yaw = 0;     // yaw (z) in radians

public:
  int8_t home(float *servoValues);
  int8_t moveTo(float *servoValues, int sway, int surge, int heave, float pitch, float roll, float yaw);
  int8_t moveTo(float *servoValues, float pitch, float roll);

  int getSway();
  int getSurge();
  int getHeave();

  float getPitch();
  float getRoll();
  float getYaw();
};

#endif //__HEXAPODKINEMATICS_H__
