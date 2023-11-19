#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

int readNumOfCoords(char *fileName);
double **readCoords(char *filename, int numOfCoords);
void *writeTourToFile(int *tour, int tourLength, char *filename);
int *findFarthestTour(double **distanceMatrix, int numOfCoords);
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
    double start_time = omp_get_wtime();

    // Creating the distance matrix
    double **distanceMatrix = createDistanceMatrix(coords, numOfCoords);

    // Finding the shortest tour
    int *tour = findFarthestTour(distanceMatrix, numOfCoords);

    // Stop timer before writing to file
    double time = omp_get_wtime() - start_time;
    printf("Time spent: %.3f seconds\n", time);

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

int *findFarthestTour(double **distanceMatrix, int numOfCoords)
{
    bool *visited = (bool *)calloc(numOfCoords, sizeof(bool));
    int *tour = (int *)malloc((numOfCoords + 1) * sizeof(int)); // +1 for the starting coordinate at the end
    int tourSize = 1;                                           // Start with the first coordinate
    visited[0] = true;                                          // Start coordinate is visited
    tour[0] = 0;                                                // Start at the first coordinate

    while (tourSize < numOfCoords)
    {
        double globalMaxDistance = -1;
        int globalMaxIndex = -1;
        int globalInsertPosition = -1;

#pragma omp parallel
        {
            double localMaxDistance = -1;
            int localMaxIndex = -1;
            int localInsertPosition = -1;
            double localMinCost = INFINITY;

// Find the farthest point to insert
#pragma omp for nowait
            for (int i = 0; i < numOfCoords; i++)
            {
                if (!visited[i])
                {
                    for (int j = 0; j < tourSize; j++)
                    {
                        int k = (j + 1) % tourSize;
                        double distance = distanceMatrix[i][tour[j]];
                        if (distance > localMaxDistance)
                        {
                            localMaxDistance = distance;
                            localMaxIndex = i;
                        }
                    }
                }
            }

#pragma omp critical
            {
                if (localMaxDistance > globalMaxDistance)
                {
                    globalMaxDistance = localMaxDistance;
                    globalMaxIndex = localMaxIndex;
                }
            }
        }

        // Insert the farthest location into the tour
        for (int i = 0; i < tourSize; i++)
        {
            int k = (i + 1) % tourSize;
            double cost = distanceMatrix[tour[i]][globalMaxIndex] + distanceMatrix[globalMaxIndex][tour[k]] - distanceMatrix[tour[i]][tour[k]];
            if (cost < globalMinCost)
            {
                globalMinCost = cost;
                globalInsertPosition = i + 1;
            }
        }

        // Insert maxIndex at insertPosition
        for (int i = tourSize; i >= globalInsertPosition; i--)
        {
            tour[i] = tour[i - 1];
        }
        tour[globalInsertPosition] = globalMaxIndex;
        visited[globalMaxIndex] = true;
        tourSize++;
    }

    // Add the starting coordinate to the end
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