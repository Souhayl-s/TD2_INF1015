
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "./GSL/include/gsl/span"

using namespace std;
using namespace iter;
using namespace gsl;
struct MyStruct {
    int myArray[5];
};

void modifyArray(MyStruct& myStruct) {
    // Modifying the array inside the function
    for (int i = 0; i < 5; ++i) {
        myStruct.myArray[i] *= 2;
    }
}

struct ListeNum1D{
    int* numbers ;
} ;

void EditStruct(ListeNum1D &list){
    list.numbers[0] = 99 ; 
}


int main() {
    cout << "\n35m════════════════════════════════════════\n" ;
    return 0;
}
