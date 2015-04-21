#include "Timer.h"

Timer::Timer()
{
}

void Timer::start_timing(string passed_in_name)
{
    name.push(passed_in_name);
    starting_times.push(glfwGetTime());
}
double Timer::end_timing()
{
    double elapsedTime = 0;
    if (!starting_times.empty())
    {
        elapsedTime = glfwGetTime() - starting_times.top();
        name.pop();
        starting_times.pop();
    }
    else
        cout << "You don't have a starting time on the stack... check yourself" << endl;

    return elapsedTime;
}