
#include <PRO_util.h>
#include <stdarg.h>

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

double GetReportingPointUtil(int index) {
	return proData->reportingPoint.params[index];
}

void ReportTimeUtil(double t) {
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

