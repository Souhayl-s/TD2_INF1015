#include <iostream>

struct MyStruct {
    int myArray[5];
};

void modifyArray(MyStruct& myStruct) {
    // Modifying the array inside the function
    for (int i = 0; i < 5; ++i) {
        myStruct.myArray[i] *= 2;
    }
}

int main() {
    MyStruct myStruct = {1, 2, 3, 4, 5};

    // Before modification
    std::cout << "Before modification: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << myStruct.myArray[i] << " ";
    }
    std::cout << std::endl;

    // Modifying the array by passing a reference to the struct
    modifyArray(myStruct);

    // After modification
    std::cout << "After modification: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << myStruct.myArray[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
