#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

double nonInvertingGain(const double* rangeAddr, int i, uint32_t multiplier1, int j, uint32_t multiplier2);
double invertingGain(const double* rangeAddr, int i, uint32_t multiplier1, int j, uint32_t multiplier2);

const double E6[6] = {1.0, 1.5, 2.2,
					  3.3, 4.7, 6.8 };
				
const double E12[12] = {1.0, 1.2, 1.5,
				        1.8, 2.2, 2.7,
				        3.3, 3.9, 4.7,
				        5.6, 6.8, 8.2 };
				
const double E24[24] = {1.0, 1.1, 1.2,
				        1.3, 1.5, 1.6,
				        1.8, 2.0, 2.2,
				        2.4, 2.7, 3.0,
				        3.3, 3.6, 3.9,
				        4.3, 4.7, 5.1,
				        5.6, 6.2, 6.8,
				        7.5, 8.2, 9.1 }; 	

const double E48[48] = {1.00, 1.05, 1.10,
				        1.15, 1.21, 1.27,
				        1.33, 1.40, 1.47,
				        1.54, 1.62, 1.69,
				        1.78, 1.87, 1.96,
				        2.05, 2.15, 2.26,
				        2.37, 2.49, 2.61,
				        2.74, 2.87, 3.01,
				        3.16, 3.32, 3.48,
				        3.65, 3.83, 4.02,
				        4.22, 4.42, 4.64,
				        4.87, 5.11, 5.36,
				        5.62, 5.90, 6.19,
				        6.49, 6.81, 7.15,
				        7.50, 7.87, 8.25,
				        8.66, 9.09, 9.53 }; 			
				
const double E96[96] = {1.00, 1.02, 1.05, 1.07, 1.10, 1.13, 1.15, 1.18, 
				        1.21, 1.24, 1.27, 1.30, 1.33, 1.37, 1.40, 1.43, 
				        1.47, 1.50, 1.54, 1.58, 1.62, 1.65, 1.69, 1.74, 
				        1.78, 1.82, 1.87, 1.91, 1.96, 2.00, 2.05, 2.10, 
				        2.15, 2.21, 2.26, 2.32, 2.37, 2.43, 2.49, 2.55, 
				        2.61, 2.67, 2.74, 2.80, 2.87, 2.94, 3.01, 3.09, 
				        3.16, 3.24, 3.32, 3.40, 3.48, 3.57, 3.65, 3.74, 
				        3.83, 3.92, 4.02, 4.12, 4.22, 4.32, 4.42, 4.53, 
				        4.64, 4.75, 4.87, 4.99, 5.11, 5.23, 5.36, 5.49, 
				        5.62, 5.76, 5.90, 6.04, 6.19, 6.34, 6.49, 6.65, 
				        6.81, 6.98, 7.15, 7.32, 7.50, 7.68, 7.87, 8.06, 
				        8.25, 8.45, 8.66, 8.87, 9.09, 9.31, 9.53, 9.76 };

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
		printf("E6, E12, E24, E48, or E96.\n");
		scanf("%s", range);
		
		if(isalpha(range[0])) {
				rangeNum = atoi(range+1); // Get numeric value of range
		} else {
				rangeNum = atoi(range);
		}
		
		const double* rangeAddr;
		
		switch(rangeNum) {
		case 6:
				rangeAddr = E6;
				break;
		case 12:
				rangeAddr = E12;
				break;
		case 24:
				rangeAddr = E24;
				break;
		case 48:
				rangeAddr = E48;
				break;
		case 96:
				rangeAddr = E96;
				break;	
		default:
				return 0;
		}
		
		/* Iterate through all options, select one with lowest error */
		double lowestError = 1000;		
		double gainError;
		double optimalGain = 0;
		
		double multiplierMax = 100;
		
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
												printf("error\n\n");
												//calculatedGain = invertingGain(rangeAddr, r1Index, r1Multiplier, r2Index, r2Multiplier);
										}
										
										gainError = desiredGain - calculatedGain;
										if(fabs(gainError) < lowestError) {
												lowestError = gainError;
												r1Optimal = rangeAddr[r1Index] * r1Multiplier;
												r2Optimal = rangeAddr[r2Index] * r2Multiplier;
												optimalGain = calculatedGain;
										 }
										
										#ifdef DEBUG
										printf("r1: %.1f, r2: %.1f   ", rangeAddr[r1Index], rangeAddr[r2Index]);
										printf("calculated gain: %8.4f  gain error %+8.4f\n", calculatedGain, gainError);
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
		printf("Desired gain     : %.4f\n", desiredGain);
		printf("Resistor range   : E%d\n", rangeNum);
		printf("\n");
		printf("\n");
		
		printf("Iterations: %ld\n", iterationNumber);
		printf("Lowest gain error: %.4f\n", lowestError);
		
		printf("\n");
		printf("r1 optimal:      %9.1f\n", r1Optimal);
		printf("r2 optimal:      %9.1f\n", r2Optimal);
		printf("calculated gain: %9.1f\n", optimalGain);
		
		return EXIT_SUCCESS;
}

/* 
	Calculates the gain of an inverting amplifier circuit, where
	r2 is the feedback resistor and r1 is the input resistor on the inverting input. 
*/
double
invertingGain(const double* rangeAddr, int i, uint32_t multiplier1, int j, uint32_t multiplier2) {
		double gain = 0;
		double r1, r2;
		
		r1 = rangeAddr[i] * multiplier1;
		r2 = rangeAddr[j] * multiplier2;
		
		gain = (r2 / r1);		
		return gain;
}

/* 
	Calculates the gain of a non inverting amplifier circuit, where
	r2 is the feedback resistor and r1 is the pulldown resistor on the inverting input. 
*/
double
nonInvertingGain(const double* rangeAddr, int i, uint32_t multiplier1, int j, uint32_t multiplier2) {
		double gain = 0;
		double r1, r2;
		
		r1 = rangeAddr[i] * multiplier1;
		r2 = rangeAddr[j] * multiplier2;		
		
		#ifdef DEBUG
		printf("inside gain block: r1 = %.1f, r2 = %.1f", r1, r2);
		printf(" r2 / r1 = %.2f", r2 / r1);
		printf("   1.0 + (r2 / r1) = %.2f\n\n", 1.0 + (r2 / r1));
		#endif
		
		gain = 1.0 + (r2 / r1);		
		return gain;
}
