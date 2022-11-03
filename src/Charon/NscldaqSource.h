#ifndef NSCLDAQ_SOURCE_H
#define NSCLDAQ_SOURCE_H

#include "daq/format/DataFormat.h"
#include "utilities/IO/CDataSourceFactory.h"
#include "utilities/IO/CDataSource.h"
#include "daq/format/CRingItem.h"
#include "daq/format/CRingItemFactory.h"
#include "daq/format/CRingStateChangeItem.h"
#include "daq/format/CPhysicsEventItem.h"
#include "daq/format/CRingScalerItem.h"
#include "libtcl/include/exception/Exception.h"

#include <string>
#include <vector>

#include "StygianMessage.h"

namespace Charon {

    class NscldaqSource
    {
    public:
        NscldaqSource(const std::string& ringName);
        ~NscldaqSource();

        bool IsActive() const { return m_isActive; }

        StygianMessage GetData();

    private:
        void Init(const std::string& ringName);

        StygianMessage HandlePhysicsEvent(CPhysicsEventItem* item);
        void HandleBeginRun(CRingStateChangeItem* item);
        void HandleEndRun(CRingStateChangeItem* item);

        bool m_isActive;
        CDataSource* m_dataSource;

    };

}

#endif