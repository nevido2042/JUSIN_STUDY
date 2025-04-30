#include "Hello.h"
#include "HelloDLL.h"

int main()
{
	Say_Hello(); //정적 라이브러리
	Say_HelloDLL(); //동적 라이브러리
}