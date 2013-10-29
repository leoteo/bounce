#include <iostream>
#include <vector>

class A {
    protected:
        double * a;
    public:
        A() { 
            a = new double(0);
            std::cout << "Constructor A(), a=" << *a << std::endl; 
        }
        A(double x) { 
            a = new double(x);
            std::cout << "Constructor A(double x), a=" << *a << std::endl; 
        }

        ~A() { 
            std::cout<< "Destructor, a=" << *a << std::endl; 
            delete a; 
        }
        // Uncommenting the lines below solves the problem
//        A(const A& b) { 
//            a = new double( *(b.a) );
//            std::cout << "Copy constructor" << std::endl; 
//        }
//        A& operator=(const A& b){
//            a = new double( *(b.a));
//            return *this;
//        }
};

int main(){

    A bad_apple = A();
    // This first calls the implicit copy constructor, which copies A.a
    // and then calls the destructor on the temporary object A(N), thus freeing
    // the space pointed to by A.a .
    bad_apple = A(10);
    std::cout << "Before termination of program" << std::endl;

    return 0;
}
    // Here, bad_apple moves out of scope, so its destructor is called.
    // But the memory to which bad_apple.a points has already been freed...

