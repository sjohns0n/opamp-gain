#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include "resistorSeries.h"

double nonInvertingGain(double* rangeAddr, int r1Index, uint32_t multiplier1, int r2Index, uint32_t multiplier2);
double invertingGain(double* rangeAddr, int i, uint32_t multiplier1, int j, uint32_t multiplier2);

int
main(int argc, char **argv) {
	char range[4], gainType;
	int rangeNum = 0;
	double desiredGain = 0, calculatedGain = 0;
	
	printf("Inverting (i) / Non-Inverting (n): ");
	//TODO: safer input handling
	scanf("%c", &gainType);
	if(gainType != 'i' && gainType != 'n') {
		printf("Invlaid input.\n");
	}
	
	printf("Please enter a gain: ");
	//TODO: safer input handling
	scanf("%lf", &desiredGain);
	if(desiredGain <= 0) {
		printf("Gain must be > 0\n");
		exit(EXIT_FAILURE);
	}
	
	//TODO: Tolerances
	
	printf("Please enter standard resistor range to use:\n");
	printf("E6, E12, E24, E48, or E96: ");
	scanf("%s", range);
	
	if(isalpha(range[0])) {
		rangeNum = atoi(range+1); // Get numeric value of range
	} else {
		rangeNum = atoi(range);
	}
	
	double* rangeAddr = getSeriesAddr(rangeNum);
		
	/* Iterate through all options, select one with lowest error */
	double lowestError = 1000;		
	double gainError;
	double absGainError;
	double optimalGain = 0;
	
	double multiplierMax = 1e7;
	
	uint64_t iterationNumber = 0;
	uint32_t r1Multiplier = 1,
		     r2Multiplier = 1;
	
	uint8_t r1Index,
			r2Index;
	
	double r1Optimal,
		   r2Optimal;
	
	// brute force finding the closest pair
	for(r1Index = 0; r1Index < rangeNum; r1Index++) {
		for(r2Index = 0; r2Index < rangeNum; r2Index++) {
			for(r1Multiplier = 1; r1Multiplier <= multiplierMax; r1Multiplier *= 10) {
				for(r2Multiplier = 1; r2Multiplier <= multiplierMax; r2Multiplier *= 10) {
					if(gainType == 'n') {
						calculatedGain = nonInvertingGain(rangeAddr, r1Index, r1Multiplier, r2Index, r2Multiplier);
					} else {
						calculatedGain = invertingGain(rangeAddr, r1Index, r1Multiplier, r2Index, r2Multiplier);
					}
					
					gainError = desiredGain - calculatedGain;
					absGainError = fabs(gainError);
					if(absGainError < lowestError) {
						lowestError = absGainError;
						r1Optimal = rangeAddr[r1Index] * r1Multiplier;
						r2Optimal = rangeAddr[r2Index] * r2Multiplier;
						optimalGain = calculatedGain;
					}
					
					#ifdef DEBUG
					printf("r1: %.1f, r2: %.1lf   ", rangeAddr[r1Index], rangeAddr[r2Index]);
					printf("calculated gain: %8.4lf  gain error %+8.4f\n", calculatedGain, gainError);
					#endif
					//printf("%d\n", iterationNumber);
					iterationNumber++;
				}
			}
		}
	}
	
	printf("\n");
	printf("Search parameters: \n");
	printf("Inv./ Non-inv.   : %c\n", gainType);
	printf("Desired gain     : %.4lf\n", desiredGain);
	printf("Resistor range   : E%d\n", rangeNum);
	printf("\n");
	printf("\n");
	
	#ifdef DEBUG
	printf("Iterations: %ld\n", iterationNumber);
	printf("Lowest gain error: %.4lf\n", lowestError);
	printf("\n");
	#endif
	
	printf("r1 optimal:      %9.1lf ohms\n", r1Optimal);
	printf("r2 optimal:      %9.1lf ohms\n", r2Optimal);
	printf("calculated gain: %10.2lf\n", optimalGain);
	printf("gain error:      %+10.2lf%%\n", (optimalGain / desiredGain) * 100 - 100);
	
	return 0;
}

/* 
Calculates the gain of an inverting amplifier circuit, where
r2 is the feedback resistor and r1 is the input resistor on the inverting input. 
*/
double
invertingGain(double* rangeAddr, int r1Index, uint32_t multiplier1,
								 int r2Index, uint32_t multiplier2) {
	double gain = 0;
	double r1, r2;
	
	r1 = rangeAddr[r1Index] * multiplier1;
	r2 = rangeAddr[r2Index] * multiplier2;
	
	gain = (r2 / r1);		
	return gain;
}

/* 
Calculates the gain of a non inverting amplifier circuit, where
r2 is the feedback resistor and r1 is the pulldown resistor on the inverting input. 
*/
double
nonInvertingGain(double* rangeAddr, int r1Index, uint32_t multiplier1,
    						        int r2Index, uint32_t multiplier2) {
	double gain = 0;
	double r1, r2;
	
	r1 = rangeAddr[r1Index] * multiplier1;
	r2 = rangeAddr[r2Index] * multiplier2;		
	
	#ifdef DEBUG
	printf("inside gain block: r1 = %.1f, r2 = %.1f", r1, r2);
	printf(" r2 / r1 = %.2f", r2 / r1);
	printf("   1.0 + (r2 / r1) = %.2f\n\n", 1.0 + (r2 / r1));
	#endif
	
	gain = 1.0 + (r2 / r1);		
	return gain;
}
