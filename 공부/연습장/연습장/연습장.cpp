#include "pch.h"

// A 클래스 선언
class A {
private:
    int value;

public:
    A() : value(0) {}

    // friend 선언을 통해 A가 B::Render()에 private 멤버를 허용
    friend void B::Render(A* a);
};

// B 클래스 선언
class B {
public:
    // Render 함수는 A의 private 멤버에 접근할 수 있는 friend 함수
    void Render(A* a) {//정의가 다른 파일에 있으니까 되네 신기...지금은 안됨
        a->value = 42; // A의 private 멤버에 접근 가능
        cout << "A의 value 값은: " << a->value << endl; 
    }
};

int main() {
    A objA;
    B objB;
    objB.Render(&objA); // B 클래스의 Render가 A의 private 멤버에 접근 가능

    return 0;
}
