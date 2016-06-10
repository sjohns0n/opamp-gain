#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

double nonInvertingGain(int range, int i, int j);
double invertingGain(int range, int i, int j);

double E6[6] = {1.0, 1.5, 2.2,
				3.3, 4.7, 6.8 };
				
double E12[12] = {1.0, 1.2, 1.5,
				  1.8, 2.2, 2.7,
				  3.3, 3.9, 4.7,
				  5.6, 6.8, 8.2 };
				
double E24[24] = {1.0, 1.1, 1.2,
				  1.3, 1.5, 1.6,
				  1.8, 2.0, 2.2,
				  2.4, 2.7, 3.0,
				  3.3, 3.6, 3.9,
				  4.3, 4.7, 5.1,
				  5.6, 6.2, 6.8,
				  7.5, 8.2, 9.1 }; 	

double E48[48] = {1.00, 1.05, 1.10,
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
				
double E96[96] = {1.00, 1.02, 1.05, 1.07, 1.10, 1.13, 1.15, 1.18, 
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
		double tolerance;
		int foundSuitable = 0;
		int results = 0;
		double gain = 0, calculatedGain = 0;
		double error = 0;
		int i, j;
		
		printf("Inverting (i) / Non-Inverting (n): ");
		scanf("%c", &gainType);
		if(gainType != 'i' && gainType != 'n') {
				printf("Invlaid input.\n");
		}
		
		printf("Please enter a gain: ");
		scanf("%lf", &gain);
		if(gain <= 0) {
				printf("Gain must be > 0\n");
				exit(EXIT_FAILURE);
		}
		
		printf("Please enter a percentage tolerance for the gain: ");
		scanf("%lf", &tolerance);
		
		printf("Please enter standard resistor range to use:\n");
		printf("E6, E12, E24, E48, or E96.\n");
		scanf("%s", range);
		// Allows E96 or 96 as input
		if(isalpha(range[0])) {
				rangeNum = atoi(range+1); // Get numeric value of range
		} else {
				rangeNum = atoi(range);
		}
		
		error = gain / (100/tolerance); 
		
		for(i = 0; i < rangeNum; i++) {
				for(j = 0; j < rangeNum; j++) {
						if(gainType == 'n') {
								calculatedGain = nonInvertingGain(rangeNum, i, j);
						} else {
								calculatedGain = invertingGain(rangeNum, i, j);
						}
						if(calculatedGain <= (gain + error) &&
								calculatedGain >= (gain - error)) {
								foundSuitable = 1;
								results += 1;
								printf("Gain = %.2lf, R1 = %.2lf, R2 = %.2lf\n", calculatedGain, E96[i], E96[j]);
								break;
						}
				}
		}
		
		if(foundSuitable == 0) {
				printf("Could not find any suitable resistor combinations");
		} else {
				printf("Results found: %d", results);
		}
		//printf("sizeof E96: %d\n  sizeof E96/sizeE96[0]: %d\n", sizeof(E96), sizeof(E96) / sizeof(E96[0]));
		
		return 0;
}

/* 
	Calculates the gain of an inverting amplifier circuit, where
	r2 is the feedback resistor and r1 is the input resistor on the inverting input. 
*/
double
invertingGain(int range, int i, int j) {
		double gain = 0;
		double r1, r2;
		
		switch(range) {
		case 6:
				r1 = E6[i];
				r2 = E6[j];
				break;
		case 12:
				r1 = E12[i];
				r2 = E12[j];
				break;
		case 24:
				r1 = E24[i];
				r2 = E24[j];
				break;
		case 48:
				r1 = E48[i];
				r2 = E48[j];
				break;
		case 96:
				r1 = E96[i];
				r2 = E96[j];
				break;	
		default:
				return 0;
		}
		
		gain = r2 / r1;		
		return gain;
}

/* 
	Calculates the gain of a non inverting amplifier circuit, where
	r2 is the feedback resistor and r1 is the pulldown resistor on the inverting input. 
*/
double
nonInvertingGain(int range, int i, int j) {
		double gain = 0;
		double r1, r2;
		
		switch(range) {
		case 6:
				r1 = E6[i];
				r2 = E6[j];
				break;
		case 12:
				r1 = E12[i];
				r2 = E12[j];
				break;
		case 24:
				r1 = E24[i];
				r2 = E24[j];
				break;
		case 48:
				r1 = E48[i];
				r2 = E48[j];
				break;
		case 96:
				r1 = E96[i];
				r2 = E96[j];
				break;	
		default:
				return 0;
		}
		
		gain = 1+ (r2 / r1);		
		return gain;
}