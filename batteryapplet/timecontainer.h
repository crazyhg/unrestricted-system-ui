#ifndef TIMECONTAINER_H
#define TIMECONTAINER_H

#include <DuiContainer>

class DuiLabel;

class TimeContainer : public DuiContainer
{
public:
    TimeContainer(DuiWidget *parent);
    virtual ~TimeContainer();

    void updateTimeLabel(int mins);

protected: //methods
    virtual void setLayout() = 0;

protected: //attributes    
    DuiLabel *timeLabel;    

};

#endif // STANDBYTIMECONTAINER_H