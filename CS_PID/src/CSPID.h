#pragma once
#include "Arduino.h"
//#include "FireTimer.h"
#include <msg.h>




const byte PRINT_INPUT    = 0b1;
const byte PRINT_OUTPUT   = 0b10;
const byte PRINT_SETPOINT = 0b100;
const byte PRINT_BIAS     = 0b1000;
const byte PRINT_P        = 0b10000;
const byte PRINT_I        = 0b100000;
const byte PRINT_D        = 0b1000000;




enum pOn { P_ON_E, P_ON_M };
enum mode { POFF, PON };
enum dir { PFORWARD, PBACKWARD };




class CSPID
{
public:


	double pOut=0.0;
	double iOut=0.0;
	double dOut=0.0;

	double* input;
	double* output;
	double* setpoint;




	virtual void begin(double* _input,
		               double* _output,
		               double* _setpoint,
		               const double& _p = 0,
		               const double& _i = 0,
		               const double& _d = 0,
		               const pOn& _pOn = P_ON_E,
		               const dir& _direction = PFORWARD,		               
		               const double& _bias = 0);

	void reset();	
	virtual CMsg compute(Stream* stream = &Serial);
	void setOutputLimits(const double& min, const double& max);
	void setWindUpLimits(const double& min, const double& max);
	void setDeadBand(const double& min, const double& max);
	void setPOn(const pOn& _pOn);
	void setBias(const double& _bias);
	void setCoefficients(const double& _p, const double& _i, const double& _d);
	void setDirection(const dir& _direction);
	void reverse();	

	double B();
	double P();
	double I();
	double D();
	
	void debug(Stream* stream = &Serial,
	           const char* controllerName = "controller",
		       const byte& mask = 0xFF);




protected:
	double bias;

	double outputMax = 100;
	double outputMin = 0;

	double windupMax = 1000;
	double windupMin = -1000;

	double deadBandMax = 0;
	double deadBandMin = 0;

	double curError;
	double curSetpoint;
	double curInput;

	double lastError;
	double lastSetpoint;
	double lastInput;

	double pIn;
	double iIn;
	double dIn;

	double kp;
	double ki;
	double kd;

	int countMaxMin = 0;

	pOn pOnType;
	mode modeType;
	dir direction;

	//FireTimer timer;
};