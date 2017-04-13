#include <stdio.h>
#include <Rcpp.h>
#include <math.h>

using namespace Rcpp;
// [[Rcpp::plugins(cpp11)]]



double CalculateMean(double * values, int size)
{
    double sum = 0;
    for(int i = 0; i < size; i++)
    {
      sum += values[i];
    }
    return (sum / size);
}

double CalculateVariance(double * values, double mean, int size)
{
    double temp = 0;

    for(int i = 0; i < size; i++)
    {
        temp += (values[i] - mean) * (values[i] - mean);
    }
    return temp / (size - 1 );
}

double Calculate_StandardDeviation(double * values, int size)
{
    double mean = CalculateMean(values, size);
    return sqrt(CalculateVariance(values, mean, size));
}

// [[Rcpp::export]]
StringMatrix which_rows_with_no_sd_cpp(CharacterMatrix x, NumericVector sampleCols)
{
    int colSize = sampleCols.size();
    CharacterMatrix out(x.nrow(), (x.ncol()+1));

    for(int i = 0; i < x.nrow(); i++)
    {
        double expressions[colSize];
        //Extract an array containing the sample expression values
        for(int j = 0; j < colSize; j++)
        {
            std::string a = as<std::string>(x(i, sampleCols[j]));
            double expression = atof(a.c_str());
            expressions[j] = expression;
        }

        double stdev = Calculate_StandardDeviation(expressions, colSize);
        for(int j = 0; j < x.ncol(); j++)
        {
            out(i, j) = x(i, j );
        }
        out(i, x.ncol()) = stdev;

    }

    return out;
}
