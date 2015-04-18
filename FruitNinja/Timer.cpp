#include "Timer.h"

Timer::Timer()
{
}

void Timer::start_timing(string passed_in_name)
{
    name.push(passed_in_name);
    starting_times.push(glfwGetTime());
}
void Timer::end_timing()
{
    if (!starting_times.empty())
    {
        cout << name.top().c_str() << " took " << glfwGetTime() - starting_times.top() << "seconds" << endl;
        name.pop();
        starting_times.pop();
    }
    else
        cout << "You don't have a starting time on the stack... check yourself" << endl;
}