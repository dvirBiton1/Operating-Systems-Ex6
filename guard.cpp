#include <iostream>
#include <stdexcept>
#include <pthread.h>

using namespace std;

void* ptr;

void changePtr(void* p){
    ptr = p;
}

int main(int argc, char const *argv[])
{
    
    cout << " hello \n";
    return 0;
}
