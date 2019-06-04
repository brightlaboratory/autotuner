from ctypes import *

PRO = CDLL("../PRO/PRO.so")

N = 1000
T = 32
PRO.InitAlgorithmUtil(3, 2, N, T, 2, N, T, 2, N, T)
PRO.GetNewParameterValuesUtil()
T1 = PRO.GetReportingPointUtil(0)
T2 = PRO.GetReportingPointUtil(1)
T3 = PRO.GetReportingPointUtil(2)

print "T1: ", T1
print "T2: ", T2
print "T3: ", T3


