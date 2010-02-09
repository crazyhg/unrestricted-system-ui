#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <DuiWidget>

class DuiSlider;
class DuiContainer;
class DuiStatusIndicatorMenuInterface;

class VolumeBar : public DuiWidget
{
    Q_OBJECT

public:
    VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
               QGraphicsItem *parent = NULL);
    virtual ~VolumeBar ();

private:
    DuiSlider   *m_bar;

};

#endif
