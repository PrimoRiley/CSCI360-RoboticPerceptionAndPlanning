// DriveTrain classes implementation file

#include "CSCIDriveTrain.h"

namespace csci
{

/*********************** DRIVE TRAIN ******************************/

DriveTrain::DriveTrain(double fbMultiplier, double lrMultiplier,
                       double diagMultiplier, double spinMultiplier,
                       double speedFraction)
  : m_fbMultiplier(fbMultiplier),
    m_lrMultiplier(lrMultiplier),
    m_diagMultiplier(diagMultiplier),
    m_spinMultiplier(spinMultiplier),
    m_speedFraction(speedFraction),
    m_moveState(MoveState::msStop)
  { }

double DriveTrain::adjustDistance(MoveState moveState, double distance)
{
  const double SQRT_2 = 1.41421356237;
  
  // Depending on move state, adjust distance by appropriate multiplier.
  
  switch ( moveState )
  {
    case MoveState::msForward:
    case MoveState::msReverse:
    {
      distance *= m_fbMultiplier;
      break;
    }

    case MoveState::msLeft:
    case MoveState::msRight:
    {
      distance *= m_lrMultiplier;
      break;
    }    
    
    case MoveState::msDiagFL:
    case MoveState::msDiagFR:
    case MoveState::msDiagRL:
    case MoveState::msDiagRR:    
    {
      // Since we're moving along a 45-degree diagonal, we must actually
      // move sqrt(2) times further to achieve desired distance.
      
      distance *= ( m_diagMultiplier * SQRT_2 );
      break;
    }
    
    case MoveState::msRotateCW:
    case MoveState::msRotateCCW:
    {
      // Since we're moving along the circumference of a circle, we must
      // actually move sqrt(2) times further to achieve desired distance.
            
      distance *= ( m_spinMultiplier * SQRT_2 );
      break;
    }
    
    case MoveState::msStop:
    {
      distance = 0.0;
      break;
    }
  }
  
  return distance;
}

double DriveTrain::inchesToMM(double inches)
{
  return inches * 25.4;
}

/***************** TETRIX MECANUM DRIVE TRAIN *********************/

TMDriveTrain::TMDriveTrain(PRIZM& prizm, EXPANSION& exc,
                           double fbMultiplier,
                           double lrMultiplier,
                           double diagMultiplier,
                           double spinMultiplier,
                           double speedFraction)
  : DriveTrain(fbMultiplier, lrMultiplier, diagMultiplier,
               spinMultiplier, speedFraction),
    m_Prizm(prizm),
    m_Exc(exc)
{
  setSpeedFraction(speedFraction);

  // Tetrix mecanum wheel diameter = 98 millimeters.
  // Calculate linear millimeter to rotational degree conversion.
  
  m_MMToDegrees =  360.0 / (98.0 * PI);
}

void TMDriveTrain::setup()
{
  m_Prizm.PrizmBegin();         // Initialize Tetrix controllers.
  halt();  
}

void TMDriveTrain::setSpeedFraction(double speedFraction)
{
  m_speedFraction = speedFraction;
  
  // Top rotational speed of Tetrix motor controllers is
  // 720 deegrees per second.
  
  m_speedDPS = static_cast<int>( 720.0 * speedFraction + 0.5 );
}

double TMDriveTrain::getSpeedFraction( ) const
{
  return m_speedFraction;
}

uint32_t TMDriveTrain::getInchesTravelTime
           (MoveState moveState, double inches)
{
  return getMMTravelTime(moveState, inchesToMM(inches));
}

uint32_t TMDriveTrain::getMMTravelTime
           (MoveState moveState, double millimeters)
{  
  millimeters = adjustDistance(moveState, millimeters);
  
  return getDegreesTravelTime(moveState, mmToDegrees(millimeters));
}

uint32_t TMDriveTrain::getDegreesTravelTime
           (MoveState moveState, double degrees)
{
  // Return the time (in milliseconds) to rotate wheel the
  // required number of degrees at current speed.
  
  double timeInSeconds = degrees / static_cast<double>(m_speedDPS);
  
  return static_cast<uint32_t>(timeInSeconds * 1000.0 + 0.5); 
}

uint32_t TMDriveTrain::getSpinCWTravelTime(double spinDegrees)
{
  double spinMM = spinDegreesToMM(spinDegrees);
  
  return getMMTravelTime(MoveState::msRotateCW, spinMM);
}

uint32_t TMDriveTrain::getSpinCCWTravelTime(double spinDegrees)
{
  double spinMM = spinDegreesToMM(spinDegrees);
  
  return getMMTravelTime(MoveState::msRotateCCW, spinMM);
}

double TMDriveTrain::mmToDegrees(double millimeters)
{
  return millimeters * m_MMToDegrees;
}

double TMDriveTrain::spinDegreesToMM(double degrees)
{
  // Typical Tetrix wheel base diagonal distance is 15 inches.
  // The circumference of the circle spun is 15 * PI (inches).
  // This is the distance to move the wheels to spin 360 degrees.
  
  return ( ( inchesToMM(15.0) * PI ) / 360.0 ) * degrees; 
}

void TMDriveTrain::move(MoveState moveState)
{
  moveDegrees(moveState, 0);  // Zero means no specific target.
}

void TMDriveTrain::moveDegrees(MoveState moveState, double degrees)
{
  // Round to nearest integer number of degrees.
  
  uint32_t intDegrees = static_cast<uint32_t>(degrees) + 0.5;
  
  // Depending on move state, call appropriate movement routine...
  
  switch ( moveState )
  {
    case MoveState::msForward:
    {
      forward(intDegrees);
      break;
    }
    
    case MoveState::msReverse:
    {
      reverse(intDegrees);
      break;
    }
    
    case MoveState::msLeft:
    {
      left(intDegrees);
      break;
    }    
    
    case MoveState::msRight:
    {
      right(intDegrees);
      break;
    }

    case MoveState::msDiagFL:
    {
      diagFL(intDegrees);
      break;
    }
    
    case MoveState::msDiagFR:
    {
      diagFR(intDegrees);
      break;
    }

    case MoveState::msDiagRL:
    {
      diagRL(intDegrees);
      break;
    }

    case MoveState::msDiagRR:
    {
      diagRR(intDegrees);
      break;
    }

    case MoveState::msRotateCW:
    {
      rotateCW(intDegrees);
      break;
    }

    case MoveState::msRotateCCW:
    {
      rotateCCW(intDegrees);
      break;
    }

    default:
    case MoveState::msStop:
    {
      halt();
      break;
    }
  }
}

void TMDriveTrain::moveInches(MoveState moveState, double inches)
{
  moveMM(moveState, inchesToMM(inches));
}

void TMDriveTrain::moveMM(MoveState moveState, double millimeters)
{  
  millimeters = adjustDistance(moveState, millimeters);
  
  moveDegrees(moveState, mmToDegrees(millimeters));
}

void TMDriveTrain::spinCW(double spinDegrees)
{
  double spinMM = spinDegreesToMM(spinDegrees);  
  
  moveMM(MoveState::msRotateCW, spinMM);
}

void TMDriveTrain::spinCCW(double spinDegrees)
{
  double spinMM = spinDegreesToMM(spinDegrees);  
  
  moveMM(MoveState::msRotateCCW, spinMM);
}

void TMDriveTrain::stop()
{
  halt();
}

void TMDriveTrain::forward(uint32_t degrees)
{
  // Invert left side motors. Make sure right side motors are not inverted.
  
  m_Prizm.setMotorInvert(leftMotor,1);
  m_Exc.setMotorInvert(1,leftMotor,1);
  
  m_Prizm.setMotorInvert(rightMotor,0);
  m_Exc.setMotorInvert(1,rightMotor,0);
  
  setAllMotorsMoving(degrees);
  
  m_moveState = MoveState::msForward;
}

void TMDriveTrain::reverse(uint32_t degrees)
{
  // Invert right side motors. Make sure left side motors are not inverted.

  m_Prizm.setMotorInvert(leftMotor,0);
  m_Exc.setMotorInvert(1,leftMotor,0);
  
  m_Prizm.setMotorInvert(rightMotor,1);
  m_Exc.setMotorInvert(1,rightMotor,1);
  
  setAllMotorsMoving(degrees);
    
  m_moveState = MoveState::msReverse;
}

void TMDriveTrain::left(uint32_t degrees)
{
  // Invert rear motors. Make sure front motors are not inverted.
  
  m_Prizm.setMotorInvert(leftMotor,0);
  m_Exc.setMotorInvert(1,leftMotor,1);
  
  m_Prizm.setMotorInvert(rightMotor,0);
  m_Exc.setMotorInvert(1,rightMotor,1);
  
  setAllMotorsMoving(degrees);
  
  m_moveState = MoveState::msLeft;
}

void TMDriveTrain::right(uint32_t degrees)
{
  // Invert front motors. Make sure rear motors are not inverted.
  
  m_Prizm.setMotorInvert(leftMotor,1);
  m_Exc.setMotorInvert(1,leftMotor,0);
  
  m_Prizm.setMotorInvert(rightMotor,1);
  m_Exc.setMotorInvert(1,rightMotor,0);
  
  setAllMotorsMoving(degrees);
  
  m_moveState = MoveState::msRight;
}

void TMDriveTrain::diagFL(uint32_t degrees)
{
  // Invert rear left motor. Make sure front right motor is not inverted.
  // Other motors don't matter.
  
  m_Prizm.setMotorInvert(leftMotor,0);
  m_Exc.setMotorInvert(1,leftMotor,1);
  
  m_Prizm.setMotorInvert(rightMotor,0);
  m_Exc.setMotorInvert(1,rightMotor,0);
  
  // Set front right and rear left motors in motion.
  
  setFR_RL_MotorsMoving(degrees);
  
  m_moveState = MoveState::msDiagFL;
}

void TMDriveTrain::diagFR(uint32_t degrees)
{
  // Invert front left motor. Make sure rear right motor is not inverted.
  // Other motors don't matter.
  
  m_Prizm.setMotorInvert(leftMotor,1);
  m_Exc.setMotorInvert(1,leftMotor,0);
  
  m_Prizm.setMotorInvert(rightMotor,0);
  m_Exc.setMotorInvert(1,rightMotor,0);
  
  // Set front left and rear right motors in motion.
  
  setFL_RR_MotorsMoving(degrees);
  
  m_moveState = MoveState::msDiagFR;
}

void TMDriveTrain::diagRL(uint32_t degrees)
{
  // Invert rear right motor. Make sure front left motor is not inverted.
  // Other motors don't matter.
  
  m_Prizm.setMotorInvert(leftMotor,0);
  m_Exc.setMotorInvert(1,leftMotor,0);
  
  m_Prizm.setMotorInvert(rightMotor,0);
  m_Exc.setMotorInvert(1,rightMotor,1);
  
  // Set front left and rear right motors in motion.
  
  setFL_RR_MotorsMoving(degrees);
  
  m_moveState = MoveState::msDiagRL;
}

void TMDriveTrain::diagRR(uint32_t degrees)
{
  // Invert front right motor. Make sure rear left motor is not inverted.
  // Other motors don't matter.
  
  m_Prizm.setMotorInvert(leftMotor,0);
  m_Exc.setMotorInvert(1,leftMotor,0);
  
  m_Prizm.setMotorInvert(rightMotor,1);
  m_Exc.setMotorInvert(1,rightMotor,0);
  
  // Set front right and rear left motors in motion.
  
  setFR_RL_MotorsMoving(degrees);
  
  m_moveState = MoveState::msDiagRR;
}

void TMDriveTrain::rotateCW(uint32_t degrees)
{
  // Invert all motors
  
  m_Prizm.setMotorInvert(leftMotor,1);
  m_Exc.setMotorInvert(1,leftMotor,1);
  
  m_Prizm.setMotorInvert(rightMotor,1);
  m_Exc.setMotorInvert(1,rightMotor,1);
  
  setAllMotorsMoving(degrees);
  
  m_moveState = MoveState::msRotateCW;
}

void TMDriveTrain::rotateCCW(uint32_t degrees)
{
  // Invert no motors
  
  m_Prizm.setMotorInvert(leftMotor,0);
  m_Exc.setMotorInvert(1,leftMotor,0);
  
  m_Prizm.setMotorInvert(rightMotor,0);
  m_Exc.setMotorInvert(1,rightMotor,0);
  
  setAllMotorsMoving(degrees);
  
  m_moveState = MoveState::msRotateCCW;
}

void TMDriveTrain::halt()
{
  // Setting speed to zero stops all movement in progress.
  
  m_Prizm.setMotorSpeeds(0, 0);
  m_Exc.setMotorSpeeds(1, 0, 0);
  
  // Reset encoders in preparation for next movement.
  
  m_Prizm.resetEncoders();
  m_Exc.resetEncoders(1);
  
  m_moveState = MoveState::msStop;
}

void TMDriveTrain::setAllMotorsMoving(uint32_t degrees)
{
  if ( degrees == 0 )
  {
    // Set all motors moving, but there's no specific target.

    m_Prizm.setMotorSpeeds(m_speedDPS, m_speedDPS);
    m_Exc.setMotorSpeeds(1, m_speedDPS, m_speedDPS);    
  }
  else
  {
    m_Prizm.setMotorDegrees(m_speedDPS, degrees, m_speedDPS, degrees);
    m_Exc.setMotorDegrees(1, m_speedDPS, degrees, m_speedDPS, degrees);    
  }
}

void TMDriveTrain::setFR_RL_MotorsMoving(uint32_t degrees)
{
  // If we're currently in motion, stop motors we won't be using
  // in this move.  Otherwise, they'll keep running.
  
  if ( m_moveState != MoveState::msStop )
  {
    m_Prizm.setMotorSpeed(leftMotor,0);
    m_Exc.setMotorSpeed(1, rightMotor, 0);
    m_Prizm.resetEncoder(leftMotor);
    m_Exc.resetEncoder(1,rightMotor);
  }
  
  if ( degrees == 0 )
  {
    // Set motors moving, but there's no specific target.

    m_Prizm.setMotorSpeed(rightMotor, m_speedDPS);
    m_Exc.setMotorSpeed(1, leftMotor, m_speedDPS);
  }
  else
  {
    m_Prizm.setMotorDegree(rightMotor, m_speedDPS, degrees);
    m_Exc.setMotorDegree(1,leftMotor, m_speedDPS, degrees);
  }
}

void TMDriveTrain::setFL_RR_MotorsMoving(uint32_t degrees)
{
  // If we're currently in motion, stop motors we won't be using
  // in this move.  Otherwise, they'll keep running.
  
  if ( m_moveState != MoveState::msStop )
  {
    m_Prizm.setMotorSpeed(rightMotor,0);
    m_Exc.setMotorSpeed(1, leftMotor, 0);    
    m_Prizm.resetEncoder(rightMotor);
    m_Exc.resetEncoder(1,leftMotor);
  }
    
  if ( degrees == 0 )
  {
    // Set motors moving, but there's no specific target.

    m_Prizm.setMotorSpeed(leftMotor, m_speedDPS);
    m_Exc.setMotorSpeed(1, rightMotor, m_speedDPS);
  }
  else
  {
    m_Prizm.setMotorDegree(leftMotor, m_speedDPS, degrees);
    m_Exc.setMotorDegree(1, rightMotor, m_speedDPS, degrees);
  }
}

MoveState TMDriveTrain::getMoveState()
{
  return m_moveState;
}

bool TMDriveTrain::isBusy()
{
  // Depending on the current movement state...
  
  switch( m_moveState )
  {
    case MoveState::msForward:
    case MoveState::msReverse:
    case MoveState::msRotateCW:
    case MoveState::msRotateCCW:
    case MoveState::msLeft:
    case MoveState::msRight:
    {
      return ( m_Prizm.readMotorBusy(leftMotor)  ||
               m_Prizm.readMotorBusy(rightMotor) ||
               m_Exc.readMotorBusy(1,leftMotor)  ||
               m_Exc.readMotorBusy(1,rightMotor) );
    }
    
    case MoveState::msDiagFL:
    case MoveState::msDiagRR:
    {
      return ( m_Prizm.readMotorBusy(rightMotor) ||
               m_Exc.readMotorBusy(1,leftMotor) );
    }

    case MoveState::msDiagFR:
    case MoveState::msDiagRL:
    {
      return ( m_Prizm.readMotorBusy(leftMotor) ||
               m_Exc.readMotorBusy(1,rightMotor) );
    }

    case MoveState::msStop:
    default:
      return false;
  }
  
  return false;
}

}   // End namespace
