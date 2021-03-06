/* 
 * Audrey Bender
 */

#include "neurone.hpp" 
#include <assert.h>
#include <random>


using namespace std; 


//Constructeur
Neurone::Neurone( long double membranePotential, unsigned long timeSpikes,
				double currentExt,unsigned long numberSpikes,
				unsigned long clock, double refStep )
				: membranePotential(0.0), timeSpikes(0),
				  currentExt(0.0), numberSpikes(0), clock(0)
			
		{
			refStep = ( static_cast<const unsigned long>(refTime / h) );
			//Initialization of the spike buffer
			for ( auto& element : buffer) {
				element = 0; 
			} 
		}
		
//Destructeur 
Neurone::~Neurone(){}

//Getters
long double Neurone::getPotential() const {
	return membranePotential; 
}

double Neurone::getNumberSpikes() const {
	return numberSpikes;
}
double Neurone::getTimeSpikes() const {
	return timeSpikes; 
}
double Neurone::getCurrentExt() const {
	return currentExt;
}

double Neurone::getClock() const {
	return clock;
}
//Setter
void Neurone::setPotential( long double p) {
	membranePotential = p; 
}
void Neurone::setCurrentExt( double c) {
	currentExt = c; 
}

void Neurone::setNumberSpikes( int n) {
	numberSpikes = n;
}
//Méthodes

bool Neurone::update(unsigned long time) {
	
        if ( time == 0) {
			return false; 
		}
			
		bool spike(false);
		unsigned long tStop = clock + time; 
		
		
		while( clock < tStop) 
		{		
			unsigned long tArrival = clock % (delayStep + 1); //convert in steps
			
			if ( getPotential() > Vteta) { //If neurone spikes
			 numberSpikes += 1;
			 spike = true; 
			 timeSpikes = clock; 
			}
			
		    if ( (timeSpikes > 0) and ( (clock-timeSpikes) <= refStep/h) ){
			   //Refractory time of the neuron after a spike 
			   setPotential(0.0); 
		    }else{
				
			assert( tArrival < buffer.size() );
			long double potential = getPotential();
			setPotential( exp(-h/tao) * potential +  R*(1-exp(-h/tao)) * getCurrentExt() 
			              + buffer[tArrival] );                         
			//set potential depending on the buffer content (given spikes)
		}
		

		buffer[tArrival] = 0.0; // Clear spike buffer at tArrival 
		clock += 1; 
		}
		
	return spike;
}

void Neurone::receive(unsigned long clockDelay, long double j)
{
	const size_t tSpike = (clockDelay%(delayStep + 1)); //Change in steps
	buffer[tSpike] += j; 
}
