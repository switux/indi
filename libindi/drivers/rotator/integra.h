/*
    Gemini Telescope Design Integra85 Focusing Rotator.
    Copyright (C) 2017 Hans Lambermont

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include "indifocuser.h"
#include "indirotatorinterface.h"

class Integra : public INDI::Focuser, public INDI::RotatorInterface
{
  public:

    typedef enum { MOTOR_FOCUS, MOTOR_ROTATOR } MotorType;

    Integra();
    virtual ~Integra() = default;

    virtual bool Handshake();
    const char * getDefaultName();
    virtual bool initProperties();
    virtual bool updateProperties();
    virtual bool ISNewNumber (const char * dev, const char * name, double values[], char * names[], int n);
    virtual bool ISNewSwitch (const char * dev, const char * name, ISState * states, char * names[], int n);

  protected:
    // Focuser
    virtual IPState MoveAbsFocuser(uint32_t targetTicks);
    virtual IPState MoveRelFocuser(FocusDirection dir, uint32_t ticks);
    virtual bool AbortFocuser();

    // Rotator
    virtual IPState HomeRotator();
    virtual IPState MoveRotator(double angle);
    virtual bool AbortRotator();

    // Misc.
    virtual bool saveConfigItems(FILE *fp);
    virtual void TimerHit();

  private:
    bool getFirmware();
    bool getFocuserType();
    bool Ack();
    bool gotoMotor(MotorType type, int32_t position);
    bool relativeGotoMotor(MotorType type, int32_t relativePosition);
    bool getPosition(MotorType type);
    bool stopMotor(MotorType type);
    bool isMotorMoving(MotorType type);
    bool getTemperature();
    bool findHome(uint8_t motorTypes);
    bool isHomingComplete();
    void cleanPrint(const char *cmd, char *cleancmd);

    INumber MaxPositionN[2];
    INumberVectorProperty MaxPositionNP;

    INumber SensorN[2];
    INumberVectorProperty SensorNP;
    enum { SENSOR_TEMPERATURE };

    ILight LimitSwitchL[3];
    ILightVectorProperty LimitSwitchLP;
    enum { ROTATION_SWITCH, OUT_SWITCH, IN_SWITCH };

    ISwitch HomeSelectionS[3];
    ISwitchVectorProperty HomeSelectionSP;
    ISwitch FindHomeS[1];
    ISwitchVectorProperty FindHomeSP;

    // Rotator Steps
    INumber RotatorAbsPosN[1];
    INumberVectorProperty RotatorAbsPosNP;

    double lastTemperature { 0 };
    int timeToReadTemperature = 0;
    double ticksPerDegree { 0 };
    uint32_t lastFocuserPosition { 0 };
    uint32_t lastRotatorPosition { 0 };
    uint32_t targetPosition { 0 };
    IPState rotationLimit { IPS_IDLE };
    IPState outSwitchLimit { IPS_IDLE };
    IPState inSwitchLimit { IPS_IDLE };
};
