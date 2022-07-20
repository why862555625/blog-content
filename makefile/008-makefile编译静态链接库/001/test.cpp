#include <iostream>
#include "SoTest.h"

class Test:public SoTest{

public:
    void func2(){
        printf("test-func2\n");
    }


    void func3(){
        printf("test-func3\n");
    }
};


int main() {

    Test t1;
    t1.func1();
    t1.func2();
    t1.func3();

    return 0;
}