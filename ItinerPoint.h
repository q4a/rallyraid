
#ifndef ITITERPOINT_H
#define ITITERPOINT_H

#include "ObjectWireGlobalObject.h"


class ItinerPoint : public ObjectWireGlobalObject
{
public:
    ItinerPoint(const irr::core::vector3df& apos);
    virtual ~ItinerPoint();

    virtual void editorRender(bool last);
    
private:
    virtual void updateVisible();

};

#endif // ITITERPOINT_H
