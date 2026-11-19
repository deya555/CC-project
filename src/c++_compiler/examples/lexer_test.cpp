Set-Content "backend/c++_compiler/examples/lexer_test.cpp" @(
'#include <iostream>'
'using namespace std;'
''
'int main() {'
'    int number = 10;'
'    float price = 25.5;'
'    bool flag = true;'
'    cout << number << endl;'
'    return 0;'
'}'
)