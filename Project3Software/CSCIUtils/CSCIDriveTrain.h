#ifndef INCLUDE_CSCI_DRIVE_TRAIN
#define INCLUDE_CSCI_DRIVE_TRAIN

// DriveTrain classes header file for Arduino sensors/devices.

#include "CSCICore.h"
#include <PRIZM.h>    // Tetrix PRIZM and EXPANSION controller library

namespace csci
{

// Enum defining movement states.
  
enum MoveState
{
  msForward,    // Straight forward (no rotation)
  msReverse,    // Straight backward (no rotation)
  msLeft,       // Straight Left (no rotation)
  msRight,      // Straight Right (no rotation)
  msDiagFL,     // Diagonal forward left (no rotation)
  msDiagFR,     // Diagonal forward right (no rotation)
  msDiagRL,     // Diagonal reverse left (no rotation)
  msDiagRR,     // Diagonal reverse right (no rotation)
  msRotateCW,   // Rotate clockwise (spin in place)
  msRotateCCW,  // Rotate counter clockwiser (spin in place)    
  msStop        // No motion
};
  
/*********************** DRIVE TRAIN ******************************/
// DriveTrain is the virtual base class from which all drive train
// classes are derived.
//
// NOTE: Without additional sensors (such as accelerometers and
//       gyroscopes), it's not possible to drive at precise speeds
//       and directions, as floor and wheel characteristics will
//       result in the mechanics to be behave non-ideally.
//
//       However, it's still useful to be able to drive at speeds
//       and distances within a couple percent of those desired.
//       This is accomplished using movement adjustment multipliers.
//       These multipliers will change depending on the physical
//       characteristics of the floor and drive train mechanics,
//       so they should be experimentally determined and passed
//       into the ctor.
//
//       (The multipliers are usually in the range of 1.0 - 2.0.) 

class DriveTrain
{
  public:
  DriveTrain(double fbMultiplier, double lrMultiplier,
             double diagMultiplier, double spinMultiplier,
             double speedFraction);

  // Set/get drive train speed fraction (0.0 = none, 1.0 = max speed)
  
  virtual void    setSpeedFraction(double speedFraction) = 0;
  virtual double  getSpeedFraction() const = 0;
             
  // Perform additonal setup.  MUST be called after construction,
  // but before any other methods are called.
  
  virtual void setup() = 0;
  
  // Returns the time (in milliseconds) necessary to travel the
  // specified distance in the specified direction at the currently
  // set speed fraction.
  
  virtual uint32_t getMMTravelTime(MoveState moveState, double millimeters) = 0;
  virtual uint32_t getInchesTravelTime(MoveState moveState, double inches) = 0;
  virtual uint32_t getDegreesTravelTime(MoveState moveState, double degrees) = 0;
    
  // Returns the time (in milliseconds) necessary to spin the Tetrix
  // the specified degrees at the currently set speed fraction.
  
  virtual uint32_t getSpinCWTravelTime(double spinDegrees) = 0;
  virtual uint32_t getSpinCCWTravelTime(double spinDegrees) = 0;
  
  // Move in the specified direction, but there's no specific target.
  // Motion continues until another motion operation is specified.
  
  virtual void move(MoveState moveState) = 0;
  
  // Move in specified direction, rotating wheels specified degrees.
  // When "moveState == msStop", degrees is ignored.

  virtual void moveDegrees(MoveState moveState, double degrees) = 0;
  
  // Move linear distance in specified direction.
  // When "moveState == msStop", distance is ignored.
  
  virtual void moveInches(MoveState moveState, double inches) = 0;
  virtual void moveMM(MoveState moveState, double millimeters) = 0;
  
  // Spin in place the specified number of degrees.
  // "spinCW" spins clockwise
  // "spinCCW spins counter-clockwise
  
  virtual void spinCW(double spinDegrees) = 0;
  virtual void spinCCW(double spinDegrees) = 0;
  
  // Routine to stop all movement.
  
  virtual void stop() = 0;
  
  // Returns current movement state.
   
  virtual MoveState getMoveState() = 0;
  
  // Returns "true" if drive train is busy performing last action.
  
  virtual bool isBusy() = 0;
  
  protected:
  
  // Apply heuristic distance adjustments to compensate for non-ideal
  // and surface-specific movement mechanics.
  
  double adjustDistance(MoveState moveState, double distance);
  
  // Convert inches to millimeters.
  
  double inchesToMM(double inches);
  
  protected:
  MoveState   m_moveState;      // Current movement state.
  double      m_speedFraction;  // Speed fraction (0.0 = none, 1.0 = max speed)
  double      m_fbMultiplier;   // Front-Back movement multiplier
  double      m_lrMultiplier;   // Left-Right movement multiplier
  double      m_diagMultiplier; // Diagonal movement multiplier
  double      m_spinMultiplier; // Rotational movement multiplier
};

/***************** TETRIX MECANUM DRIVE TRAIN *********************/
// A TMDriveTrain consists of:
//
// 4 TorqueNADO motors with encoders.
// 4 Tetrix mechanum wheels.
// 1 PRIZM controller for the front motors (1 = left, 2 = right)
//     Front left wheel is Tetrix Mecanum type "A".
//     Front right wheel is Tetrix Mecanum type "B".
// 1 EXPANSION controller for the rear motors (1 = left, 2 = right)
//     Rear left wheel is Tetrix Mecanum type "B".
//     Rear right wheel is Tetrix Mecanum type "A".

class TMDriveTrain : public DriveTrain
{
  public:
  // Construct from references to Tetrix PRIZM and EXPANSION controller
  // objects, movement adjustment multipliers, and an initial speed
  // factor (set low for safety).
  
  // See note (in DriveTrain ctor comments) concerning movement
  // adjustment multipliers.  
  
  TMDriveTrain(PRIZM& prizm, EXPANSION& exc,
               double fbMultiplier,
               double lrMultiplier,
               double diagMultiplier,
               double spinMultiplier,
               double speedFraction = 0.1);
  
  // Base class overrides
  
  void setup() override;
  
  void    setSpeedFraction(double speedFraction) override;
  double  getSpeedFraction() const override;  
  
  uint32_t getInchesTravelTime(MoveState moveState, double inches) override;
  uint32_t getMMTravelTime(MoveState moveState, double millimeters) override;
  uint32_t getDegreesTravelTime(MoveState moveState, double degrees) override;
  
  uint32_t getSpinCWTravelTime(double spinDegrees);
  uint32_t getSpinCCWTravelTime(double spinDegrees); 
  
  void move(MoveState moveState) override;
  
  void moveDegrees(MoveState moveState, double degrees) override;
  void moveInches(MoveState moveState, double inches) override;
  void moveMM(MoveState moveState, double millimeters) override;
  
  void spinCW(double spinDegrees) override;
  void spinCCW(double spinDegrees) override;
  
  void stop() override;
  
  MoveState getMoveState() override;
  
  bool isBusy() override;
  
  protected:
  
  // Tetrix left/right side motor numbers.
  
  const int leftMotor = 1;
  const int rightMotor = 2;  
  
  // Drive train movement routines.
  // Speed is controlled by the m_speedFraction value.
  // "degrees" is the number of degrees to turn the wheels.
  //
  // Note: If degrees == 0, the motors move in the indicated
  //       direction, but continue indefinitely until
  //       another movement command is given.
  
  void forward(uint32_t degrees);
  void reverse(uint32_t degrees);
  void left(uint32_t degrees);
  void right(uint32_t degrees);
  void diagFL(uint32_t degrees);
  void diagFR(uint32_t degrees);
  void diagRL(uint32_t degrees);
  void diagRR(uint32_t degrees);
  void rotateCW(uint32_t degrees);
  void rotateCCW(uint32_t degrees);  
  void halt();
  
  // Set motors moving in the indicated direction for the specified
  // wheel rotation (in degrees).
  //
  // Note: If degrees == 0, the motors move in the indicated
  //       direction, but continue indefinitely until
  //       another movement command is given.  
  
  void setAllMotorsMoving(uint32_t degrees);
  void setFR_RL_MotorsMoving(uint32_t degrees);
  void setFL_RR_MotorsMoving(uint32_t degrees);
  
  // Convert millimeters to wheel rotation degrees.
  
  double mmToDegrees(double millimeters); 
  
  // Convert spin degrees to Tetrix rotation distance (in millimeters).
  
  double spinDegreesToMM(double degrees);  
  
  protected:
  PRIZM&      m_Prizm;        // Associated Tetrix PRIZM controller.
  EXPANSION&  m_Exc;          // Associated Tetrix EXPANSION controller.
  double      m_MMToDegrees;  // Linear mm to wheel rotation degrees.
  int         m_speedDPS;     // Speed in degrees per second. 
};

}   // End namespace

#endif    // INCLUDE_CSCI_DRIVE_TRAIN
