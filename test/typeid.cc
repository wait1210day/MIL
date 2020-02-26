#include <iostream>
#include <string>
#include <typeindex>
#include <vector>
#include <map>
using namespace std;

int main(int argc, char const *argv[])
{
    int ip;
    double op;
    char cp;

    map<type_index, void *> typesmap;
    typesmap[type_index(typeid(ip))] = (void *) &ip;
    typesmap[type_index(typeid(op))] = (void *) &op;
    typesmap[type_index(typeid(cp))] = (void *) &cp;

    for (pair<const type_index, void *>& ir : typesmap)
    {
        cout << "type " << ir.first.name() << " at " << ir.second << endl;
        cout << dynamic_cast<ir.first>(ir.second) << endl;
    }

    return 0;
}
