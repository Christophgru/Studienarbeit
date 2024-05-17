
#include <vector>
#include <algorithm>
#include <cmath>
#include "../../../Backend/calc.h"
#include "../../../Backend/painthelper.h"



using namespace calc;

int main() {
    

        std::vector<line>vec;
        line l1=line({0,15},{10,-1});
        line l2=line({0,5},{10,1});
        /* line l3=line({18,0},{-1,1});
        line l4=line({18,0},{0,1});
        line l5=line({2,0},{0,1});
        line l6=line({2,18},{1,0}); */
        vec.push_back(l1);
        vec.push_back(l2);
        /* vec.push_back(l3);
        vec.push_back(l4);
        vec.push_back(l5);
        vec.push_back(l6); */

        paintpicture(vec, 1000,1000,50.0f,30);
        
  

    return 0;
}