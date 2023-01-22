#include "CSPID.h"




void CSPID::begin(double* _input,
	                double* _output,
	                double* _setpoint,
	                const double& _p,
	                const double& _i,
	                const double& _d,
	                const pOn& _pOn,
	                const dir& _direction,	          
	                const double& _bias)
{
	input    = _input;
	output   = _output;
	setpoint = _setpoint;
	setCoefficients(_p, _i, _d);
	setPOn(_pOn);
	setBias(_bias);
	setDirection(_direction);
	reset();	
}









void CSPID::reset()
{
	curError    = 0;
	curSetpoint = 0;
	curInput    = 0;

	lastError    = 0;
	lastSetpoint = 0;
	lastInput    = 0;

	pOut = 0;
	iOut = 0;
	dOut = 0;


}








CMsg CSPID::compute(Stream* stream)
{
		CMsg m;
	//if (timer.fire() && modeType == PON)
	//	{
		kp = pIn;
		ki = iIn;
		kd = dIn;

		if (direction == PBACKWARD)
		{
			kp *= -1;
			ki *= -1;
			kd *= -1;
		}

	//m.set("P", kp);
	//	m.set("I", ki);
	//	m.set("D", kd);

		m.set("SET", *setpoint);
		m.set("IN", *input);
		
		

		lastInput    = curInput;
		lastSetpoint = curSetpoint;
		lastError    = curError;

		curInput    = *input;
		curSetpoint = *setpoint;
		curError    = curSetpoint - curInput;


		if(curError<-180.0){
			curError+=360.0;
		}
		else if(curError>180.0){
			curError-=360.0;
		}
  

		m.set("ERR", curError);

		double dInput = *input - lastInput;

		if (pOnType == P_ON_E){
			pOut = kp * curError;
		//	m.set("TYPE", "POE");

		}
		else if (pOnType == P_ON_M){
			pOut = -kp * dInput;
			m.set("TYPE", "POM  -- PROBLEM");
		}

		//m.set("Po", pOut );

		
		//dOut = -kd * dInput; // Derrivative on measurement
		//double dInput = *input - lastInput;
		
		dOut = -kd * (curError - lastError); // Derrivative on error

		m.set("Do", dOut );

		double iTemp = iOut + (ki * ((curError + lastError) / 2.0)); // Trapezoidal integration

		m.set("iTemp", iTemp );

		iOut       = constrain(iTemp, windupMin, windupMax);       // Prevent integral windup

		m.set("Io", iOut);

/*
		m.set("windupMin", windupMin);
		m.set("windupMax", windupMax);

		double outTemp = bias + pOut + dOut;                           // Output without integral
		m.set("outTemp", outTemp );
		double iMax    = constrain(outputMax - outTemp, 0, outputMax); // Maximum allowed integral term before saturating output
		double iMin    = constrain(outTemp - outputMin, outputMin, 0); // Minimum allowed integral term before saturating output

		m.set("iMax", iMax );
		m.set("iMin", iMin );

		m.set("outputMin", outputMin );

		iOut = constrain(iTemp, iMin, iMax);
		m.set("iOut", iOut );
		*/
		double newOutput = bias + pOut + iOut + dOut;
		m.set("newOut", newOutput );
		

		if ((newOutput>outputMax) || (newOutput<outputMin))
		{
			countMaxMin++;
		}
		else
		{
			countMaxMin=0;
		}

		newOutput = constrain(newOutput, outputMin, outputMax);
		m.set("Output", newOutput );
		*output   = newOutput;
		if(countMaxMin>50)
		{
			reset();
			countMaxMin=0;
		}
	//}
	return m;
}




void CSPID::setOutputLimits(const double& min, const double& max)
{
	
	outputMax = max;
	outputMin = min;


}




void CSPID::setWindUpLimits(const double& min, const double& max)
{

	windupMax = max;
	windupMin = min;
}




void CSPID::setDeadBand(const double& min, const double& max)
{
	
	deadBandMax = max;
	deadBandMin = min;

}




void CSPID::setPOn(const pOn& _pOn)
{
	pOnType = _pOn;
}




void CSPID::setBias(const double& _bias)
{
	bias = _bias;
}




void CSPID::setCoefficients(const double& _p, const double& _i, const double& _d)
{
	pIn = _p;
	iIn = _i;
	dIn = _d;
}




void CSPID::setDirection(const dir& _direction)
{
	direction = _direction;
	reset();
}




void CSPID::reverse()
{
	if (direction == PFORWARD)
		direction = PBACKWARD;
	else if (direction == PBACKWARD)
		direction = PFORWARD;
	
	reset();
}








double CSPID::B()
{
	return bias;
}




double CSPID::P()
{
	return pOut;
}




double CSPID::I()
{
	return iOut;
}




double CSPID::D()
{
	return dOut;
}



void CSPID::debug(Stream* stream , const char* controllerName, const byte& mask)
{
	if(stream==NULL)
		return;



	stream->print(controllerName);
	stream->print("> OMax ");
	stream->print(outputMax);
	stream->print(" ");

	stream->print("  OMin ");
	stream->print(outputMin);
	stream->print(" ");


	if (mask & PRINT_INPUT)
	{
		
		stream->print("Input ");
		stream->print(*input);
		stream->print(" ");
	}
	
	if (mask & PRINT_OUTPUT)
	{
		stream->print("Output ");
		stream->print(*output);
		stream->print(" ");
	}
		
	if (mask & PRINT_SETPOINT)
	{
		stream->print("Setpoint ");
		stream->print(*setpoint);
		stream->print(" ");
	}
		
	if (mask & PRINT_BIAS)
	{
		stream->print("Bias ");
		stream->print(bias);
		stream->print(" ");
	}
		
	if (mask & PRINT_P)
	{
		stream->print("P ");
		stream->print(pOut);
		stream->print(" ");
	}
		
	if (mask & PRINT_I)
	{
		stream->print("I ");
		stream->print(iOut);
		stream->print(" ");
	}
		
	if (mask & PRINT_D)
	{
		stream->print("D ");
		stream->print(dOut);
		stream->print(" ");
	}
	
	stream->println();
			
	
}