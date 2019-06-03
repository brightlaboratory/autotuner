#ifndef PRO_H
# define PRO_H



struct PerformanceData
{
	double *params;
	double time;
};


struct PROData
{
	struct PerformanceData *simplex;
	int num_simplex_points;

	struct PerformanceData *reflectionSimplex;
	int pointer; // front is where you insert, rear is where you delete	
	double *param_min_values;
	double *param_max_values;
	int num_params;

	struct PerformanceData expansionPoint;
	struct PerformanceData reportingPoint;

	int state;
	int bestPoint; // pointer into simplex queue
	int reflectionBestPoint;
	int worstPoint;
	int AlgorithmStarted;

	// Algorithm termination variables
	int iterations; // The number of iterations of the algorithm
	int stop;

	/*
	state:
	1 - initialization. i.e the points of the simplex are still to be evaluated.
	2 - reflection.
	3 - expansion
	4 - shrink
	*/


};//struct PerformanceData


inline double rtclock();
void InitializeAlgorithm(struct PROData *proData, int num_params, int *param_min_values, int *param_max_values,
	int *param_initial_values);
void GetNewParameterValues(struct PROData *proData);
void TerminateAlgorithm(struct PROData *proData);
void ProjectParametersToValidPoints(struct PROData *proData, struct PerformanceData point);


#endif

