#ifndef NSCLDAQ_SOURCE_H
#define NSCLDAQ_SOURCE_H

#include "DataFormat.h"
#include "CDataSourceFactory.h"
#include "CDataSource.h"
#include "CRingItem.h"
#include "CRingItemFactory.h"
#include "CRingStateChangeItem.h"
#include "CPhysicsEventItem.h"
#include "CRingScalerItem.h"
#include "Exception.h"

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
