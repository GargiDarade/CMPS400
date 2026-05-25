// MatrixWorkbench.cpp : Defines the entry point for the console application.
//g++ MatrixWorkbench.cpp -o MatrixWorkbench.exe

//#include "stdafx.h"
//#include <string>  
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h> // For getch()

using namespace std;

const int MAX = 10;

// ===================== STRUCT DEFINITIONS =====================

struct Matrix
{
    int id;                     // auto-generated Unique matrix ID
    int values[MAX][MAX];       // Current working matrix
    int original[MAX][MAX];     // Original backup matrix
    int rows;					// number of rows
    int cols;					// number of columns
    bool exists;				// true if the matrix has been created
};

struct MultiplyResult
{
    bool compatible;			// true if multiplication is valid
    int result[MAX][MAX];		// resulting matrix
    int resultRows;				// rows of result
    int resultCols;				// cols of result
    int scalarMultiplications;	// number of multiplications a[i][k] * b[k][j]
    int additions;				// number of additions used to combine products
};

struct ScalarResult
{
    int scalar;				// scalar value used
    int multiplications;	// number of scalar multiplications performed
    int writes;				// number of matrix updates performed
};

// ===================== GLOBAL VARIABLES =====================

int usedIDs[100];
int usedCount = 0;

// ===================== FUNCTION PROTOTYPES =====================

int generateUniqueID();
void copyMatrix(int dest[][MAX], const int src[][MAX], int rows, int cols);
void displayMatrix(const int mat[][MAX], int rows, int cols);

void createMatrix(Matrix &m, char name);
bool canMultiply(const Matrix &A, const Matrix &B);

MultiplyResult multiplyMatrices(const Matrix &A, const Matrix &B);

ScalarResult scalarMultiply(Matrix &m, int k);

void restoreMatrix(Matrix &m);

void displayCurrentMatrices(const Matrix &A, const Matrix &B);

void checkCompatibility(const Matrix &A, const Matrix &B);

void multiplicationReport(const Matrix &A, const Matrix &B, string label);

void pauseScreen();

// ===================== MAIN FUNCTION =====================

int main()
{
    srand(time(0));

    Matrix A = {0, {}, {}, 0, 0, false};
    Matrix B = {0, {}, {}, 0, 0, false};

    int choice;

    do
    {
        //system("cls");

        cout << "*************** Matrix Workbench ***************\n";
        cout << "1. Create Matrix A\n";
        cout << "2. Create Matrix B\n";
        cout << "3. Display the current matrices\n";
        cout << "4. Check multiplication compatibility\n";
        cout << "5. Perform A x B\n";
        cout << "6. Perform B x A\n";
        cout << "7. Multiply a matrix by a scalar\n";
        cout << "8. Restore the original matrices\n";
        cout << "0. Exit\n";

        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            createMatrix(A, 'A');
            break;

        case 2:
            createMatrix(B, 'B');
            break;

        case 3:
            displayCurrentMatrices(A, B);
            break;

        case 4:
            checkCompatibility(A, B);
            break;

        case 5:
            multiplicationReport(A, B, "A x B");
            break;

        case 6:
            multiplicationReport(B, A, "B x A");
            break;

        case 7:
        {
            if (!A.exists && !B.exists)
            {
                cout << "No matrices exist yet.\n";
                pauseScreen();
                break;
            }

            int option;
            int scalar;

            cout << "Select matrix:\n";
            cout << "1- Matrix A\n";
            cout << "2- Matrix B\n";
            cout << "Choice: ";
            cin >> option;

            cout << "Enter scalar value: ";
            cin >> scalar;

            if (option == 1 && A.exists)
            {
                cout << "\nBefore Scalar Multiplication:\n";
                displayMatrix(A.values, A.rows, A.cols);

                ScalarResult result = scalarMultiply(A, scalar);

                cout << "\nAfter Scalar Multiplication:\n";
                displayMatrix(A.values, A.rows, A.cols);

                cout << "\nScalar Multiplications: "
                     << result.multiplications << endl;

                cout << "Writes: "
                     << result.writes << endl;
            }
            else if (option == 2 && B.exists)
            {
                cout << "\nBefore Scalar Multiplication:\n";
                displayMatrix(B.values, B.rows, B.cols);

                ScalarResult result = scalarMultiply(B, scalar);

                cout << "\nAfter Scalar Multiplication:\n";
                displayMatrix(B.values, B.rows, B.cols);

                cout << "\nScalar Multiplications: "
                     << result.multiplications << endl;

                cout << "Writes: "
                     << result.writes << endl;
            }
            else
            {
                cout << "Selected matrix does not exist.\n";
            }

            cout << "\nTheoretical Complexity:\n";
            cout << "Time Complexity: O(r x c)\n";
            cout << "Auxiliary Space: O(1)\n";

            pauseScreen();
            break;
        }

        case 8:
        {
            if (A.exists)
            {
                restoreMatrix(A);

                cout << "\nMatrix A Restored:\n";
                displayMatrix(A.values, A.rows, A.cols);
            }

            if (B.exists)
            {
                restoreMatrix(B);

                cout << "\nMatrix B Restored:\n";
                displayMatrix(B.values, B.rows, B.cols);
            }

            cout << "\nOriginal matrices restored successfully!\n";

            pauseScreen();
            break;
        }

        case 0:
            cout << "\nThank you for using the Matrix Workbench! Goodbye!\n";
            break;

        default:
            cout << "Invalid choice.\n";
            pauseScreen();
        }

    } while (choice != 0);

    return 0;
}

// ===================== FUNCTION DEFINITIONS =====================

// Generate unique matrix ID
int generateUniqueID()
{
    int id;
    bool duplicate;

    do
    {
        duplicate = false;

        id = 10000 + rand() % 90000;

        for (int i = 0; i < usedCount; i++)
        {
            if (usedIDs[i] == id)
            {
                duplicate = true;
                break;
            }
        }

    } while (duplicate);

    usedIDs[usedCount++] = id;

    return id;
}

// Copy matrix data
void copyMatrix(int dest[][MAX], const int src[][MAX], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

// Display matrix
void displayMatrix(const int mat[][MAX], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << setw(6) << mat[i][j];
        }
        cout << endl;
    }
}

// Create matrix
void createMatrix(Matrix &m, char name)
{
    //system("cls");

    cout << "Enter number of rows for Matrix "
         << name << " (1-10): ";
    cin >> m.rows;

    cout << "Enter number of columns for Matrix "
         << name << " (1-10): ";
    cin >> m.cols;

    int method;

    cout << "Choose input method:\n";
    cout << "1- Manual\n";
    cout << "2- Random\n";
    cout << "Choice: ";
    cin >> method;

    if (method == 1)
    {
        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                cout << "Enter "
                     << name << "[" << i << "][" << j << "]: ";

                cin >> m.values[i][j];
            }
        }
    }
    else
    {
        int minVal, maxVal;

        cout << "Enter minimum random value: ";
        cin >> minVal;

        cout << "Enter maximum random value: ";
        cin >> maxVal;

        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                m.values[i][j] =
                    minVal + rand() % (maxVal - minVal + 1);
            }
        }
    }

    // Backup original matrix
    copyMatrix(m.original, m.values, m.rows, m.cols);

    m.id = generateUniqueID();
    m.exists = true;

    cout << "\nGreat! Matrix "
         << name << " has been created.\n";

    cout << "Matrix " << name
         << " ID: " << m.id << endl;

    cout << "Order: "
         << m.rows << " x " << m.cols << endl;

    displayMatrix(m.values, m.rows, m.cols);

    pauseScreen();
}

// Check multiplication compatibility
bool canMultiply(const Matrix &A, const Matrix &B)
{
    return (A.cols == B.rows);
}

// Matrix multiplication
MultiplyResult multiplyMatrices(const Matrix &A, const Matrix &B)
{
    MultiplyResult result;

    result.compatible = canMultiply(A, B);

    result.scalarMultiplications = 0;
    result.additions = 0;

    if (!result.compatible)
    {
        return result;
    }

    result.resultRows = A.rows;
    result.resultCols = B.cols;

    for (int i = 0; i < A.rows; i++)
    {
        for (int j = 0; j < B.cols; j++)
        {
            result.result[i][j] = 0;

            for (int k = 0; k < A.cols; k++)
            {
                result.result[i][j] +=
                    A.values[i][k] * B.values[k][j];

                result.scalarMultiplications++;

                if (k > 0)
                {
                    result.additions++;
                }
            }
        }
    }

    return result;
}

// Scalar multiplication
ScalarResult scalarMultiply(Matrix &m, int k)
{
    ScalarResult result;

    result.scalar = k;
    result.multiplications = 0;
    result.writes = 0;

    for (int i = 0; i < m.rows; i++)
    {
        for (int j = 0; j < m.cols; j++)
        {
            m.values[i][j] *= k;

            result.multiplications++;
            result.writes++;
        }
    }

    return result;
}

// Restore original matrix
void restoreMatrix(Matrix &m)
{
    copyMatrix(m.values, m.original, m.rows, m.cols);
}

// Display matrices
void displayCurrentMatrices(const Matrix &A, const Matrix &B)
{
    //system("cls");

    if (A.exists)
    {
        cout << "------ Matrix A ------\n";
        cout << "Matrix ID: " << A.id << endl;
        cout << "Order: "
             << A.rows << " x " << A.cols << endl;

        displayMatrix(A.values, A.rows, A.cols);
    }
    else
    {
        cout << "Matrix A has not been created.\n";
    }

    cout << endl;

    if (B.exists)
    {
        cout << "------ Matrix B ------\n";
        cout << "Matrix ID: " << B.id << endl;
        cout << "Order: "
             << B.rows << " x " << B.cols << endl;

        displayMatrix(B.values, B.rows, B.cols);
    }
    else
    {
        cout << "Matrix B has not been created.\n";
    }

    pauseScreen();
}

// Compatibility report
void checkCompatibility(const Matrix &A, const Matrix &B)
{
    //system("cls");

    if (!A.exists || !B.exists)
    {
        cout << "Both matrices must exist.\n";
        pauseScreen();
        return;
    }

    cout << "------ Compatibility Report ------\n";

    cout << "Matrix A Order: "
         << A.rows << " x " << A.cols << endl;

    cout << "Matrix B Order: "
         << B.rows << " x " << B.cols << endl;

    // A x B
    if (canMultiply(A, B))
    {
        cout << "A x B: Valid\n";
        cout << "Result Order: "
             << A.rows << " x " << B.cols << endl;
    }
    else
    {
        cout << "A x B: Not Defined\n";
    }

    // B x A
    if (canMultiply(B, A))
    {
        cout << "B x A: Valid\n";
        cout << "Result Order: "
             << B.rows << " x " << A.cols << endl;
    }
    else
    {
        cout << "B x A: Not Defined\n";
    }

    cout << "\nNote: Even when both products are defined, ";
    cout << "matrix multiplication is generally not commutative.\n";

    cout << "\nTheoretical Complexity:\n";
    cout << "Time Complexity: O(1)\n";
    cout << "Auxiliary Space: O(1)\n";

    cout << "----------------------------------\n";

    pauseScreen();
}

// Multiplication report
void multiplicationReport(const Matrix &A,
                          const Matrix &B,
                          string label)
{
    //system("cls");

    if (!A.exists || !B.exists)
    {
        cout << "Both matrices must exist.\n";
        pauseScreen();
        return;
    }

    MultiplyResult result = multiplyMatrices(A, B);

    if (!result.compatible)
    {
        cout << "Multiplication is not defined.\n";
        pauseScreen();
        return;
    }

    cout << "------ Matrix Multiplication Report ------\n";

    cout << "\nMatrix 1:\n";
    displayMatrix(A.values, A.rows, A.cols);

    cout << "\nMatrix 2:\n";
    displayMatrix(B.values, B.rows, B.cols);

    cout << "\n" << label << ":\n";
    displayMatrix(result.result,
                  result.resultRows,
                  result.resultCols);

    cout << "\nResult Order: "
         << result.resultRows
         << " x "
         << result.resultCols << endl;

    cout << "Scalar Multiplications: "
         << result.scalarMultiplications << endl;

    cout << "Additions: "
         << result.additions << endl;

    cout << "\nTheoretical Complexity:\n";

    cout << "Time Complexity: O(rA x cA x cB)\n";
    cout << "For square matrices n x n: O(n^3)\n";

    cout << "Auxiliary Space: O(rA x cB)\n";

    cout << "------------------------------------------\n";

    pauseScreen();
}

// Pause screen
void pauseScreen()
{
    cout << "\nPress any key to return to Main Menu!\n";
    getch();
}