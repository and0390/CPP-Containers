#include <iostream>
#include "f_list.h" //include the header file
#include <cstdint>
#include <cstring>
#include <algorithm> 
#include <vector> //include stl container vector

//overloading operator<< for f_list container
std::ostream& operator<<(std::ostream& out, andr::f_list<int32_t> obj){
    for(auto it = obj.begin(); it != obj.end(); ++it){
        out << *it << ' ';
    }
    return out;
}

static constexpr char en {'\n'};

class foo{
    public:
    foo()
        :str(nullptr){
        std::cout << "constructor default 1\n";
    }
    explicit
    foo(const char* str) 
        : str(new char[std::strlen(str) + 1]) {
        std::strcpy(this->str,str);
        std::cout << "constructor default 2\n";
    }
    foo(const foo& f) {std::cout << "copy constructor\n";}
    foo(foo&& f) {std::cout << "move constructor\n";}
    void print() const noexcept {
        std::cout << str << en;
    }
    ~foo() {std::cout << "deconstructor\n";}
    private:
    char* str;
};

int main(){

    std::vector<int32_t> ints {1,2,3,4,5,6};

    andr::f_list<int32_t> ints1 (ints.cbegin(),ints.cend()); //range base constructor, compatible with stl containers
    ints1.push_front(8); //prepends datum at the beginning of the container, constant complexity
    ints1.push_back(9); //appends datum at the end of the container, linear complexity.

    std::cout << ints1 << en;

    auto found = ints1.find(9); //find the element __val in the container,if found returns the iterator,otherwise returns end(), linear complexity

    ints1.erase(found); //erase elemet at pos, linear complexity

    ints1.erase(ints1.begin(),ints1.begin() + 1); //erase element in range [__begin,__end)

    ints1.replace_at(ints1.begin() + 2,500); //replace element at pos linear complexity

    std::cout << ints1 << en;

    std::cout << ints1.length() << en;

    andr::f_list<foo> foo1;

    foo1.emplace_front("hello world"); //construct object directly at the beginning of the container, constant complexity
    foo1.emplace_back("Hi, There!"); //construct object directly at the end of the constainer, linear complexity
    //push_front and push_back also support this!

    auto it = foo1.cbegin();

    for(; it != foo1.cend(); it++)
        it->print();
    
    foo1.clear(); //clear the container

    return 0;
}