/**
 * @brief COSC2430 - HW 1 
 * 
 * @file densemult.cpp
 * @author Omer Tarik Koc
 * @date 2018-08-28
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include "ArgumentManager.h"

bool is_number(const std::string &);

class MatrixMultiply
{
  private:
    double matrixA[20][20];
    int matrixA_row_count;
    int matrixA_column_count;
    double matrixB[20][20];
    int matrixB_row_count;
    int matrixB_column_count;
    double matrixC[20][20];
    int matrixC_row_count;
    int matrixC_column_count;
    std::string outfilename;

    /**
     * @brief When an error founds this function will terminate the program and write "error" to the output file
     * 
     */
    int error()
    {
        ofstream outfile;
        outfile.open(outfilename);
        outfile << "error";
        outfile.close();
        std::cout << "error";
        exit(EXIT_FAILURE);
        return -1;
    }

    /**
    * @brief Determine if the input is number
    * @param input string the number that will be checked 
    */
    bool is_number(const std::string &input)
    {
        for (int i = 0; i < input.size(); i++)
        {
            if (!isdigit(input.at(i)))
            {
                // Check if we encounter "."
                if (input.at(i) == '.')
                {
                    if (!isdigit(input.at(i + 1)))
                    {
                        return false;
                    }
                }
                else
                {
                    std::cout << input.at(i) << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    double *parseLine(const std::string &line, int &count)
    {
        std::string delim = " ";
        std::string current;
        count = 0;
        double *p = new double[20]; // create dynamic array

        auto start = 0U;
        auto end = line.find(delim);
        while (end != std::string::npos)
        {
            current = line.substr(start, end - start);
            if (!(is_number(current)))
            {
                error();
            }

            // convert the parsed string to double and store it in the p dynamic array
            p[count] = std::stod(current);
            count++;

            //std::cout << current << std::endl;
            start = end + delim.length();
            end = line.find(delim, start);
        }
        current = line.substr(start, end);
        p[count] = std::stod(current);
        count++;
        //std::cout << current << std::endl;
        if (!(is_number(current)))
        {
            error();
        }
        //std::cout << "Column Count: " << count + 1 << std::endl;

        return p;
    }

  public:
    MatrixMultiply(std::string fileNameA, std::string fileNameB, std::string fileNameC)
    {
        outfilename = fileNameC;
        readMatrix(fileNameA, fileNameB);
    }

    /**
     * @brief Read and store the matrix to the private members
     * @param fileNameA coming from the argument A
     * @param fileNameB coming from the argument B
     */
    bool readMatrix(std::string fileNameA, std::string fileNameB)
    {
        ifstream infile;
        std::string *p;
        std::string delim = " ";
        int *row_count = 0;
        int column_count = 0;
        double *row;

        infile.open(fileNameA);
        p = new std::string;
        row_count = new int(0);

        // Find row count
        while (!infile.eof())
        {
            getline(infile, *p);

            row = parseLine(*p, column_count);

            for (int i = 0; i < column_count; i++)
            {
                matrixA[*row_count][i] = row[i];
            }
            (*row_count)++;
        }

        infile.close();

        matrixA_column_count = column_count;
        matrixA_row_count = *row_count;

        std::cout << "MATRIX A ROW COUNT: " << *row_count << std::endl;
        std::cout << "MATRIX A COLUMN COUNT: " << column_count << std::endl;

        printMatrixA();

        delete p;
    }

    void printMatrixA()
    {
        std::cout << "======== " << std::endl;
        std::cout << "Matrix A " << std::endl;
        std::cout << "======== " << std::endl;
        for (int j = 0; j < matrixA_row_count; j++)
        {
            for (int m = 0; m < matrixA_column_count; m++)
            {
                std::cout << matrixA[j][m] << " ";
            }
            std::cout << std::endl;
        }
    }

    void printMatrixB()
    {
        std::cout << "======== " << std::endl;
        std::cout << "Matrix B" << std::endl;
        std::cout << "======== " << std::endl;
        for (int j = 0; j < matrixB_row_count; j++)
        {
            for (int m = 0; m < matrixB_column_count; m++)
            {
                std::cout << matrixB[j][m] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main(int argc, char *argv[])
{
    // Check if the argument number is correct
    // Otherwise print the proper usage of the program
    if (argc < 2)
    {
        std::cout << "Please call like that: densemult \"A=<file>;B=<file>;C=<file>\"" << std::endl;
        return -1;
    }

    // Using ArgumentManager.h parse the arguments and print out
    ArgumentManager am(argc, argv);
    std::string matrixA = am.get("A");
    std::string matrixB = am.get("B");
    std::string outfilename = am.get("C");
    std::cout << "File name for matrix A: " << matrixA << std::endl
              << "File name for matrix B: " << matrixB << std::endl
              << "File name for output matrix C: " << outfilename << std::endl;

    MatrixMultiply m(matrixA, matrixB, outfilename);
    return 0;
}