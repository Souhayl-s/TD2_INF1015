#include <iostream> 
#include ".\cppitertools\range.hpp"
#include "./GSL/include/gsl/span"

using namespace std ; 
using namespace iter ;
using namespace gsl;


void EditTableau(int* *(&tableau)){
    int num = 5 ; 
    int* ptrNum = &num ;
    tableau[0] = ptrNum ; 
}

int main(){
    int num = 99 ; 
    int* ptrNum = &num ;
    int* *tableau {&ptrNum};
    cout << tableau[0] << endl;
    EditTableau(tableau) ;
    cout << tableau[0] << endl;

}