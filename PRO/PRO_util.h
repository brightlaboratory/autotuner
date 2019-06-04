#ifndef PRO_UTIL_H
# define PRO_UTIL_H

#include <PRO.h>

struct PROData *proData;

void InitAlgorithmUtil(int numParams, ...);
void GetNewParameterValuesUtil();
int ShouldPROStopUtil();
double GetReportingPointUtil(int index);
void ReportTimeUtil(double t);
void IncrementIterationsUtil();
void PrintSummaryUtil();
void TerminateAlgorithmUtil();

#endif
