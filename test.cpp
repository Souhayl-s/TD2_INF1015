
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
struct ListeNum2D{
    ListeNum1D liste ;
} ;

int* obtenirList1D(ListeNum2D* listaModifier){
    return (listaModifier->liste).numbers ; 
}


int main() {

    ListeNum1D sample ; 
    sample.numbers = new int [2] {0,0} ; 

    ListeNum2D sample2D = {sample} ;
    ListeNum2D* ptrList2D = &sample2D ;

    cout << (ptrList2D->liste).numbers[0] <<endl; 
    obtenirList1D(ptrList2D)[0] = 999 ;
    cout << (ptrList2D->liste).numbers[0] ; 


    return 0;
}
