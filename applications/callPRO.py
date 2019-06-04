from ctypes import *
import time

PRO = CDLL("../PRO/PRO.so")
PRO.ReportTimeUtil.argtypes = [c_double]

#Initialization
T1_min = 2
T1_max = 1000
T1_init = 32

T2_min = 2
T2_max = 1000
T2_init = 32

T3_min = 2
T3_max = 1000
T3_init = 32

numParams = 3
PRO.InitAlgorithmUtil(numParams, T1_min, T1_max, T1_init, T2_min, T2_max, T2_init, T3_min, T3_max, T3_init)
PRO.GetNewParameterValuesUtil()
T1 = PRO.GetReportingPointUtil(0)
T2 = PRO.GetReportingPointUtil(1)
T3 = PRO.GetReportingPointUtil(2)

print "T1: ", T1
print "T2: ", T2
print "T3: ", T3

#Run the algorithm
while PRO.ShouldPROStopUtil() == 0:
	T1 = PRO.GetReportingPointUtil(0)
	T2 = PRO.GetReportingPointUtil(1)
	T3 = PRO.GetReportingPointUtil(2)

	print "T1: ", T1
	print "T2: ", T2
	print "T3: ", T3

	start = time.time()
	time.sleep(0.25)
	end = time.time()
	interval = end - start
	print "Elapsed time: ", interval
	PRO.ReportTimeUtil(interval)
	PRO.GetNewParameterValuesUtil()
	PRO.IncrementIterationsUtil()

#Print summary
PRO.PrintSummaryUtil()
PRO.TerminateAlgorithmUtil()

