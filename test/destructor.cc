#include <iostream>
using namespace std;

class T1
{
public:
    T1()
    {
        cout << "T1 constructor" << endl;
    }
    ~T1()
    {
        cout << "T1 destructor" << endl;
    }
};

struct S1
{
    T1 *t1;

    ~S1()
    {
        cout << "S1 destructor" << endl;
    }
};

int main(int argc, char const *argv[])
{
    S1 *s = new S1;
    s->t1 = new T1;

    delete s->t1;
    delete s;
    getchar();
    return 0;
}
