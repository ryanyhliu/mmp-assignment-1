#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

int readNumOfCoords(char *fileName);
double **readCoords(char *filename, int numOfCoords);
int *findShortestTour(double **distanceMatrix, int numOfCoords);

int main(int argc, char const *argv[])
{
    // check
    printf("Starting cInsertion.c\n");
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <coordinate file name> <output file name>\n", argv[0]);
        return 1;
    }

    char coordinateFileName[500];
    char outputFileName[500];
    strcpy(coordinateFileName, argv[1]);
    strcpy(outputFileName, argv[2]);
    int numOfCoords = readNumOfCoords(coordinateFileName);
    printf("Number of coordinates: %d\n", numOfCoords);
    double **coords = readCoords(coordinateFileName, numOfCoords);
    // printf("First coordinate: %f, %f\n", coords[0][0], coords[0][1]);

    // convert this into a distance matrix with euclidean distance formula.
    // init a 2d array of doubles: n x n
    double **distanceMatrix = (double **)malloc(numOfCoords * sizeof(double *));
    for (int i = 0; i < numOfCoords; i++)
    {
        distanceMatrix[i] = (double *)malloc(numOfCoords * sizeof(double));
        if (distanceMatrix[i] == NULL)
        {
            perror("Memory Allocation Failed");
        }
    }

    int calcCount = 0;
    // fill the distance matrix with euclidean distances
    for (int i = 0; i < numOfCoords; i++)
    {
        for (int j = 0; j < numOfCoords; j++)
        {
            if (i > j)
            {
                distanceMatrix[i][j] = distanceMatrix[j][i];
                continue;
            }
            if (i == j)
            {
                distanceMatrix[i][j] = 0;
                continue;
            }
            double x1 = coords[i][0];
            double y1 = coords[i][1];
            double x2 = coords[j][0];
            double y2 = coords[j][1];
            double distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
            calcCount++;
            distanceMatrix[i][j] = distance;
        }
    }
    printf("Number of sqrt and pow calculations: %d\n", calcCount);

    // print the distance matrix
    for (int i = 0; i < numOfCoords; i++)
    {
        for (int j = 0; j < numOfCoords; j++)
        {
            printf("%f ", distanceMatrix[i][j]);
        }
        printf("\n");
    }

    // find the shortest tour
    int *tour = findShortestTour(distanceMatrix, numOfCoords);

    // print the tour
    for (int i = 0; i < numOfCoords + 1; i++)
    {
        printf("%d ", tour[i]);
    }
    printf("\n");
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
        double minCost = INFINITY;
        int minIndex = -1;
        int insertPosition = -1;

        // Find the cheapest insertion
        for (int i = 0; i < numOfCoords; i++)
        {
            if (!visited[i])
            {
                for (int j = 0; j < tourSize; j++)
                {
                    int k = (j + 1) % tourSize;
                    double cost = distanceMatrix[tour[j]][i] + distanceMatrix[i][tour[k]] - distanceMatrix[tour[j]][tour[k]];
                    if (cost < minCost)
                    {
                        minCost = cost;
                        minIndex = i;
                        insertPosition = j + 1;
                    }
                }
            }
        }

        // Insert minIndex at insertPosition
        for (int i = tourSize; i >= insertPosition; i--)
        {
            tour[i] = tour[i - 1];
        }
        tour[insertPosition] = minIndex;
        visited[minIndex] = true;
        tourSize++;
    }

    // Add the starting coordinate to the end
    tour[numOfCoords] = tour[0];

    free(visited);
    return tour;
}