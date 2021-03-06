#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "defines.h"
#include "ObjectClasses/MemoryManager.h"
#include "Utils/Simulator.h"

using namespace traceGen;
using namespace std;

FILE* gDetLog;

int NUM_THREADS ;
int NUM_CLAZZ;   
int longevity ; 
int escape ;    
int RATIO_ALLOC_SET; 
int RATIO_READ_ACCESS ;
int RATIO_STORE_ACCESS ;
int ROOT_DELETE_PROBABILITY; 	   
int MAX_POINTERS;
int MAX_PRIMITIVES;	
int STATIC_FIELD_ACCESS;
int PRIMITIVE_FIELD_ACCESS;
//int MAXCACCESS;
int ESCAPE_PROBABILITY;
int ESPACE_TO_PARTNER;


double maxWeight; // = 25.0;
int constantValue; // = 10;
double maxFrequency; // = 25.0;
double slop; //  = 0.5;


int na = 0;
int ar = 0;
int dr = 0;
int sRo = 0;
int sRc = 0;
int sPo = 0;
int sPc = 0;
int rRo = 0;
int rRc = 0;
int rPo = 0;
int rPc = 0;


/*
int main(int argc, char *argv[]) {
	//default trace file name (and path).
	char filename[80] = "traceFile.trc";
	int iterations = 0;
	switch(argc){
	case 3:
		strcpy(filename,argv[2]);
	case 2:
		iterations = atoi(argv[1]);
		break;
	default:
		fprintf(stderr, "Wrong number of arguments provided\n\n");
		fprintf(stderr, "Usage: \t./traceGen numTraceFileLines\n<OR>\n"
				"\t./traceGen numTraceFileLines outputFilename\n");
		return EXIT_FAILURE;
	}

	Simulator* sim = new Simulator(filename);
	sim->run(iterations);
	//sim->test();
	return 0;
}
*/


int setArgs(int argc, char *argv[], const char *option, const char *shortOption) {
	int i;
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], option) || !strcmp(argv[i], shortOption)) {
			if (!strcmp(option, "--operations") || !strcmp(shortOption, "-i")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--thread") || !strcmp(shortOption, "-t")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--class") || !strcmp(shortOption, "-c")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--pointers") || !strcmp(shortOption, "-p")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--primitives") || !strcmp(shortOption, "-pm")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--allocation") || !strcmp(shortOption, "-a")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--stroe") || !strcmp(shortOption, "-s")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--read") || !strcmp(shortOption, "-r")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--delrootset") || !strcmp(shortOption, "-d")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--static") || !strcmp(shortOption, "-stc")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--prifaccess") || !strcmp(shortOption, "-pfa")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--clsaccess") || !strcmp(shortOption, "-ca")) {
				return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--escape") || !strcmp(shortOption, "-e")) {
					return atoi(argv[i + 1]);
			}
			else if (!strcmp(option, "--esctopartner") || !strcmp(shortOption, "-etp")) {
					return atoi(argv[i + 1]);
			}

			else if (!strcmp(option, "--maxWeight") || !strcmp(shortOption, "-maxW")) {
					return atoi(argv[i + 1]);
			}

			else if (!strcmp(option, "--maxFrequency") || !strcmp(shortOption, "-maxF")) {
					return atoi(argv[i + 1]);
			}

			else if (!strcmp(option, "--constantValue") || !strcmp(shortOption, "-cvalue")) {
					return atoi(argv[i + 1]);
			}

			else if (!strcmp(option, "--slop") || !strcmp(shortOption, "-slp")) {
					return atoi(argv[i + 1]);
			}

			else{
				fprintf(stderr, "Some options have been set by default\n");
				return -1;
			}
		}
	}
	return -1;
}

int main(int argc, char *argv[]){


	struct timeval tv;
	unsigned long time_start,ts;
	float time;


	gettimeofday(&tv, NULL);
	time_start = 1000000*tv.tv_sec+tv.tv_usec;


	
	if(argc < 2) {
		fprintf(stderr, "Usage: TraceFileGenerator traceFile [OPTIONS]\n" \
						"  The name of trace file (as traceFile)\n" \
						"Options:\n" \
						"  --iteration x, -i x       uses x number as the number of iterations (default: 100)\n" \
						"  --thread x, -t x          uses x number as the number of threads (default: 10)\n" \
						"  --class x, -c x           uses x number as the number of classes (default: 300)\n" \
						"  --pointers x, -p x        uses x number as the maximum number of pointer fields in objects/static pointer fileds in classes (default: 10)\n" \
						"  --primitives x, -pm x     uses x number as the maximum number of primitive fields in objects/static primitive fileds in classes (default: 6)\n" \
						"  --allocation x, -a x      uses x percent as the allocation (default: 1)\n" \
						"  --storeaccess x, -s x     uses x percent as the store (default: 8)\n" \
						"  --readaccess x, -r x      uses x percent as the read (default: 80)\n" \
						"  --deleteroot x, -d x      uses x percent as the rootdelete (default: 11)\n" \
						"  --static x, -sf x         uses x percent as the static field access (default: 30)\n" \
						"  --prifaccess x, -pfa x    uses x percent as the primitive field access (default: 70)\n" \
						//"  --classaccess x, -ca x    uses x number as the maximum used a cleass to create objects (default: 300)\n" \
						"  --escape x, -e x          uses x probability as the an object to be escaped (default: 12)\n" \
						"  --esctopartner x, -etp x  uses x probability as an object to be escaped to partner thread (default: 90)\n" \
						"  -- maxWeight x, -maxW x   uses x maximum weight of a class (default: 25)\n" \
						"  -- maxFrequency x, -maxF x   uses x maximum frequency of a class in object allocation (default: 25)\n" \
						"  --constantValue x, -cvalue x  uses x constant for a where {y = ab^x} (default: 10)\n" \
						"  --slop x, -slp x  uses x tangent of a line value between 0 t0 1 (default: b=.5)\n" \

						);
		exit(1);
	}

		fprintf(stderr, "TraceFileGenerator v-%d\n\n", VERSION);
		

		char filename[100];
		strcpy(filename, argv[1]);
		strcat(filename, ".trace");
		
		char clsfilename[100];
		strcpy(clsfilename, argv[1]);
		strcat(clsfilename, ".cls");

		char logFile[100];
		strcpy(logFile, argv[1]);
		strcat(logFile, ".log");
		gDetLog = fopen(logFile,"w+");

		 int OPERATIONS = setArgs(argc, argv, "--operations",  "-i");
		 NUM_THREADS      = setArgs(argc, argv, "--thread",  "-t");
		 NUM_CLAZZ       = setArgs(argc, argv, "--class",  "-c");
		 MAX_POINTERS = setArgs(argc, argv, "--pointers", "-p");
		 MAX_PRIMITIVES = setArgs(argc, argv, "--primitives", "-pm");
		 RATIO_ALLOC_SET = setArgs(argc, argv, "--allocation",  "-a");
		 RATIO_READ_ACCESS  		= setArgs(argc, argv, "--read",  "-r");
		 RATIO_STORE_ACCESS  	    = setArgs(argc, argv, "--store",  "-s");
		 ROOT_DELETE_PROBABILITY 	= setArgs(argc, argv, "--deleteaccess",  "-d");
		 STATIC_FIELD_ACCESS 	= setArgs(argc, argv, "--static",  "-sf");
		 PRIMITIVE_FIELD_ACCESS 	= setArgs(argc, argv, "--primaccess",  "-pfa");
		 //MAXCACCESS 	= setArgs(argc, argv, "--classaccess",  "-ca");
		 ESCAPE_PROBABILITY      = setArgs(argc, argv, "--escape",  "-e");
		 ESPACE_TO_PARTNER      = setArgs(argc, argv, "--esctopartner",  "-etp");



		 maxWeight = setArgs(argc, argv, "--maxWeight",  "-maxW");
		 maxFrequency = setArgs(argc, argv, "--maxFrequency",  "-maxF");
		 constantValue = setArgs(argc, argv, "--constantValue",  "-cvalue");
		 slop = setArgs(argc, argv, "--slop",  "-slp");



		// set default values
		OPERATIONS = OPERATIONS <0 ? 100 : OPERATIONS;
		NUM_THREADS   = NUM_THREADS <0 ? 10 : NUM_THREADS;
		NUM_CLAZZ 	 = NUM_CLAZZ <0 ?  300 : NUM_CLAZZ;
		MAX_POINTERS = MAX_POINTERS<0? 10 : MAX_POINTERS;
		MAX_PRIMITIVES = MAX_PRIMITIVES<0? 6 : MAX_PRIMITIVES;
		RATIO_ALLOC_SET = RATIO_ALLOC_SET <0 ? 1 : RATIO_ALLOC_SET;
		RATIO_READ_ACCESS = RATIO_READ_ACCESS < 0 ? 80-RATIO_ALLOC_SET : RATIO_READ_ACCESS;
		RATIO_STORE_ACCESS = RATIO_STORE_ACCESS <0 ? 8 : RATIO_STORE_ACCESS;
		ROOT_DELETE_PROBABILITY = ROOT_DELETE_PROBABILITY <0 ? 11 : ROOT_DELETE_PROBABILITY;
		STATIC_FIELD_ACCESS = STATIC_FIELD_ACCESS <0 ? 30 : STATIC_FIELD_ACCESS;
		PRIMITIVE_FIELD_ACCESS = PRIMITIVE_FIELD_ACCESS <0 ? 70 : PRIMITIVE_FIELD_ACCESS;
		//MAXCACCESS = MAXCACCESS <0 ? 300 : MAXCACCESS;
		ESCAPE_PROBABILITY   = ESCAPE_PROBABILITY <0 ? 12 : ESCAPE_PROBABILITY;
		ESPACE_TO_PARTNER   = ESPACE_TO_PARTNER <0 ? 90 : ESPACE_TO_PARTNER;


		maxWeight = maxWeight <0.0 ? 25.0 : maxWeight;
		maxFrequency = maxFrequency <0 ? 25.0 : maxFrequency;
		constantValue = constantValue <0 ? 10.0 : constantValue;
		slop = slop <0? 0.5 : slop;

		
		fprintf(gDetLog, "TraceFileGenerator v-%d.0\n", VERSION);
		fprintf(gDetLog, "Number of operations: %d\n", OPERATIONS);
		fprintf(gDetLog, "Number of threads: %d\n", NUM_THREADS);
		fprintf(gDetLog, "Number of classes: %d\n", NUM_CLAZZ);
		fprintf(gDetLog, "Maximum reference fields in an object/static reference fileds in a class: %d\n",  MAX_POINTERS);
		fprintf(gDetLog, "Maximum primitive fields in an object/static primitive fileds in a class:: %d\n",  MAX_PRIMITIVES);
		fprintf(gDetLog, "Maximum payload size: %d\n", MAX_POINTERS*8 + MAX_PRIMITIVES*8);
		fprintf(gDetLog, "Ratio of allocation: %d\n", RATIO_ALLOC_SET);
		fprintf(gDetLog, "Ration of read access: %d\n", RATIO_READ_ACCESS);
		fprintf(gDetLog, "Ratio of store access: %d\n", RATIO_STORE_ACCESS);
		fprintf(gDetLog, "Ratio of rootsetdeletion: %d\n", ROOT_DELETE_PROBABILITY);
		fprintf(gDetLog, "Ratio of static field access: %d\n", STATIC_FIELD_ACCESS);
		fprintf(gDetLog, "Ratio of primitive field access: %d\n", PRIMITIVE_FIELD_ACCESS);
		//fprintf(gDetLog, "Maximum used a class to create objects: %d\n", MAXCACCESS);
		fprintf(gDetLog, "Probability of escaped object: %d\n", ESCAPE_PROBABILITY);
		fprintf(gDetLog, "Probability of escaped to partner thread: %d\n", ESPACE_TO_PARTNER);

		
		
	
		Simulator* sim = new Simulator(filename);
		if(VERSION == 0){
			//printf(" Version 0 \n");
			sim->run(OPERATIONS);
		}else{
			//printf("Version 3 \n");
			sim->initializeClassTable(clsfilename);
			sim->runTraceFileGenerator(OPERATIONS);
		}
		fprintf(gDetLog, "#allocation: %d\n", na);
		fprintf(gDetLog, "#addRoot: %d\n", ar);
		fprintf(gDetLog, "#deleteRoot: %d\n", dr);
		fprintf(gDetLog, "#storeRefinObject: %d\n", sRo);
		fprintf(gDetLog, "#storeRefinClass: %d\n", sRc);
		fprintf(gDetLog, "#storePriminObject: %d\n", sPo);
		fprintf(gDetLog, "#storePriminClass: %d\n", sPc);
		fprintf(gDetLog, "#readReffromObject: %d\n", rRo);
		fprintf(gDetLog, "#readReffromClass: %d\n", rRc);
		fprintf(gDetLog, "#readPrimfromObject: %d\n", rPo);
		fprintf(gDetLog, "#readPrimfromClass: %d\n", rPc);
		fprintf(gDetLog, "Total Number of Operations: %d\n", na+ar+dr+sRo+sRc+sPo+sPc+rRo+rRc+rPo+rPc);
		


		gettimeofday(&tv, NULL);
		time = ((1000000*tv.tv_sec+tv.tv_usec) - time_start)/1.0e6;
  		printf("Wallclock time: %f\n", time);
		fprintf(gDetLog, "Total Generation Time: %.2f\n", time);
		fclose(gDetLog);
		
	return 0;
}
