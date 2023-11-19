#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

int readNumOfCoords(char *fileName);
double **readCoords(char *filename, int numOfCoords);
void *writeTourToFile(int *tour, int tourLength, char *filename);
int *findShortestTour(double **distanceMatrix, int numOfCoords);
double **createDistanceMatrix(double **coords, int numOfCoords);
void freeMemory(double **coords, double **distanceMatrix, int numOfCoords);

int main(int argc, char *argv[])
{
    printf("Starting\n");

    // Checking command line arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <coordinate file name> <output file name>\n", argv[0]);
        return 1; // Exit if not enough arguments
    }

    int numOfCoords = readNumOfCoords(argv[1]);
    double **coords = readCoords(argv[1], numOfCoords);

    // Start timer after reading coordinates
    clock_t start = clock();

    // Creating the distance matrix
    double **distanceMatrix = createDistanceMatrix(coords, numOfCoords);

    // Finding the shortest tour
    int *tour = findShortestTour(distanceMatrix, numOfCoords);

    // Stop timer before writing to file
    clock_t end = clock();

    // Calculate and print the elapsed time
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent: %.10f seconds\n", time_spent);

    // Writing the tour to a file
    writeTourToFile(tour, numOfCoords + 1, argv[2]);

    // print tool length
    printf("Tour length: %d\n", numOfCoords + 1);

    // Printing the tour
    for (int i = 0; i < numOfCoords + 1; i++)
    {
        printf("%d ", tour[i]);
    }
    printf("\n");

    // Freeing memory
    freeMemory(coords, distanceMatrix, numOfCoords);
    free(tour);
    return 0;
}

int *findShortestTour(double **distanceMatrix, int numOfCoords)
{
    bool *visited = (bool *)calloc(numOfCoords, sizeof(bool));
    int *tour = (int *)malloc((numOfCoords + 1) * sizeof(int)); // +1 for the starting coordinate at the end
    int tourSize = 1;                                           // Start with the first coordinate
    visited[0] = true;                                          // Start coordinate is visited
    tour[0] = 0;                                                // Start at the first coordinate

    while (tourSize < numOfCoords)
    {
        double globalMinCost = INFINITY;
        int globalMinIndex = -1;
        int globalInsertPosition = -1;

#pragma omp parallel
        {
            double localMinCost = INFINITY;
            int localMinIndex = -1;
            int localInsertPosition = -1;

// Parallel loop, each thread calculates its own minimum cost and related index
#pragma omp for nowait
            for (int i = 0; i < numOfCoords; i++)
            {
                if (!visited[i])
                {
                    for (int j = 0; j < tourSize; j++)
                    {
                        int k = (j + 1) % tourSize;
                        double cost = distanceMatrix[tour[j]][i] + distanceMatrix[i][tour[k]] - distanceMatrix[tour[j]][tour[k]];
                        if (cost < localMinCost)
                        {
                            localMinCost = cost;
                            localMinIndex = i;
                            localInsertPosition = j + 1;
                        }
                    }
                }
            }

// Single thread to update minimum values
#pragma omp critical
            {
                if (localMinCost < globalMinCost)
                {
                    globalMinCost = localMinCost;
                    globalMinIndex = localMinIndex;
                    globalInsertPosition = localInsertPosition;
                }
            }
        }

        // Use global minimum value to update tour
        for (int i = tourSize; i >= globalInsertPosition; i--)
        {
            tour[i] = tour[i - 1];
        }
        tour[globalInsertPosition] = globalMinIndex;
        visited[globalMinIndex] = true;
        tourSize++;
    }

    tour[numOfCoords] = tour[0];

    free(visited);
    return tour;
}

// Function to create a distance matrix
double **createDistanceMatrix(double **coords, int numOfCoords)
{
    double **distanceMatrix = (double **)malloc(numOfCoords * sizeof(double *));
    for (int i = 0; i < numOfCoords; i++)
    {
        distanceMatrix[i] = (double *)malloc(numOfCoords * sizeof(double));
        for (int j = 0; j <= i; j++)
        {
            if (i == j)
            {
                distanceMatrix[i][j] = 0;
            }
            else
            {
                double distance = sqrt(pow(coords[i][0] - coords[j][0], 2) + pow(coords[i][1] - coords[j][1], 2));
                distanceMatrix[i][j] = distanceMatrix[j][i] = distance;
            }
        }
    }
    return distanceMatrix;
}

// Function to free allocated memory
void freeMemory(double **coords, double **distanceMatrix, int numOfCoords)
{
    for (int i = 0; i < numOfCoords; i++)
    {
        free(coords[i]);
        free(distanceMatrix[i]);
    }
    free(coords);
    free(distanceMatrix);
}