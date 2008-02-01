//Daniel Hakim

#ifndef RAM_VISION_EVENTS_01_08_2008
#define RAM_VISION_EVENTS_01_08_2008

// Project Includes
#include "core/include/Event.h"

namespace ram {
namespace vision {

class EventType 
{
    public:
    static const core::Event::EventType LIGHT_FOUND;
    static const core::Event::EventType PIPE_FOUND;
    static const core::Event::EventType GATE_FOUND;
    static const core::Event::EventType BIN_FOUND;
};

class ImageEvent : public core::Event
{
	public:
	ImageEvent(double x, double y, int numPix){this->x=x; this->y=y; this->pixCount=numPix;}
    double x;
    double y;
    int pixCount;
};

class PipeEvent : public core::Event
{
	public:
	PipeEvent(double x, double y, double angle){this->x=x;this->y=y;this->angle=angle;}
	double x;
	double y;
	double angle;
};

} // namespace vision
} // namespace ram

#endif // RAM_VISION_EVENTS_01_08_2008
