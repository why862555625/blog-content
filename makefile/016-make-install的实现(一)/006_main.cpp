//
//

#include<iostream>
#include<unistd.h>
using namespace std;



int main(){

    int i=0;
    while (true){
        i++;
        cout<<"006-main-running-"<<i<<endl;
        sleep(1);
    }

    return 0;
}