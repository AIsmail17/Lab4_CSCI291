#include<iostream>
#include<iomanip>
#include <cstdlib> 
using namespace std;

// Template function declaration for update_scale
template <typename T>
void update_scale(T& m1, T& m2, T m3 = 10);

void print_real( float number, int fieldspace, int precision);

int main(void){
    float  a, b;
    cout<<" Please input two real numbers";
    cin>>a>>b; // Input Two Numbers

    print_real(a,7,3);
    print_real(b,7,3);

    // call the function update_scale with ONLY the actual parameters a  and b
    update_scale(a, b);

    print_real(a,7,3);
    print_real(b,7,3);
}

/* complete the body of the function */
void print_real( float number, int fieldspace, int precision){
    cout << fixed << setprecision(precision) << setw(fieldspace) << number << "\n";
}

// add the function defintion (header+body) of the template function update_scale
template <typename T>
void update_scale(T& m1, T& m2, T m3) {
    // Store original values
    T Org1 = m1;
    T Org2 = m2;
    
    // Update m1: sum of original values multiplied by m3
    m1 = (Org1 + Org2) * m3;
    
    // Update m2: difference of original values multiplied by m3
    m2 = (Org1 - Org2) * m3;
}