#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    double tmp;
    double force = 0;
    int n;
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cin >> tmp;
        force += tmp;
    }

    cout << force << "N" << endl;
    return 0;
}
