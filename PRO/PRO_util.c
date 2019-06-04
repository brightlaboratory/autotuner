
#include <PRO_util.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG 0
#define debug_print(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

void InitAlgorithmUtil(int numParams, ...) {
	va_list valist;
	va_start(valist, numParams * 3);
	proData = (struct PROData*) malloc(sizeof(struct PROData));

	int *param_min_values = (int*)malloc(sizeof(int) * numParams);
	int *param_max_values = (int*)malloc(sizeof(int) * numParams);
	int *param_initial_values = (int*)malloc(sizeof(int) * numParams);

	int i;
	for (i = 0; i < numParams; i++) {
		param_min_values[i] = va_arg(valist, int);
		param_max_values[i] = va_arg(valist, int);
		param_initial_values[i] = va_arg(valist, int);
	}

	InitializeAlgorithm(proData, numParams, param_min_values, param_max_values, param_initial_values);
	va_end(valist);
}

void GetNewParameterValuesUtil() {
	GetNewParameterValues(proData);
}

int ShouldPROStopUtil() {
	return proData->stop;
}

int GetReportingPointUtil(int index) {
	int val = proData->reportingPoint.params[index];
	debug_print("GetReportingPointUtil: val = %d\n", val);
	return val;
}

void ReportTimeUtil(double t) {
	debug_print("ReportTimeUtil: t = %f\n", t);
	proData->reportingPoint.time = t;
}

void IncrementIterationsUtil() {
	(proData->iterations)++;
}

void PrintSummaryUtil() {
	PrintSummary(proData);
}

void TerminateAlgorithmUtil() {
	TerminateAlgorithm(proData);
}

