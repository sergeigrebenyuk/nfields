#pragma once

#define surface_type unsigned __int16
#define field_type long double
#define LUINT unsigned __int64

enum {	// regimes of simulator
		STAT_INIT,		// initialization
		STAT_SIM,		// simulation itself
		STAT_WALLS,		// setting walls
		STAT_TONIC};	// setting tonic inputs

enum {	CMD_EXIT,			// exit 
		CMD_EXIT_ACK,		// exit ack. by numeric core
		CMD_EXIT_ACK_ACK,	// exit exponse ack. by CField 
		CMD_PAUSE,			// pause
		CMD_PAUSE_ACK,		// pause ack
		CMD_RUN,			// run
		CMD_RUN_ACK,		// run ack
		CMD_DEAD};			// initial 'dead' state of numeric core
/*
typedef struct
{
	unsigned __int64 nTick;
	unsigned __int32 Cnt;
} TSpike;
*/
#define TSpike unsigned __int64

typedef struct
{
	__int32		X;			// X coordinate on lattice
	__int32		Y;			// Y coordinate on lattice
	long double*		W;			// connection weights matrix (synaptic connections, density of flow between cells, etc.)
	long double*		D;			// transmission delays (synaptic delays, etc.)
	__int8		AFRadius;	// radius of afferent (receptive) field around the given cell (for heterogenous fields)
	long double*		S;			// State variables(slow,fast, etc. - for RD systems, tabled activation function for neurons)
	__int8		S_n;		// size of S (for heterogenous fields)
	bool		Fired;		// true, if cell is in excited state (ready to transmit signal or energy)
	bool		AffCounter;	// counter of fired afferent cells
	TSpike*		SpikeTimes;	// array of spike times in a presynaptic burst
	UINT		ST_n;		// size of SpikeTimes
	long double		min;
	long double		max;
	bool		AP_proc;
	int			SpikeSlotsAllocated;				// number of allocated slots for generated spikes
	//long double	dT;			// time step for flexible stepping (large steps for slow evolving cells)
} TCell;

typedef void (TFunc)(void*);

class CFieldSimulator
{
private:
	//simulation related
	volatile TCell*					cells;
	volatile field_type*			states;								// state field
	volatile field_type*			walls;								// walls in the neuronal field
	volatile field_type*			tonic_input;						// tonic excitation/inhibition in the neuronal field
	
	long double					exp_weight_begin;					// waight of input at the center of the receptive field
	long double					exp_weight_end;						// waight of input at the very edge of the receptive field
	long double					exp_delay_begin;					// delay of input at the center of the receptive field
	long double					exp_delay_end;						// delay of input at the very edge of the receptive field	
	long double					weight_noise;						// amplitude of gaussian noise for weights
	long double					delay_noise;						// amplitude of gaussian noise for delays
	long double					AfferentFieldRadius;				// Radius of afferent field around the given cell
	//engine related
	void*					pContext;							// pointer to a container class - context for the callback function
	TFunc*					RenderCallBack;						// Callback function called by container to render the field
	volatile __int32		CMD;								// Command slot (communication with Numeric Core)
	CWinThread*				NumCore;							// class of the numeric core thread	

public:
	//simulation related
	UINT					X,Y;
	volatile UINT		Ticks;								// Number of simulation steps
	long double					dT;									// time step of the simulation
	int						SpikeN_Inc;							// Increment for reallocating memory for spike times in a Cell
	//engine related
	int						oldx,oldy;							// don't remember	
	UINT					SimState;							// current state of simulations
	long double					ChangeValue;						// Value newly set for the region of field pointed by user
	long double					ChangeSize;							// Size of the region
	surface_type*			surf;								// displayed filed		


	
	CFieldSimulator(void);

	virtual ~CFieldSimulator(void);
	// Restarts numeric core
	int RestartCore(void);
	// Suspends numeric core
	int SuspendCore(void);
	// Resumes numeric core
	int ResumeCore(void);
	// Creates and initializes numeric core
	int InitializeCore(void);
	int SetSimulationState(int State);
	void UpdateValues(long double x, long double y);
	// sets callback function and its context
	void SetRenderingCallback(void *context, TFunc* func) {pContext = context; RenderCallBack = func;};
	// Saves current state of simulation to file
	int SaveStateToFile(CFile &ar);
	// Restores simulation from file
	int LoadStateFromFile(CFile &ar);
	long double GetFieldPoint(UINT x, UINT y, UINT State);
	// Performs actual calculations for processing one clock tick
	void ProcessTick(void);
	// retrieves current command state of numeric core
	int GetCommand(void);
	// shuts down the numeric core underlying thread
	int TerminateCore(void);
	
	// Test function, not supposed to be in this class
	int TestShowInternalData(long double x, long double y, int* Cmd);
	void SetAfferentFieldParams(long double edb, long double ede, long double ewb, long double ewe, long double ed_noise, long double ew_noise, long double radius);
	void GetAfferentFieldParams(float &edb, float &ede, float &ewb, float &ewe, float &ed_noise, float &ew_noise, float &radius);

	long double AlphaFunc(long double t);
	
};

UINT NumericCore( LPVOID pParam);	
void CallBack( LPVOID pParam);	