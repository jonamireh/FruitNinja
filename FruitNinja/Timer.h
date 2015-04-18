#include <iostream>
#include <stack>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class Timer
{
    stack<double> starting_times;
    stack<string> name;

public:
    Timer();
    void start_timing(string);
    void end_timing();
};