/*
 * Copyright (C) 2007 Robotics at Maryland
 * Copyright (C) 2007 Joseph Lisee <jlisee@umd.edu>
 * All rights reserved.
 *
 * Author: Joseph Lisee <jlisee@umd.edu>
 * File:  packages/vehicle/src/device/Thruster.cpp
 */

// STD Includes
#include <sstream>

// Project Includes
#include "vehicle/include/device/Thruster.h"
#include "vehicle/include/device/ThrusterCommunicator.h"
#include "vehicle/include/device/ThrusterCommand.h"
// Project Includes
#include "vehicle/include/device/Thruster.h"
#include "vehicle/include/device/ThrusterCommunicator.h"
#include "vehicle/include/device/ThrusterCommand.h"

namespace ram {
namespace vehicle {
namespace device {
    
const std::string Thruster::SOFT_RESET = std::string("Y");
const std::string Thruster::SET_FORCE = std::string("C");
    
Thruster::Thruster(Vehicle* vehicle, core::ConfigNode config) :
    Device(vehicle, config["name"].asString()),
    m_address(config["address"].asString()),
    m_calibrationFactor(config["calibration_factor"].asDouble()),
    m_direction(config["direction"].asInt(1))
{
    // Register thruster
    ThrusterCommunicator::registerThruster(this);

    // Preform a soft reset just to be safe
    ThrusterCommandPtr cmd(new ThrusterCommand(m_address, SOFT_RESET, "",
                                               500));
    ThrusterCommunicator::getSingleton().sendThrusterCommand(cmd);
}

Thruster::~Thruster()
{
    // Preform a soft reset to make sure the power dies to the thruster
    ThrusterCommandPtr cmd(new ThrusterCommand(m_address, SOFT_RESET, "",
                                               500));
    ThrusterCommunicator::getSingleton().sendThrusterCommand(cmd);

    // Unregister from communicator so it will no when to destory itself
    ThrusterCommunicator::unRegisterThruster(this);
}

ThrusterPtr Thruster::construct(Vehicle* vehicle, core::ConfigNode config)
{
    return ThrusterPtr(new Thruster(vehicle, config));
}

ThrusterPtr Thruster::castTo(IDevicePtr ptr)
{
    return boost::dynamic_pointer_cast<Thruster>(ptr);
}
/*
Thruster* Thruster::castTo(IDevice* ptr)
{
    return dynamic_cast<Thruster*>(ptr);
    }*/
    
void Thruster::setForce(double force)
{
    m_force = force;
    double b = 0; // Not currently used
    int motorCount;
    
    // Convert force here (using calibration factor)
    motorCount = (int)((m_force / m_calibrationFactor - b) * 1023) / 27;

    // Take into acount motor direction
    motorCount = motorCount * m_direction;
    
    // Clamp the values
    if (motorCount > 1024)
        motorCount = 1023;
    else if (motorCount < -1024)
        motorCount = -1023;
    m_motorCount = motorCount;

    std::stringstream ss;
    ss << " " << m_motorCount;
    ThrusterCommandPtr cmd(new ThrusterCommand(m_address, SET_FORCE, ss.str(),
                                               4));
    ThrusterCommunicator::getSingleton().sendThrusterCommand(cmd);

    // Notify observers
    setChanged();
    notifyObservers(0, FORCE_UPDATE);
}

double Thruster::getForce()
{
    return m_force;
}

int Thruster::getMotorCount()
{
    return m_motorCount;
}
    
void Thruster::update(double timestep)
{
    ThrusterCommunicator::getSingleton().update(timestep);
}
    
void Thruster::background(int interval)
{
    ThrusterCommunicator::getSingleton().background(interval);
}

void Thruster::unbackground(bool join)
{
    ThrusterCommunicator::getSingleton().unbackground(join);
}

bool Thruster::backgrounded()
{
    return ThrusterCommunicator::getSingleton().backgrounded();
}
    
} // namespace device
} // namespace vehicle
} // namespace ram
