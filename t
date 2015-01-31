#define C               281.0E-12               //membraine capacitance 281 pF
#define gl              30.0E-9         //leak conductance 30nS
#define El              -70.6E-3        //leak reversal potential -70.6mV
#define Vt              -50.4E-3        //spike threshhold -50.4mV
#define delta_t 2.0E-3          //slope factor 2mV
#define tau_w   144.0E-3        //adaptation time constant 144ms
#define a               4.0E-9          //subthreshold adaptation 4nS
#define b               0.0805E-9       //spike-triggered adaptation 0.0805 nA

                // a < -eg
                for (int x=0; x < X; x++)
                for (int y=0; y < Y; y++)
                {
                        //for each cell (x,y) ...
                        float I = cells[x+y*X].S[0]; // input to the given cell
                        float V = cells[x+y*X].S[1]; //membrane potential of the isopotential cell
                        float W = cells[x+y*X].S[2]; //recovery variable
                        
                        //V += dT*( 0.04*V*V + 5*V + 140 - W + I);
                        //W += dT*( a*(b*V - W)                                                 );
                        V += dT/C*(-gl*(V - El) + gl*delta_t*exp( (V-Vt)/delta_t ) - W + I );
                        W += dT/tau_w*(a*(V - El) - W);
                        
                        if (V >=20)
                        {
                                V = El;
                                W = W + b;
                        }
                        surf[x*3+y*X*3] = V*32767;
                        if (V>cells[x+y*X].max) cells[x+y*X].max = V;
                        if (V<cells[x+y*X].min) cells[x+y*X].min = V;
                        cells[x+y*X].S[1] = V;
                        cells[x+y*X].S[2] = W;
