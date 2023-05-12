#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <limits>

using namespace std;


//class for sample ransac testing algorithm, a implementation of the transaltion of python ransac given
//on the wiki page for RANSAC
class RANSAC{
public:
    int n; //this are all the initalized values
    int k;
    double t;
    int d;
    int yDataPoints;
    int confirmedInliers;
    double bestFitSlope; //these are the best fit slop and error markers, only update when a better fit is found for the data 
    double bestError;
    int value;
    double maybe_slope; //these are for the maybe inlier data points gien
    double maybe_int;
    double better_slope; //this is the new best slope line calculated to test the fit on just the inlier data points
    double better_int;

    RANSAC() {} //default the constructor

    RANSAC(int n, int k, double t, int d){
        this->n = n;    //this is the min # of data points required to estimate model params
        this->k = k;    //this the min # of ops allowed
        this->t = t;    //this is threshold value
        this->d = d;    //this is lowest data points required to fit model to data
        this->bestFitSlope = NULL;
        this->bestError = numeric_limits<double>::infinity();
    }

    double fit(double x[], double y[], int size){
        int i = 0;
        int b = 0;
        while (i < this->k){
            srand(time(0));
            value = this->n + (rand() % size); //get random number for mybe inlier data points to select
            //now copy x and y points into new array
            
            double tempX[value];
            double tempY[value];
            for(int w = 0; w < value; w++){
                tempX[w] = x[w];
                tempY[w] = y[w];
            }

            
            //cout << " got here : " << b << endl;

            //now get maybe model using points
            getMaybeModel(tempX, tempY, value);
            
            confirmedInliers = 0;
            double confirmedInlierX[value];
            double confirmedInlierY[value];
            int s = 0;
            for(int j = 0; j < size; j++){
                // Calculate error
                double y_pred = this->maybe_slope * x[j] + this->maybe_int;
                double error = y[j] - y_pred;
                if(error < t){ //if error is less than thresshold we can add inlier point and total ammount collected can increment
                    confirmedInliers++;
                    confirmedInlierX[s] = x[j]; //add points to confirmed inliers
                    confirmedInlierY[s] = y[j];
                    s++;
                }
            }
            //cout << "got here" << value << endl;
            if (confirmedInliers > this->d){ //then there is a decent fit to the model, but have to test
                //get better model using inlier data points
                getBetterModel(confirmedInlierX, confirmedInlierY, confirmedInliers);
                //get betterErr as meansquared error
                double mse = 0;
                double error2 = 0;
                for(int u = 0; u < confirmedInliers; u++){
                    double y_pred = this->better_slope * confirmedInlierX[u] + this->better_int;
                    error2 = confirmedInlierY[u] - y_pred;
                    mse += error2*error2; 
                }
                mse /= confirmedInliers; //calculate mean squared error
                if(error2 < this->bestError){ //if the error is less than the previous error update new slope and best fit error
                    this->bestFitSlope = better_slope; //set new values
                    this->bestError = error2;
                }
            }
            i++; //increment i for loop
            b++; //increment here for debugging purposes
        }
        return bestFitSlope; //return th best fit line found after iterations ran through
    }

    void getMaybeModel(double x[], double y[], int length){ //calculations to find the line of best fit for subset data points
        double x_mean = 0, y_mean = 0;
        for (int i = 0; i < length; i++) { //loop to get the x and y data point means
            x_mean += x[i];  //sum up values
            y_mean += y[i];
        }
        x_mean /= length; //caluculate mean for x and y points
        y_mean /= length;

        // Calculating the slope and y-intercept for the maybe model 
        double numerator = 0, denominator = 0;
        for (int i = 0; i < length; i++) { //loop through to sum up numerator and denominator values
            numerator += (x[i] - x_mean) * (y[i] - y_mean);  //formula calculations for least squared method
            denominator += pow(x[i] - x_mean, 2);
        }
        maybe_slope = numerator / denominator; //get final slope and intercept calculations from data points
        maybe_int = y_mean - maybe_slope * x_mean;
    }

    void getBetterModel(double x[], double y[], int length){
        double x_mean = 0, y_mean = 0;
        for (int i = 0; i < length; i++) {
            x_mean += x[i];
            y_mean += y[i];
        }
        x_mean /= length; //get mean values
        y_mean /= length;

        // Calculating the slope and y-intercept for the maybe model 
        double numerator = 0, denominator = 0;
        for (int i = 0; i < length; i++) { //calcualtrions to sum up values using Least squared method
            numerator += (x[i] - x_mean) * (y[i] - y_mean);
            denominator += pow(x[i] - x_mean, 2); //get denominator of lease squared formula for slope
        }
        better_slope = numerator / denominator; //get final slope and intercept
        maybe_int = y_mean - maybe_slope * x_mean;
    }


};

int main(){

    //tested x and y data points I can compare to the python implementation
    double x[] = {-0.848,-0.800,-0.704,-0.632,-0.488,-0.472,-0.368,-0.336,-0.280,-0.200,-0.00800,-0.0840,0.0240,0.100,0.124,0.148,0.232,0.236,0.324,0.356,0.368,0.440,0.512,0.548,0.660,0.640,0.712,0.752,0.776,0.880,0.920,0.944,-0.108,-0.168,-0.720,-0.784,-0.224,-0.604,-0.740,-0.0440,0.388,-0.0200,0.752,0.416,-0.0800,-0.348,0.988,0.776,0.680,0.880,-0.816,-0.424,-0.932,0.272,-0.556,-0.568,-0.600,-0.716,-0.796,-0.880,-0.972,-0.916,0.816,0.892,0.956,0.980,0.988,0.992,0.00400};
    double y[] = {-0.917,-0.833,-0.801,-0.665,-0.605,-0.545,-0.509,-0.433,-0.397,-0.281,-0.205,-0.169,-0.0531,-0.0651,0.0349,0.0829,0.0589,0.175,0.179,0.191,0.259,0.287,0.359,0.395,0.483,0.539,0.543,0.603,0.667,0.679,0.751,0.803,-0.265,-0.341,0.111,-0.113,0.547,0.791,0.551,0.347,0.975,0.943,-0.249,-0.769,-0.625,-0.861,-0.749,-0.945,-0.493,0.163,-0.469,0.0669,0.891,0.623,-0.609,-0.677,-0.721,-0.745,-0.885,-0.897,-0.969,-0.949,0.707,0.783,0.859,0.979,0.811,0.891,-0.137};

    //create ransac object
    RANSAC test(10, 100, 0.05, 10);
    //get best first line
    double answer = test.fit(x, y, (sizeof(x) / sizeof(x[0])));

    //print out the slope for the line of best fit returned after running the algorithm
    cout << "Slope: " << answer << endl;
}



//Below is the given psuedo code from the RANSAC webpage that I am trying to implemente and test
/*
Given:
    data – A set of observations.
    model – A model to explain the observed data points.
    n – The minimum number of data points required to estimate the model parameters.
    k – The maximum number of iterations allowed in the algorithm.
    t – A threshold value to determine data points that are fit well by the model (inlier).
    d – The number of close data points (inliers) required to assert that the model fits well to the data.

Return:
    bestFit – The model parameters which may best fit the data (or null if no good model is found).


iterations = 0
bestFit = null
bestErr = something really large // This parameter is used to sharpen the model parameters to the best data fitting as iterations goes on.

while iterations < k do
    maybeInliers := n randomly selected values from data
    maybeModel := model parameters fitted to maybeInliers
    confirmedInliers := empty set
    for every point in data do
        if point fits maybeModel with an error smaller than t then
             add point to confirmedInliers
        end if
    end for
    if the number of elements in confirmedInliers is > d then
        // This implies that we may have found a good model.
        // Now test how good it is.
        betterModel := model parameters fitted to all the points in confirmedInliers
        thisErr := a measure of how well betterModel fits these points
        if thisErr < bestErr then
            bestFit := betterModel
            bestErr := thisErr
        end if
    end if
    increment iterations
end while

return bestFit
*/