// Declarations for varying tile sizes
#include <time.h>
#include <sys/time.h>
#include <PRO.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))


/* Internal function declarations begins*/
void FindBestPointInSimplex(struct PROData *proData);
void ProjectToValidPoints(struct PROData *proData, struct PerformanceData *simplex);
int CheckPointsOfSimplexForConvergence(struct PROData *proData, struct PerformanceData *simplex);
void CheckReflectionSimplexForConvergence(struct PROData *proData);
void FindBestPointInReflectionSimplex(struct PROData *proData);
void CheckSimplexForConvergence(struct PROData *proData);
void FillExpansionPointDataInSimplex(struct PROData *proData);
void CopyReflectionSimplexToSimplex(struct PROData *proData);
void PrintPerformanceData(struct PerformanceData data, int num_params);
void PrintSimplexPoints(struct PROData *proData);
void FormReflectionSimplex(struct PROData *proData);
void FindExpansionPoint(struct PROData *proData);
void ExpandSimplex(struct PROData *proData);
void ShrinkSimplex(struct PROData *proData);
/* Internal function declarations ends*/


inline double rtclock()
{
	struct timezone Tzp;
	struct timeval Tp;
	int stat;
	stat = gettimeofday(&Tp, &Tzp);
	if (stat != 0)
		printf("Error return from gettimeofday: %d", stat);
	return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}// inline double rtclock()


void InitializeSimplex(struct PROData *proData, int *param_initial_values)
{
	int i, j;
	// Allocate memory
	proData->simplex = (struct PerformanceData *) malloc(sizeof(struct PerformanceData) * proData->num_simplex_points);

	for (i = 0; i < proData->num_simplex_points; i++)
	{
		proData->simplex[i].time = -1;
		proData->simplex[i].params = (double*)malloc(sizeof(double) * proData->num_params);
	}// for (i = 0; i < proData->num_simplex_points; i++)


 // Initialize all simplex points to param_initial_values
	for (i = 0; i < proData->num_simplex_points; i++)
	{
		for (j = 0; j < proData->num_params; j++)
		{
			proData->simplex[i].params[j] = param_initial_values[j];
		}// for (j = 0; j < proData->num_params; j++)
	}// for (i = 0; i < proData->num_simplex_points; i++)

 // First half of the simplex points
	for (i = 0; i < proData->num_params; i++)
	{
		proData->simplex[i].params[i] = 2 * proData->simplex[i].params[i];
	}// for (i = 0; i < proData->num_params; i++)


 // Second half of the simplex points
	for (i = proData->num_params; i < proData->num_simplex_points; i++)
	{
		proData->simplex[i].params[i - proData->num_params] = floor(0.5 * proData->simplex[i].params[i - proData->num_params]);
	}// for (i = proData->num_params; i < proData->num_simplex_points; i++)


 // Project to valid points
	ProjectToValidPoints(proData, proData->simplex);

}// void InitializeSimplex (struct PROData *proData, int *param_initial_values)


void InitializeOtherPointsUsed(struct PROData *proData)
{
	proData->expansionPoint.params = (double *)malloc(sizeof(double) * proData->num_params);
	proData->expansionPoint.time = -1;

	proData->reportingPoint.params = (double *)malloc(sizeof(double) * proData->num_params);
	proData->reportingPoint.time = -1;

	// Initialize reflectionSimplex

	int i, j;
	// Allocate memory
	proData->reflectionSimplex = (struct PerformanceData *) malloc(sizeof(struct PerformanceData) * proData->num_simplex_points);

	for (i = 0; i < proData->num_simplex_points; i++)
	{
		proData->reflectionSimplex[i].time = -1;
		proData->reflectionSimplex[i].params = (double*)malloc(sizeof(double) * proData->num_params);
	}// for (i = 0; i < proData->num_simplex_points; i++)


}// void InitializeOtherPointsUsed (struct PROData *proData)

void InitializeAlgorithm(struct PROData *proData, int num_params, int *param_min_values, int *param_max_values,
	int *param_initial_values)
{
	int i;

	proData->num_params = num_params;
	proData->num_simplex_points = 2 * num_params; // A simplex with 2N vertices, where N is the number of tunable parameters

	// Initialize param_min
	proData->param_min_values = (double*)malloc(sizeof(double) * num_params);
	for (i = 0; i < num_params; i++)
	{
		proData->param_min_values[i] = param_min_values[i];
	}// for (i = 0; i < num_params; i++)


	// Initialize param_max
	proData->param_max_values = (double*)malloc(sizeof(double) * num_params);
	for (i = 0; i < num_params; i++)
	{
		proData->param_max_values[i] = param_max_values[i];
	}// for (i = 0; i < num_params; i++)


	InitializeSimplex(proData, param_initial_values);

	proData->state = 1; // Initialization phase
	proData->iterations = 0;
	proData->pointer = 0;
	proData->stop = 0;
	proData->AlgorithmStarted = 0;

	InitializeOtherPointsUsed(proData);
}// void InitializeAlgorithm ()


void PrintSummary(struct PROData *proData)
{
	printf("Number of iterations: %d\n", proData->iterations);

	printf("Best point: ");
	PrintPerformanceData(proData->simplex[proData->bestPoint], proData->num_params);

}// void PrintSummary(struct Queue *queue)

void TerminateAlgorithm(struct PROData *proData)
{
	free(proData->param_min_values);
	free(proData->param_max_values);

	int i;
	// De-allocate memory
	for (i = 0; i < proData->num_simplex_points; i++)
	{
		free(proData->simplex[i].params);
		free(proData->reflectionSimplex[i].params);
	}// for (i = 0; i < proData->num_simplex_points; i++)


	free(proData->simplex);
	free(proData->reflectionSimplex);
	free(proData->expansionPoint.params);
	free(proData->reportingPoint.params);

	free(proData);
}// void TerminateAlgorithm(struct PROData *proData)


void PrintPerformanceData(struct PerformanceData data, int num_params)
{
	int i;
	printf("Parameter values: ");

	for (i = 0; i < num_params; i++)
	{
		printf("%f ", data.params[i]);
	}// for (i = 0; i < num_params; i++)

	printf("Function value: %f\n", data.time);
}// void PrintPerformanceData(struct PerformanceData data, int num_params)



void PrintPointsOfSimplex(struct PROData *proData, struct PerformanceData *simplex)
{
	int i;
	for (i = 0; i < proData->num_simplex_points; i++)
	{
		PrintPerformanceData(simplex[i], proData->num_params);
	}// for (i = 0; i < proData->num_simplex_points; i++)

}// void PrintPointsOfSimplex (struct PROData *proData, struct PerformanceData *simplex)


void PrintSimplexPoints(struct PROData *proData)
{
	int i;
	printf("Simplex points:\n");
	PrintPointsOfSimplex(proData, proData->simplex);

}// void PrintSimplexPoints (struct PROData *proData)



void CopyParameterValues(struct PerformanceData dst, struct PerformanceData src, int num_params)
{
	int i;
	for (i = 0; i < num_params; i++)
	{
		dst.params[i] = src.params[i];
	}// for (i = 0; i < num_params; i++)
}// void CopyParameterValues (struct PerformanceData dst, struct PerformanceData src, int num_params)

void GetNewParameterValues(struct PROData *proData)
{

	if (proData->AlgorithmStarted == 1)
	{
		// Initialization
		if (proData->state == 1)
		{
			proData->simplex[proData->pointer].time = proData->reportingPoint.time;
			(proData->pointer)++;

			while (proData->pointer < proData->num_simplex_points && proData->simplex[proData->pointer].time != -1)
			{
				(proData->pointer)++;
			}// while (proData->pointer < proData->num_simplex_points && proData->simplex[proData->pointer].time != -1)

			if (proData->pointer <= (proData->num_simplex_points - 1))
			{
				// There are still points to be evaluated in the simplex
				CopyParameterValues(proData->reportingPoint, proData->simplex[proData->pointer], proData->num_params);
			}//if if (proData->pointer <= (proData->num_simplex_points - 1))
			else
			{
				FindBestPointInSimplex(proData);
				// printf ("Best point: ");
				// PrintPerformanceData(proData->simplex[proData->bestPoint], proData->num_params);

				FormReflectionSimplex(proData);

				// printf ("Reflection Simplex: \n");
				// PrintPointsOfSimplex (proData, proData->reflectionSimplex);

				proData->pointer = 0;

				CheckReflectionSimplexForConvergence(proData);

				if (proData->stop == 1)
				{
					CopyParameterValues(proData->reportingPoint, proData->simplex[proData->bestPoint], proData->num_params);

				}// if (proData->stop == 1)
				else
				{
					while (proData->pointer < proData->num_simplex_points && proData->reflectionSimplex[proData->pointer].time != -1)
					{
						(proData->pointer)++;
					}// while (proData->pointer < proData->num_simplex_points && proData->reflectionSimplex[proData->pointer].time != -1)


					CopyParameterValues(proData->reportingPoint, proData->reflectionSimplex[proData->pointer], proData->num_params);
					proData->state = 2; // Reflection step has started
				}// else :  if (proData->stop == 1)
			}// else : if (proData->pointer <= (proData->num_simplex_points - 1))
		}// if (queue->state == 1)	
		else if (proData->state == 2) // Reflection step
		{
			proData->reflectionSimplex[proData->pointer].time = proData->reportingPoint.time;
			(proData->pointer)++;

			while (proData->pointer < proData->num_simplex_points && proData->reflectionSimplex[proData->pointer].time != -1)
			{
				(proData->pointer)++;
			}// while (proData->pointer < proData->num_simplex_points && proData->reflectionSimplex[proData->pointer].time != -1)

			if (proData->pointer <= (proData->num_simplex_points - 1))
			{
				// There are still points to be evaluated in the simplex
				CopyParameterValues(proData->reportingPoint, proData->reflectionSimplex[proData->pointer], proData->num_params);
			}//if (proData->pointer <= (proData->num_simplex_points - 1))
			else
			{
				FindBestPointInReflectionSimplex(proData);

				// printf ("Best point in the reflection simplex\n");
				// PrintPerformanceData(proData->reflectionSimplex[proData->reflectionBestPoint], proData->num_params);


				if (proData->reflectionBestPoint != proData->bestPoint)
				{
					// printf ("Reflection Successful. Checking for expansion\n");  
					FindExpansionPoint(proData);
					CopyParameterValues(proData->reportingPoint, proData->expansionPoint, proData->num_params);
					proData->state = 3;
				}// if (proData->reflectionBestPoint != proData->bestPoint)
				else
				{
					// Reflection unsuccessful. Shrink the simplex
			// printf ("Reflection unsuccessful. Shrinking the simplex\n");

					ShrinkSimplex(proData);

					// PrintSimplexPoints (proData);

					proData->state = 1;
					CheckSimplexForConvergence(proData);

					if (proData->stop == 1)
					{
						CopyParameterValues(proData->reportingPoint, proData->simplex[proData->bestPoint], proData->num_params);
					}// if (proData->stop == 1)
					else
					{
						proData->pointer = 0;
						while (proData->pointer < proData->num_simplex_points && proData->simplex[proData->pointer].time != -1)
						{
							(proData->pointer)++;
						}// while (proData->pointer < proData->num_simplex_points && proData->simplex[proData->pointer].time != -1)

						CopyParameterValues(proData->reportingPoint, proData->simplex[proData->pointer], proData->num_params);
					}// else ! if (proData->stop == 1)
				}// else : if (proData->reflectionBestPoint != proData->bestPoint)           
			}// else : if (proData->pointer <= (proData->num_simplex_points - 1))
		}// else if (proData->state == 2) // Reflection step
		else if (proData->state == 3) // Expansion check step
		{
			proData->expansionPoint.time = proData->reportingPoint.time;

			if (proData->expansionPoint.time < proData->reflectionSimplex[proData->reflectionBestPoint].time) // Expansion successful
			{
				// printf ("Expansion successful. Expanding the simplex\n");
				ExpandSimplex(proData);
				proData->state = 1;

				CheckSimplexForConvergence(proData);

				if (proData->stop == 1)
				{
					CopyParameterValues(proData->reportingPoint, proData->simplex[proData->bestPoint], proData->num_params);
				}// if (proData->stop == 1)
				else
				{
					proData->pointer = 0;
					FillExpansionPointDataInSimplex(proData);

					while (proData->pointer < proData->num_simplex_points && proData->simplex[proData->pointer].time != -1)
					{
						(proData->pointer)++;
					}// while (proData->pointer < proData->num_simplex_points && proData->simplex[proData->pointer].time != -1)

					CopyParameterValues(proData->reportingPoint, proData->simplex[proData->pointer], proData->num_params);
				}// else ! if (proData->stop == 1)


	// PrintSimplexPoints (proData);
			}// if (proData->expansionPoint.time < proData->reflectionSimplex[proData->reflectionBestPoint].time) // Expansion successful
			else
			{
				// printf ("Expansion Unsuccessful, Accepting Reflection\n");			

				CopyReflectionSimplexToSimplex(proData);

				FindBestPointInSimplex(proData);
				// printf ("Best point: ");
				// PrintPerformanceData(proData->simplex[proData->bestPoint], proData->num_params);

				FormReflectionSimplex(proData);

				// printf ("Reflection Simplex: \n");
				// PrintPointsOfSimplex (proData, proData->reflectionSimplex);

				proData->pointer = 0;
				CheckReflectionSimplexForConvergence(proData);

				if (proData->stop == 1)
				{
					CopyParameterValues(proData->reportingPoint, proData->simplex[proData->bestPoint], proData->num_params);
				}// if (proData->stop == 1)
				else
				{
					while (proData->pointer < proData->num_simplex_points && proData->reflectionSimplex[proData->pointer].time != -1)
					{
						(proData->pointer)++;
					}// while (proData->pointer < proData->num_simplex_points && proData->reflectionSimplex[proData->pointer].time != -1)

					CopyParameterValues(proData->reportingPoint, proData->reflectionSimplex[proData->pointer], proData->num_params);
					proData->state = 2; // Reflection step has started
				}// else :  if (proData->stop == 1)			
			}// else : ! if (proData->expansionPoint.time < proData->reflectionSimplex[proData->reflectionBestPoint].time) // Expansion unsuccessful						
		}// else if (proData->state == 3)
	} // if (proData->AlgorithmStarted == 1)
	else
	{
		proData->AlgorithmStarted = 1;
		CopyParameterValues(proData->reportingPoint, proData->simplex[proData->pointer], proData->num_params);
	}// else ! if (queue->AlgorithmStarted == 1)

}// void GetNewParameterValues(struct PROData *proData, struct PerformanceData data)


int FindBestPointAmongSimplexPoints(struct PerformanceData *simplex, int num_simplex_points)
{
	int index = 0, i;
	for (i = 1; i < num_simplex_points; i++)
	{
		if (simplex[i].time < simplex[index].time)
		{
			index = i;
		}// if (simplex[i].time < simplex[index].time)
	}// for (i = 1; i < num_simplex_points; i++)

	return index;
}// int FindBestPointAmongSimplexPoints (struct PerformanceData *simplex)

void FindBestPointInSimplex(struct PROData *proData)
{
	proData->bestPoint = FindBestPointAmongSimplexPoints(proData->simplex, proData->num_simplex_points);
}// void FindBestPointInSimplex (struct PROData *proData)



void FindBestPointInReflectionSimplex(struct PROData *proData)
{
	proData->reflectionBestPoint = FindBestPointAmongSimplexPoints(proData->reflectionSimplex, proData->num_simplex_points);
}// void FindBestPointInReflectionSimplex (struct PROData *proData)

void FormReflectionSimplex(struct PROData *proData)
{
	int i, j;

	for (i = 0; i < proData->num_simplex_points; i++)
	{
		for (j = 0; j < proData->num_params; j++)
		{
			proData->reflectionSimplex[i].params[j] = 2 * proData->simplex[proData->bestPoint].params[j] - proData->simplex[i].params[j];
		}// for (j = 0; j < proData->num_params; j++)

		proData->reflectionSimplex[i].time = -1;

		if (i == proData->bestPoint)
		{
			proData->reflectionSimplex[i].time = proData->simplex[i].time;
		}// if (i == proData->bestPoint)
	}// for (i = 1; i < num_simplex_points; i++)


	ProjectToValidPoints(proData, proData->reflectionSimplex);

}// void FormReflectionSimplex (struct Queue *queue)

void ProjectParametersToValidPoints(struct PROData *proData, struct PerformanceData point)
{
	int j;
	for (j = 0; j < proData->num_params; j++)
	{
		point.params[j] = max(point.params[j], proData->param_min_values[j]);
		point.params[j] = min(point.params[j], proData->param_max_values[j]);
	}// for (j = 0; j < proData->num_params; j++)
}// void ProjectParametersToValidPoints (struct PROData *proData, struct PerformanceData point)

void ProjectToValidPoints(struct PROData *proData, struct PerformanceData *simplex)
{

	int i, j;
	// Project all simplex points to valid points
	for (i = 0; i < proData->num_simplex_points; i++)
	{
		ProjectParametersToValidPoints(proData, simplex[i]);
	}// for (i = 0; i < proData->num_simplex_points; i++)
}// void ProjectToValidPoints (struct PROData *proData, struct PerformanceData *simplex)



void CheckReflectionSimplexForConvergence(struct PROData *proData)
{
	proData->stop = CheckPointsOfSimplexForConvergence(proData, proData->reflectionSimplex);
}// void CheckReflectionSimplexForConvergence (struct PROData *proData)

void CheckSimplexForConvergence(struct PROData *proData)
{
	proData->stop = CheckPointsOfSimplexForConvergence(proData, proData->simplex);
}// void CheckSimplexForConvergence (struct PROData *proData)


int CheckPointsOfSimplexForConvergence(struct PROData *proData, struct PerformanceData *simplex)
{
	int i, j;
	int same = 1;
	int offset = 1;

	for (i = 1; i < proData->num_simplex_points; i++)
	{
		for (j = 0; j < proData->num_params; j++)
		{

			if (!(simplex[i].params[j] <= simplex[0].params[j] + offset && simplex[i].params[j] >= simplex[0].params[j] - offset))
			{
				same = 0;
			}// if (!(simplex[i].parms[j] <= simplex[0].parms[j]+offset && simplex[i].parms[j] >= simplex[0].parms[j]-offset))

		}// for (j = 0; j < proData->num_params; j++)
	}// for (i = 0; i < proData->num_simplex_points; i++)


	return same;
}// int CheckPointsOfSimplexForConvergence (struct PROData *proData, struct PerformanceData *simplex)



void FindExpansionPoint(struct PROData *proData)
{
	int j;

	// Find expansion point corresponding to the best performing point in the reflectionSimplex.
	for (j = 0; j < proData->num_params; j++)
	{
		proData->expansionPoint.params[j] = 3 * proData->simplex[proData->bestPoint].params[j] - 2 * proData->simplex[proData->reflectionBestPoint].params[j];
	}// for (j = 0; j < proData->num_params; j++)

	proData->expansionPoint.time = -1;

	// Project expansion point parameters to valid points
	ProjectParametersToValidPoints(proData, proData->expansionPoint);
}// void FindExpansionPoint (struct PROData *proData)



void ShrinkSimplex(struct PROData *proData)
{
	int i, j;

	for (i = 0; i < proData->num_simplex_points; i++)
	{
		if (i != proData->bestPoint)
		{

			for (j = 0; j < proData->num_params; j++)
			{
				proData->simplex[i].params[j] = floor((proData->simplex[proData->bestPoint].params[j] + proData->simplex[i].params[j]) * 0.5);
			}// for (j = 0; j < proData->num_params; j++)

			proData->simplex[i].time = -1;
		}// if (i != proData->bestPoint)		
	}// for (i = 1; i < num_simplex_points; i++)


	ProjectToValidPoints(proData, proData->simplex);
}// void ShrinkSimplex (struct PROData *proData)


void ExpandSimplex(struct PROData *proData)
{
	int i, j;

	for (i = 0; i < proData->num_simplex_points; i++)
	{

		if (i != proData->bestPoint)
		{
			for (j = 0; j < proData->num_params; j++)
			{
				proData->simplex[i].params[j] = 3 * proData->simplex[proData->bestPoint].params[j] - 2 * proData->simplex[i].params[j];
			}// for (j = 0; j < proData->num_params; j++)

			proData->simplex[i].time = -1;
		}// if (i != proData->bestPoint)		
	}// for (i = 1; i < num_simplex_points; i++)


	ProjectToValidPoints(proData, proData->simplex);
}// void ExpandSimplex (struct PROData *proData)


void FillExpansionPointDataInSimplex(struct PROData *proData)
{
	proData->simplex[proData->reflectionBestPoint].time = proData->expansionPoint.time;
}// void FillExpansionPointDataInSimplex (struct PROData *proData)


void CopyReflectionSimplexToSimplex(struct PROData *proData)
{
	// Swap simplex and reflectionSimplex pointers
	struct PerformanceData *temp = proData->simplex;
	proData->simplex = proData->reflectionSimplex;
	proData->reflectionSimplex = temp;
}// void CopyReflectionSimplexToSimplex (struct PROData *proData)

