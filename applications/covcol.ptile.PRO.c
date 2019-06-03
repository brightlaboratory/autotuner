#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

/* PRO code beings */
#include <PRO.h>
/* PRO code ends */
#define my_sqrt_array(x,j) sqrt(x[j])
#define M 1000
#define N 1000


float float_n = (float)N;
float eps = 0.005;
float stddev[M + 1];
float data[M + 1][N + 1];
float mean[M + 1];
float symmat[M + 1][M + 1];

#ifdef TIME
#define IF_TIME(foo) foo;
#else
#define IF_TIME(foo)
#endif

void init_array()
{
	int i, j;
	int n = N;
	int m = M;

	for (i = 0; i <= n; i++) {
		for (j = 0; j <= m; j++) {
			data[i][j] = ((float)i*j) / N;
		}
	}
}

void print_array(int argc, char** argv)
{
	int i, j;
	int n = N;
	int m = M;
#ifndef POLYBENCH_DUMP_ARRAYS
	if (argc > 42 && !strcmp(argv[0], ""))
#endif
	{
		for (i = 1; i <= m; i++) {
			for (j = 1; j <= m; j++) {
				fprintf(stdout, "%0.2lf ", symmat[i][j]);
				if (i % 80 == 20) fprintf(stdout, "\n");
			}
		}
		fprintf(stdout, "\n");
	}
}



int main(int argc, char** argv)
{
	double t_start, t_end;
	int i, j, j1, j2;
	int n = N;
	int m = M;

	float T1c1, T1c2, T1c3;

	T1c1 = atoi(argv[1]);
	T1c2 = atoi(argv[2]);
	T1c3 = atoi(argv[3]);

	/* PRO code begins */

	double start, end, total = 0.0;
	struct PROData *proData = (struct PROData*) malloc(sizeof(struct PROData));

	int *param_min_values = (int*)malloc(sizeof(int) * 3);
	int *param_max_values = (int*)malloc(sizeof(int) * 3);
	int *param_initial_values = (int*)malloc(sizeof(int) * 3);

	for (i = 0; i < 3; i++)
	{
		param_min_values[i] = 2;
		param_max_values[i] = N;
	}

	param_initial_values[0] = T1c1;
	param_initial_values[1] = T1c2;
	param_initial_values[2] = T1c3;

	InitializeAlgorithm(proData, 3, param_min_values, param_max_values, param_initial_values);
	GetNewParameterValues(proData);

	while (proData->stop == 0)
	{
		T1c1 = proData->reportingPoint.params[0];
		T1c2 = proData->reportingPoint.params[1];
		T1c3 = proData->reportingPoint.params[2];

		/* PRO code ends */


		init_array();

		start = rtclock();

#ifdef ceild
# undef ceild
#endif
#ifdef floord
# undef floord
#endif
#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))







		register int lbv, ubv, lb, ub, lb1, ub1, lb2, ub2;
		register int c1t1, c2t1, c1, c2, c3t1, c3;

#pragma scop
		if ((m >= 1)) {
			int __loop_bound_0 = round((-1 + ((1 / T1c1) * 2)));
			int __loop_bound_1 = round((m * (1 / T1c1)));
			int __loop_bound_3 = round((m * (1 / T1c2)));
#pragma omp parallel for private(c2t1, c1, c2) firstprivate(__loop_bound_0, __loop_bound_1, __loop_bound_3)
			for (c1t1 = __loop_bound_0; c1t1 <= __loop_bound_1; ++(c1t1)) {
				int __loop_bound_2 = round((((c1t1 * (T1c1 / T1c2)) + -1) + (1 / T1c2)));
				int __loop_bound_4 = max((c1t1 * T1c1), 1);
				int __loop_bound_5 = min(((c1t1 * T1c1) + (T1c1 + -1)), m);
				for (c2t1 = __loop_bound_2; c2t1 <= __loop_bound_3; ++(c2t1)) {
					int __loop_bound_7 = min(((c2t1 * T1c2) + (T1c2 + -1)), m);
					for (c1 = __loop_bound_4; c1 <= __loop_bound_5; c1++) {
						int __loop_bound_6 = max((c2t1 * T1c2), c1);
						for (c2 = __loop_bound_6; c2 <= __loop_bound_7; c2++) {
							symmat[c1][c2] = 0.0;
						}
					}
				}
			}
			for (c1 = 1; c1 <= m; c1++) {
				mean[c1] = 0.0;
			}
			if ((n >= 1)) {
				int __loop_bound_8 = round((-1 + ((1 / T1c1) * 2)));
				int __loop_bound_9 = round((m * (1 / T1c1)));
				int __loop_bound_10 = round((-1 + ((1 / T1c2) * 2)));
				int __loop_bound_11 = round((n * (1 / T1c2)));
#pragma omp parallel for private(c2t1, c1, c2) firstprivate(__loop_bound_8, __loop_bound_9, __loop_bound_10, __loop_bound_11, __loop_bound_0, __loop_bound_1, __loop_bound_3)
				for (c1t1 = __loop_bound_8; c1t1 <= __loop_bound_9; ++(c1t1)) {
					int __loop_bound_12 = max((c1t1 * T1c1), 1);
					int __loop_bound_13 = min(((c1t1 * T1c1) + (T1c1 + -1)), m);
					for (c2t1 = __loop_bound_10; c2t1 <= __loop_bound_11; ++(c2t1)) {
						int __loop_bound_14 = max((c2t1 * T1c2), 1);
						int __loop_bound_15 = min(((c2t1 * T1c2) + (T1c2 + -1)), n);
						for (c1 = __loop_bound_12; c1 <= __loop_bound_13; c1++) {
							for (c2 = __loop_bound_14; c2 <= __loop_bound_15; c2++) {
								mean[c1] += data[c2][c1];
							}
						}
					}
				}
			}
			for (c1 = 1; c1 <= m; c1++) {
				mean[c1] /= float_n;
			}
			int __loop_bound_16 = round((-1 + ((1 / T1c1) * 2)));
			int __loop_bound_17 = round((n * (1 / T1c1)));
			int __loop_bound_18 = round((-1 + ((1 / T1c2) * 2)));
			int __loop_bound_19 = round((m * (1 / T1c2)));
#pragma omp parallel for private(c2t1, c1, c2) firstprivate(__loop_bound_0, __loop_bound_1, __loop_bound_3, __loop_bound_16, __loop_bound_17, __loop_bound_18, __loop_bound_19)
			for (c1t1 = __loop_bound_16; c1t1 <= __loop_bound_17; ++(c1t1)) {
				int __loop_bound_20 = max((c1t1 * T1c1), 1);
				int __loop_bound_21 = min(((c1t1 * T1c1) + (T1c1 + -1)), n);
				for (c2t1 = __loop_bound_18; c2t1 <= __loop_bound_19; ++(c2t1)) {
					int __loop_bound_22 = max((c2t1 * T1c2), 1);
					int __loop_bound_23 = min(((c2t1 * T1c2) + (T1c2 + -1)), m);
					for (c1 = __loop_bound_20; c1 <= __loop_bound_21; c1++) {
						for (c2 = __loop_bound_22; c2 <= __loop_bound_23; c2++) {
							data[c1][c2] -= mean[c2];
						}
					}
				}
			}
			if ((n >= 1)) {
				int __loop_bound_24 = round((-1 + ((1 / T1c1) * 2)));
				int __loop_bound_25 = round((m * (1 / T1c1)));
				int __loop_bound_27 = round((m * (1 / T1c2)));
				int __loop_bound_28 = round((-1 + ((1 / T1c3) * 2)));
				int __loop_bound_29 = round((n * (1 / T1c3)));
#pragma omp parallel for private(c2t1, c3t1, c1, c2, c3) firstprivate(__loop_bound_24, __loop_bound_25, __loop_bound_27, __loop_bound_28, __loop_bound_29, __loop_bound_0, __loop_bound_1, __loop_bound_3, __loop_bound_16, __loop_bound_17, __loop_bound_18, __loop_bound_19) schedule(static, 1)
				for (c1t1 = __loop_bound_24; c1t1 <= __loop_bound_25; ++(c1t1)) {
					int __loop_bound_26 = round((((c1t1 * (T1c1 / T1c2)) + -1) + (1 / T1c2)));
					int __loop_bound_30 = max((c1t1 * T1c1), 1);
					int __loop_bound_31 = min(((c1t1 * T1c1) + (T1c1 + -1)), m);
					for (c2t1 = __loop_bound_26; c2t1 <= __loop_bound_27; ++(c2t1)) {
						int __loop_bound_33 = min(((c2t1 * T1c2) + (T1c2 + -1)), m);
						for (c3t1 = __loop_bound_28; c3t1 <= __loop_bound_29; ++(c3t1)) {
							int __loop_bound_34 = max((c3t1 * T1c3), 1);
							int __loop_bound_35 = min(((c3t1 * T1c3) + (T1c3 + -1)), n);
							for (c1 = __loop_bound_30; c1 <= __loop_bound_31; c1++) {
								int __loop_bound_32 = max((c2t1 * T1c2), c1);
								for (c2 = __loop_bound_32; c2 <= __loop_bound_33; c2++) {
									for (c3 = __loop_bound_34; c3 <= __loop_bound_35; c3++) {
										symmat[c1][c2] += data[c3][c1] * data[c3][c2];
									}
								}
							}
						}
					}
				}
			}
			int __loop_bound_36 = round((-1 + ((1 / T1c1) * 2)));
			int __loop_bound_37 = round((m * (1 / T1c1)));
			int __loop_bound_39 = round((m * (1 / T1c2)));
#pragma omp parallel for private(c2t1, c1, c2) firstprivate(__loop_bound_0, __loop_bound_1, __loop_bound_3, __loop_bound_16, __loop_bound_17, __loop_bound_18, __loop_bound_19, __loop_bound_36, __loop_bound_37, __loop_bound_39)
			for (c1t1 = __loop_bound_36; c1t1 <= __loop_bound_37; ++(c1t1)) {
				int __loop_bound_38 = round((((c1t1 * (T1c1 / T1c2)) + -1) + (1 / T1c2)));
				int __loop_bound_40 = max((c1t1 * T1c1), 1);
				int __loop_bound_41 = min(((c1t1 * T1c1) + (T1c1 + -1)), m);
				for (c2t1 = __loop_bound_38; c2t1 <= __loop_bound_39; ++(c2t1)) {
					int __loop_bound_43 = min(((c2t1 * T1c2) + (T1c2 + -1)), m);
					for (c1 = __loop_bound_40; c1 <= __loop_bound_41; c1++) {
						int __loop_bound_42 = max((c2t1 * T1c2), c1);
						for (c2 = __loop_bound_42; c2 <= __loop_bound_43; c2++) {
							symmat[c2][c1] = symmat[c1][c2];
						}
					}
				}
			}
		}
#pragma endscop

		/* Stop and print timer. */
		end = rtclock();

		print_array(argc, argv);

		printf("%f %f %f %f\n", T1c1, T1c2, T1c3, (end - start));

		/* PRO code beings */
		proData->reportingPoint.time = (end - start);
		GetNewParameterValues(proData);
		(proData->iterations)++;
	}// while (proData->stop == 0)

	PrintSummary(proData);
	TerminateAlgorithm(proData);
	/* PRO code ends */


	return 0;
}

