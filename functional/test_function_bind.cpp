/*************************************************************************
	> File Name: test_function_bind.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年09月03日 星期一 16时13分47秒
 ************************************************************************/

#include<functional>
#include<iostream>

using namespace std;
using namespace std::placeholders;    // adds visibility of _1, _2, _3, ...



double my_divide(double x, double y) {
    return x/y;
}

struct MyPair {
    double a, b;
    double multiply() {
        return a * b;
    }
};

int main(int argc, char* argv[]) {
    // binding functions
    std::function<double ()> fn_five = std::bind(my_divide, 10, 2);
    cout << fn_five() << "\n";

    std::function<double (double)> fn_half = std::bind(my_divide, _1, 2.0);
    cout << fn_half(8.0) << "\n";

    auto fn_invert = std::bind(my_divide, _2, _1);
    cout << fn_invert(10, 2) << "\n";

    std::function<double (double, double)> fn_rounding = std::bind<int>(my_divide, _1, _2);
    cout << fn_rounding(10, 3) << "\n";

    MyPair ten_two {10, 2};
    // binding members
    std::function<double (struct MyPair &)> bound_member_fn = std::bind(&MyPair::multiply, _1);
    cout << bound_member_fn(ten_two) << "\n";

    std::function<double ()> bound_member_data = std::bind(&MyPair::a, ten_two);
    cout << bound_member_data() << "\n";

    return 0;
}
