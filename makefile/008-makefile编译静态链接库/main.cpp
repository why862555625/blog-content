#include <iostream>


#include "SoTest.h"
#include "aTest.h"

class MainTest:public SoTest{

public:
    void func2(){
        printf("main-func2\n");
    }

    void func3(){
        printf("main-func3\n");
    }
};

int main() {

    MainTest t1;
    t1.func1();
    t1.func2();
    t1.func3();

    printf("\n\n");
    aTest t2;
    t2.func1();

    return 0;
}

//  DYLD_LIBRARY_PATH=./001
// export DYLD_LIBRARY_PATH