#include "stdafx.h"
#include ".\field.h"
#include "math.h"

enum {TEST_NO_COMMAND,TEST_KILL_COMMAND, TEST_SHOW_WEIGHTS,TEST_SHOW_DELAYS};

CFieldSimulator::CFieldSimulator(void)
{
		X = 8;
        Y = 8;
		dT = 0.00001;
		ChangeSize = 10;
		ChangeValue = 200E-12;
		cells = NULL;
		states = NULL;
		walls = NULL;
		tonic_input = NULL;
		surf = NULL;
		CMD = CMD_DEAD;
		AfferentFieldRadius =3.0;
		exp_weight_begin = 10.0;					
		exp_weight_end = 5;	
		exp_delay_begin = 1.0;					
		exp_delay_end = 0.1;	
		weight_noise= 0.1;				
		delay_noise	= 0.1;				
		SpikeN_Inc = 10;
}

CFieldSimulator::~CFieldSimulator(void)
{
	if (CMD!= CMD_DEAD) TerminateCore();

	delete NumCore;
	if (cells) 
	{
		for(int i=0; i < X; i++)
		for(int j=0; j < Y; j++)
		{
			delete [] cells[i+j*X].W;
			delete [] cells[i+j*X].D;
			delete [] cells[i+j*X].S;
			delete [] cells[i+j*X].SpikeTimes;
		}
		delete [] cells; cells = NULL; 
	}

	if (surf) delete [] surf; surf = NULL; 
	if (states) delete [] states; states = NULL;
	if (walls) delete [] walls; walls = NULL;
	if (tonic_input) delete [] tonic_input; tonic_input = NULL;
}

// Restarts numeric core
int CFieldSimulator::RestartCore(void)
{
	// suspend numeric core while restarting
	if ((CMD == CMD_RUN)||(CMD == CMD_DEAD))
	{
		CMD = CMD_PAUSE;
		while (CMD != CMD_PAUSE_ACK) Sleep(10);
	}

	// reallocate all storages
	if (walls) delete [] walls;
		walls = (field_type *)new char[sizeof(field_type)*X*Y]; memset((void*)walls,0,sizeof(field_type)*X*Y);
	if (tonic_input) delete [] tonic_input;
		tonic_input = (field_type *)new char[sizeof(field_type)*X*Y]; memset((void*)tonic_input,0,sizeof(field_type)*X*Y);
	if (surf)  delete [] surf;	
		surf = (surface_type *)new char[sizeof(surface_type)*X*Y*3]; memset((void*)surf,0,sizeof(surface_type)*X*Y*3); 	
	
	if (states)delete [] states; 	
		states = (field_type *)new char[sizeof(field_type)*X*Y]; memset((void*)states,0,sizeof(field_type)*X*Y); 
	
	if (cells) 
	{	// freeing allocated memory
		for(int i=0; i < X; i++)
		for(int j=0; j < Y; j++)
		{
			delete [] cells[i+j*X].W;
			delete [] cells[i+j*X].D;
			delete [] cells[i+j*X].S;
			delete [] cells[i+j*X].SpikeTimes;
		}
		delete [] cells; cells = NULL; 
	}
		cells = (TCell*)new char[sizeof(TCell)*X*Y]; memset((void*)cells,0,sizeof(TCell)*X*Y); 
	
	// zeroing simulation clock
	Ticks = 0;

	//allocate field in memory
	for(int i=0; i < X; i++)
	for(int j=0; j < Y; j++)
	{
		int IDX = i+j*X;
		cells[IDX].X = i;
		cells[IDX].Y = j;
		cells[IDX].AFRadius = AfferentFieldRadius;
		cells[IDX].S_n = 4; 
		cells[IDX].Fired = false;
		cells[IDX].ST_n = 0;
		cells[IDX].SpikeTimes = new TSpike[SpikeN_Inc];
		cells[IDX].SpikeSlotsAllocated = SpikeN_Inc;
		cells[IDX].AffCounter = 0;

		int Ws = AfferentFieldRadius * AfferentFieldRadius * 4;
		cells[IDX].W = (field_type *)new char[sizeof(field_type)*Ws]; memset((void*)cells[IDX].W,0,sizeof(field_type)*Ws); 
		
		//filling W matrix (weights)
		long double R2 = AfferentFieldRadius*AfferentFieldRadius;
		for(int k=-AfferentFieldRadius; k<AfferentFieldRadius; k++)
		for(int l=-AfferentFieldRadius; l<AfferentFieldRadius; l++)
		{
			if (k==0 && l==0) continue;
			if (k*k + l*l <= R2)
			{
				int _x = k + AfferentFieldRadius; 
				int _y = l + AfferentFieldRadius; 
				
				cells[i+j*X].W[int(_x+_y*AfferentFieldRadius*2)] = exp_weight_begin * exp( (sqrt(long double(k*k+l*l)))*log(long double(exp_weight_end/exp_weight_begin)) / AfferentFieldRadius); 
			}
		}

		cells[i+j*X].D = (field_type *)new char[sizeof(field_type)*Ws]; memset((void*)cells[i+j*X].D,0,sizeof(field_type)*Ws); 
		
		//filling D matrix (delays)
		
		for(int k=-AfferentFieldRadius; k<AfferentFieldRadius; k++)
		for(int l=-AfferentFieldRadius; l<AfferentFieldRadius; l++)
		{
			if (k*k + l*l <= R2)
			{
				int _x = k + AfferentFieldRadius; 
				int _y = l + AfferentFieldRadius; 
				//if (_x < 0) _x=0; if (_x > X-1) _x = X-1;
				//if (_y < 0) _y=0; if (_y > Y-1) _y = Y-1;
				cells[i+j*X].D[int(_x+_y*AfferentFieldRadius*2)] = 0.0;
			}
		 
		
		}
		int Ss = cells[i+j*X].S_n;
		cells[i+j*X].S = (field_type *)new char[sizeof(field_type)*Ss]; //memset((void*)cells[i+j*X].S,0,sizeof(field_type)*Ss); 
		cells[i+j*X].S[0] = 0.0L;
		cells[i+j*X].S[1] = -0.065L;
		cells[i+j*X].S[2] = 0.L;//-6.5;
		cells[i+j*X].S[3] = 0.L;//-6.5;
	}
	
	

	return 0;
}

// Suspends numeric core
int CFieldSimulator::SuspendCore(void)
{
	if (CMD != CMD_DEAD)
	if (CMD != CMD_PAUSE_ACK)
		{CMD = CMD_PAUSE;	while (CMD != CMD_PAUSE_ACK) Sleep(10);}
	return 0;
}

// Resumes numeric core
int CFieldSimulator::ResumeCore(void)
{
	CMD = CMD_RUN;
	return 0;
}

UINT NumericCore( LPVOID pParam )
{
	CFieldSimulator* core = (CFieldSimulator*)pParam;
	core->ProcessTick();
	return 0;
}

// Creates and initializes numeric core
int CFieldSimulator::InitializeCore(void)
{
	NumCore = AfxBeginThread(NumericCore,this,THREAD_PRIORITY_BELOW_NORMAL,0 ,NULL);
	NumCore->m_bAutoDelete = true;
	SuspendCore();

	return 0;
}

// Sets simulation state of the numerical core
int CFieldSimulator::SetSimulationState(int State)
{
	SimState = State;

	return 0;
}


void CFieldSimulator::UpdateValues(long double x, long double y)
{
	long double x2 = x; 
	long double y2 = long double(Y) - y; 
	long double x1 = oldx;
	long double y1 = oldy;
	switch (SimState)
	{	 
		case STAT_WALLS:
		{
			for (int x=-ChangeSize/2.; x < ChangeSize/2.; x++)
			for (int y=-ChangeSize/2.; y < ChangeSize/2.; y++)
			{
				if (x*x + y*y <= ChangeSize*ChangeSize/4.0)
				{
					int _x = x + x2; 
					int _y = y + y2; 
					if (_x < 0) continue; 
					if (_x > X-1) continue;
					if (_y < 0) continue; 
					if (_y > Y-1) continue;
					walls[_x+_y*X] = ChangeValue;
					surf[_x*3+_y*X*3+1] = ChangeValue * 65535.0;
				}
			}
		}break;
		case STAT_TONIC:
		{
			for (int x=-ChangeSize/2.; x < ChangeSize/2.; x++)
			for (int y=-ChangeSize/2.; y < ChangeSize/2.; y++)
			{
				if (x*x + y*y <= ChangeSize*ChangeSize/4.0)
				{
					int _x = x + x2; 
					int _y = y + y2; 
					if (_x < 0) continue; 
					if (_x > X-1) continue;
					if (_y < 0) continue; 
					if (_y > Y-1) continue;
					tonic_input[_x+_y*X] = ChangeValue;
					surf[_x*3+_y*X*3+1] = ChangeValue*1E12* 65535.0;
					surf[_x*3+_y*X*3+0] = ChangeValue*1E12* 65535.0;
				}
			}				

		}break;
		case STAT_INIT:
		{
			for (int x=-ChangeSize/2.; x < ChangeSize/2.; x++)
			for (int y=-ChangeSize/2.; y < ChangeSize/2.; y++)
				{
					
					if (x*x + y*y <= ChangeSize*ChangeSize/4.0)
					{
						int _x = x + x2; 
						int _y = y + y2; 
						if (_x < 0) continue; 
						if (_x > X-1) continue;
						if (_y < 0) continue; 
						if (_y > Y-1) continue;
						states[_x+_y*X] = ChangeValue;
						cells[_x+_y*X].S[1] = ChangeValue;
						cells[_x+_y*X].S[2] = ChangeValue;
						surf[_x*3+_y*X*3+0] = ChangeValue * 65535.0;
					}

					
				} 

		}break;

		case STAT_SIM:
		{
		}
			break;
	}
	oldx=x2;
	oldy=y2;
}

// Saves current state of simulation to file
int CFieldSimulator::SaveStateToFile(CFile &ar)
{
	SuspendCore();

	ar.Write(&X, sizeof(X));
	ar.Write(&Y, sizeof(Y));
	ar.Write(surf, sizeof(surface_type)*X*Y*3);
	ar.Write((void*)states, sizeof(field_type)*X*Y);
	ar.Write((void*)walls, sizeof(field_type)*X*Y);
	ar.Write((void*)tonic_input, sizeof(field_type)*X*Y);

	ResumeCore();


	ar.Flush();

	return 0;
}

// Restores simulation from file
int CFieldSimulator::LoadStateFromFile(CFile &ar)
{
	SuspendCore();
	
	ar.Read(&X, sizeof(X));
	ar.Read(&Y, sizeof(Y));

	if (surf)  delete [] surf;	surf = (surface_type *)new char[sizeof(surface_type)*X*Y*3]; memset((void*)surf,0,sizeof(surface_type)*X*Y*3); 
	if (states)delete [] states;	states = (field_type *)new char[sizeof(field_type)*X*Y]; memset((void*)states,0,sizeof(field_type)*X*Y); 
	if (walls) delete [] walls;	walls = (field_type *)new char[sizeof(field_type)*X*Y]; memset((void*)walls,0,sizeof(field_type)*X*Y);
	if (tonic_input) delete [] tonic_input;	tonic_input = (field_type *)new char[sizeof(field_type)*X*Y]; memset((void*)tonic_input,0,sizeof(field_type)*X*Y);

	ar.Read((void*)surf, sizeof(surface_type)*X*Y*3);
	ar.Read((void*)states, sizeof(field_type)*X*Y);
	ar.Read((void*)walls, sizeof(field_type)*X*Y);
	ar.Read((void*)tonic_input, sizeof(field_type)*X*Y);
	
	ResumeCore();
	return 0;
}

long double CFieldSimulator::GetFieldPoint(UINT x, UINT y, UINT State)
{
	return cells[x+y*X].S[State];
}

// Performs actual calculations for processing one clock tick
void CFieldSimulator::ProcessTick(void)
{                 
	int n_size;
	double phi;
	int xx;
	int yy;
	phi = 0.0;

	while(1)
	{

	// SERVICE PART   **********************************************************************************	 
			if (CMD==CMD_DEAD) {Sleep(100); continue;}

			if (CMD==CMD_RUN_ACK) CMD = CMD_RUN;
			
			if (CMD==CMD_PAUSE) 
			{
				Beep(2000,20); Sleep(20); Beep(1000,20);

				CMD = CMD_PAUSE_ACK;
				while ((CMD != CMD_RUN)&&(CMD != CMD_EXIT)) {Sleep(100);}
				
				Beep(1000,20); Sleep(20); Beep(2000,20);

				n_size = sizeof(field_type)*X*Y;

				xx = X/2;
				yy = Y/2;
			}
			
			if (CMD==CMD_EXIT) 
			{
				for ( int i=0; i<5; i++ ) {Beep(2000-i*400,10); Sleep(20);}
				
				CMD = CMD_EXIT_ACK;
				while (CMD != CMD_EXIT_ACK_ACK) Sleep(10);
				return;
			}

	// SIMULATION PART **********************************************************************************        
	// S[0] - input to the given cell
	// S[1] - V parameter of FN model
	// S[2] - W parameter of FN model
	// 1 Step. Calculating inputs to each cell.
	// I = SUM( w_ij * SUM alpha(t-T_j-d_j)  )
	for (int x=0; x < X; x++)
	    for (int y=0; y < Y; y++)
		{	
			int IDX = x+y*X;

			//for each cell (x,y) ...
			long double I = 0.0;
			long double E = cells[IDX].S[1]; //membrane potential of the isopotential cell
			long double Ee = 0.0;		 //reversal potential for the excitatory connections
			int AFR = cells[IDX].AFRadius;
			int cnt=0;
			for(int k=-AFR; k<AFR; k++)
			for(int l=-AFR; l<AFR; l++)
			{
				// for every input to given cell...
				int _x = k + AFR; //|
				int _y = l + AFR; //|- coordinates in local weight matrix of each cell
				if (_x < 0) continue; 
				if (_x > X-1) continue;
				if (_y < 0) continue; 
				if (_y > Y-1) continue;
				long double weight = cells[IDX].W[int(_x+_y*AFR*2)];
				if (!weight) continue; // if zero weight, then no transmission, so move over...
				
				//... 1. Calculate mPSP created by given (xk,yl) presynaptic cell...
				int xk = x + k; //|
				int yl = y + l; //|- coordinates of neigbouring cells in a global field coordinates
				if (xk < 0) continue; 
				if (xk > X-1) continue;
				if (yl < 0) continue; 
				if (yl > Y-1) continue;
				int idx  = xk + yl*X;
				int SpikeN = cells[idx].ST_n;
				long double delay = cells[IDX].D[int(_x+_y*AFR*2)]; //synaptic delay for given presynaptc cell
				long double mI = 0.0;
				
				//go through the array of spike times in given presynaptic cell
				long double t_old = 0.0;
				for (int _t=0; _t < SpikeN; _t++)
				{
					// take the absolute spike time, calculate the relative time, substruct the syn. delay and calculate the mPSP
					long double t= AlphaFunc( dT*(Ticks - cells[idx].SpikeTimes[_t] - delay ));
					if (t<0)
					{int o=0;}
					// if we are going down along alpha function
					if ((t< t_old)&& 
					// and value of function is small, then we consider that this mEPSC has gone and we remove corresponding spike from the pr4ecynaptic cell
					(t <0.01) )
					{
						//	memcpy((void*)cells[xk + yl*X].SpikeTimes[_t],(void*)cells[xk + yl*X].SpikeTimes[_t+1],sizeof(TSpike)*(SpikeN - _t - 1));
						for (int k=0; k<SpikeN - _t - 1;k++)
							cells[idx].SpikeTimes[_t+k] = cells[idx].SpikeTimes[_t+k+1];
						cells[idx].ST_n--;
					}
					t_old = t;
					t=t*(E-Ee)*weight;
					mI += t;
					//TRACE2("%f - %f\n",t_old, t);
				}
				cnt++;
				I += mI; // add the signal from given (xk,yl) presynaptic cell to the total signal to postsynaptic (x,y) cell			
			}
			cells[IDX].S[0] = -I + tonic_input[IDX]; //assign PSP value to the first state variable of the postsynaptic (x,y) cell`
		}
		// 2 Step. Calculating response of the each cell.
	/*
		#define C		0.281L		//membraine capacitance 281 pF
		#define gl		0.03L //0.3		//leak conductance 30nS
		#define	El		-70.6L	//leak reversal potential -70.6mV
		#define	Vt		-50.4L	//spike threshhold -50.4mV
		#define	delta_t	2.0L	//slope factor 2mV
		#define	tau_w	144.0L	//adaptation time constant 144ms
		#define	a		0.004L //0.004		//subthreshold adaptation 4nS
		#define	b		0.0805L	//spike-triggered adaptation 0.0805 nA
	*/ 
		// m -3 mk -6 n -9 p -12
		#define C	281.E-12L	//membraine capacitance 281 pF
		#define gl	30.E-9L 	//0.3		//leak conductance 30nS
		#define	El	-70.6E-3L	//leak reversal potential -70.6mV
		#define	Vt	-50.4E-3L	//spike threshhold -50.4mV
		#define	delta_t	2.E-3L		//slope factor 2mV
		#define	tau_w	144.E-3L	//adaptation time constant 144ms
		#define	a	4.E-9L 		//0.004		//subthreshold adaptation 4nS
		#define	b	0.0805E-9L	//spike-triggered adaptation 0.0805 nA

		for (int x=0; x < X; x++)
		for (int y=0; y < Y; y++)
		{
			int IDX = x+y*X;
			//for each cell (x,y) ...
			long double I = cells[IDX].S[0]; // input to the given cell
			long double V = cells[IDX].S[1]; //membrane potential of the isopotential cell
			long double W = cells[IDX].S[2]; //recovery variable
			
			if (V == 0.030L)
			{
				// doing reset of neuron dynamics
				V = El;
				W = W + b;
				
				// processing spike event
				int h = cells[IDX].ST_n;
				if (h >= cells[IDX].SpikeSlotsAllocated - 1 )
				{
					//reallocating slots
					cells[IDX].SpikeSlotsAllocated+=SpikeN_Inc;
					TSpike * t = new TSpike[cells[IDX].SpikeSlotsAllocated];
					memcpy(t,cells[IDX].SpikeTimes,sizeof(TSpike)*(cells[IDX].SpikeSlotsAllocated - SpikeN_Inc));
					delete [] cells[IDX].SpikeTimes;
					cells[IDX].SpikeTimes = t;
				}

				cells[IDX].SpikeTimes[cells[IDX].ST_n++] = Ticks;
				//cells[IDX].SpikeTimes[cells[IDX].ST_n++].Cnt = 
			}
			else
			{
			long double	VF = (-gl*(V - El) + gl*delta_t*exp( (V-Vt)/delta_t ) - W + I )/C;
			long double	V1 = V + dT*VF;
			long double	VF1 = (-gl*(V1 - El) + gl*delta_t*exp( (V1-Vt)/delta_t ) - W + I )/C;
					
			long double	WF = (a*(V - El) - W)/tau_w;
			long double	W1 = W+  dT*WF;
			long double	WF1 = (a*(V - El) - W1)/tau_w;
				
					V = V + dT*(VF + VF1)/2.L;
					W = W + dT*(WF + WF1)/2.L;
			}
			
			if (V > 0.030L)	V = 0.030L;

			surf[x*3+y*X*3] = (V+0.090)*546125;
			if (V>cells[IDX].max) cells[IDX].max = V;
			if (V<cells[IDX].min) cells[IDX].min = V;
			cells[IDX].S[1] = V;
			cells[IDX].S[2] = W;
		}
		// 3 Step. Do a tick.
		Ticks++;
				       
	// SERVICE PART **************************************************************************************	  	 
	      // evoke callback function to render the field
	      (*RenderCallBack)(pContext);
	      Sleep(10);
	};
	AfxMessageBox("If you see this message, some serious shit has happend!");
	//pDoc->bRun = -1;
	return;
}
// retrieves current command state of numeric core
int CFieldSimulator::GetCommand(void)
{
	return CMD;
}

// shuts down the numeric core underlying thread
int CFieldSimulator::TerminateCore(void)
{
	TRACE0("Setting termination flag...\n");

	CMD=CMD_EXIT;

	while (CMD != CMD_EXIT_ACK) Sleep(10);

	CMD = CMD_DEAD;

	TRACE0("Numeric core is supposed to finish...\n");
	return 0;
}

// Test function, not supposed to be in this class
int CFieldSimulator::TestShowInternalData(long double x, long double y, int *Cmd)
{
	long double x2 = x; 
	long double y2 = long double(Y) - y; 
	long double x1 = oldx;
	long double y1 = oldy;

	switch (*Cmd)
	{
	case TEST_NO_COMMAND:
	{
		long double R2 = AfferentFieldRadius*AfferentFieldRadius;
			for(int k=-AfferentFieldRadius; k<AfferentFieldRadius; k++)
			for(int l=-AfferentFieldRadius; l<AfferentFieldRadius; l++)
			{
				if (k*k + l*l <= R2)
					{
						int _x = k + oldx; 
						int _y = l + oldy; 
						if (_x < 0) continue;
						if (_x > X-1) continue;
						if (_y < 0)continue;
						if (_y > Y-1) continue;
						surf[_x*3+_y*X*3+2] = 0;
					}
			}				
	} break;

	case TEST_SHOW_WEIGHTS:
	{
		long double R2 = AfferentFieldRadius*AfferentFieldRadius;
			for(int k=-AfferentFieldRadius; k<AfferentFieldRadius; k++)
			for(int l=-AfferentFieldRadius; l<AfferentFieldRadius; l++)
			{
				if (k*k + l*l <= R2)
					{
						int _x = k + oldx; 
						int _y = l + oldy; 
						if (_x < 0) continue;
						if (_x > X-1) continue;
						if (_y < 0)continue;
						if (_y > Y-1) continue;
						surf[_x*3+_y*X*3+2] = 0;
					}
			}				
			for( k=-AfferentFieldRadius; k<AfferentFieldRadius; k++)
			for(int l=-AfferentFieldRadius; l<AfferentFieldRadius; l++)
			{
				if (k*k + l*l <= R2)
					{
						int _x = k + x2; 
						int _y = l + y2; 
						if (_x < 0) continue;
						if (_x > X-1) continue;
						if (_y < 0) continue;
						if (_y > Y-1) continue;
						surf[_x*3+_y*X*3+2] = 65535.0* cells[int(x2+y2*X)].W[int(k+AfferentFieldRadius+(l+AfferentFieldRadius)*AfferentFieldRadius*2)];
					}
			}
	} break;
	}


	return 0;
}

void CFieldSimulator::SetAfferentFieldParams(long double edb, long double ede, long double ewb, long double ewe, long double ed_noise, long double ew_noise, long double radius)
{
	exp_weight_begin	= ewb;					// waight of input at the center of the receptive field
	exp_weight_end		= ewe;						// waight of input at the very edge of the receptive field
	exp_delay_begin		= edb;					// delay of input at the center of the receptive field
	exp_delay_end		= ede;				// delay of input at the very edge of the receptive field	
	weight_noise		= ew_noise;				// amplitude of gaussian noise for weights
	delay_noise			= ed_noise;				// amplitude of gaussian noise for delays
	AfferentFieldRadius = radius;				// Radius of afferent field around the given cell

	return;
}

void CFieldSimulator::GetAfferentFieldParams(float &edb, float &ede, float &ewb, float &ewe, float &ed_noise, float &ew_noise, float &radius)
{
	ewb = (float)exp_weight_begin;					// waight of input at the center of the receptive field
	ewe = (float)exp_weight_end;						// waight of input at the very edge of the receptive field
	edb = (float)exp_delay_begin;					// delay of input at the center of the receptive field
	ede = (float)exp_delay_end;				// delay of input at the very edge of the receptive field	
	ew_noise = (float)weight_noise;				// amplitude of gaussian noise for weights
	ed_noise = (float)delay_noise;				// amplitude of gaussian noise for delays
	radius = (float)AfferentFieldRadius;				// Radius of afferent field around the given cell

	return;
}

//1.��������� ������� ������� �����-�������
//2.��������, ������ ��������� ������������� �������� ������� 
inline long double CFieldSimulator::AlphaFunc(long double t)
	{
		#define ts  15.0L
		#define tr  10.0L
		#define q	1.0L
		//TRACE1("%f\n",float(t));
		return t > 0.0L ? q/(ts-tr)*(exp((long double)(-t/ts)) - exp((long double)(-t/tr))) : 0.0L;
	}