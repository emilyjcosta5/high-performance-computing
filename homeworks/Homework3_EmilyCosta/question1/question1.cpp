#include <stdio.h>
#include <math.h>
#include <iostream>
#include <chrono>
using namespace std::chrono;
using namespace std;

int fact(int c){
   int factorial=1;
   for (int i=1; i<=c; i++)
      factorial *=i;
   return factorial;
}

double taylor_d(double x, int n){
    double y = ( 1. / x ) + 1.; // first term in equation and first term in approx.
    for (int i=1; i<=n; i++)
        y += ( (pow(x,i)) / fact(i) );
    std::cout << y << std::endl;
    return y;
}

float taylor_f(float x, int n){
    float one = 1.;
    float y = ( one / x ) + one; // first term in equation and first term in approx.
    for (int i=1; i<=n; i++)
        y += ( (pow(x,i)) / fact(i) );
    std::cout << y << std::endl;
    return y;
}

int main(){
    int x;
    int n;
    n = 25;
    //taylor_f(float(x),n);

    x = 2;
    cout << "\nx=" << x << "\n----" << endl;
    auto t1 = high_resolution_clock::now();
    //taylor_f(float(x),n);
    auto t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;

    t1 = high_resolution_clock::now();
    taylor_d(double(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;

    x = 5;
    cout << "\nx=" << x << "\n----" << endl;
    t1 = high_resolution_clock::now();
    //taylor_f(float(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;
    
    t1 = high_resolution_clock::now();
    taylor_d(double(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;

    x = 10;
    cout << "\nx=" << x << "\n----" << endl;
    t1 = high_resolution_clock::now();
    //taylor_f(float(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;
    
    t1 = high_resolution_clock::now();
    taylor_d(double(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;

    x = 20;
    cout << "\nx=" << x << "\n----" << endl;
    t1 = high_resolution_clock::now();
    //taylor_f(float(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;
    
    t1 = high_resolution_clock::now();
    taylor_d(double(x),n);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>(t2 - t1);
    cout << duration.count() << endl;

    return 0;
}