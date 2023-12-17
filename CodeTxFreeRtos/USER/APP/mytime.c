#include "mytime.h"

mytime_t myTime    ={22,18,55,2022,1,28};
mytime_t myTimeTemp={22,18,55,2022,1,28};

void MyTimeSecondRun()
{
    static int timeSecondCount=0;
    timeSecondCount++;
    if(timeSecondCount>199)
    {
        timeSecondCount=0;
        myTime.Second++;
    }
}

void MyTimeFun()
{
    if(myTime.Second>59)
    {
        myTime.Second=0;
        myTime.Minute++;
    }
    if(myTime.Minute>59)
    {
        myTime.Minute=0;
        myTime.Hour++;
    }
    if(myTime.Hour>23)
    {
        myTime.Hour=0;
    }
}

