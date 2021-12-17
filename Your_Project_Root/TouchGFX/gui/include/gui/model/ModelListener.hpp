#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

#include "stdint.h"

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void setImage(uint8_t data);
    virtual void setStatus(uint8_t connection_status);

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
