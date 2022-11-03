#include "NscldaqSource.h"
#include <iostream>

namespace Charon {

    NscldaqSource::NscldaqSource(const std::string& ringName) :
        m_isActive(false), m_dataSource(nullptr)
    {
        Init(ringName);
    }

    NscldaqSource::~NscldaqSource()
    {
        delete m_dataSource;
    }

    void NscldaqSource::Init(const std::string& ringName)
    {
        try
        {
            m_dataSource = CDataSourceFactory::makeSource(ringName, {}, {});
            std::cerr << "Successfully created data source named " << ringName << std::endl;
            m_isActive = true;
        }
        catch(CException& e)
        {
            std::cerr << "Unable to create data source with name " << ringName << std::endl;
            return;
        }
    }

    StygianMessage NscldaqSource::GetData()
    {
        static StygianMessage emptyResult;

        if(m_dataSource == nullptr)
            return emptyResult;

        CRingItem* ringItem = m_dataSource->getItem();
        if(ringItem == NULL) //ick
            return emptyResult;

        switch(ringItem->type)
        {
            case PHYSICS_EVENT: return HandlePhysicsEvent((CPhysicsEventItem*)ringItem);
            case BEGIN_RUN: HandleBeginRun((CRingStateChangeItem*)ringItem); break;
            case END_RUN: HandleEndRun((CRingStateChangeItem*)ringItem); break;
        }

        return emptyResult;
    }

    StygianMessage NscldaqSource::HandlePhysicsEvent(CPhysicsEventItem* item)
    {
        StygianMessage data;
        uint8_t* bodyPointer = (uint8_t*) item->getBodyPointer();
        //Least significant 12 bits of first 16 bit word gives the number of 16 bit words TO FOLLOW
        data.size = *(uint16_t*)bodyPointer * 2; //Convert to bytes
        bodyPointer += 2; //Move past the size word
        //Copy the underlying body array 
        data.body.insert(data.body.begin(), bodyPointer, bodyPointer+data.size);
        return data;
    }

    void NscldaqSource::HandleBeginRun(CRingStateChangeItem* item)
    {
        std::cout << "The ferryman has recieved a new call to pickup travellers -- Run No: "
                  << item->getRunNumber() << std::endl;
        return;
    }

    void NscldaqSource::HandleEndRun(CRingStateChangeItem* item)
    {
        std::cout << "The ferryman has delievered all travellers -- Run No: "
                  << item->getRunNumber() << std::endl;
        return;
    }
}