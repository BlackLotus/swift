/*
 * Copyright (c) 2013-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/PubSubOwnerSubscription.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class PubSubOwnerSubscriptionConvertor : public GenericLuaElementConvertor<PubSubOwnerSubscription> {
        public:
            PubSubOwnerSubscriptionConvertor();
            virtual ~PubSubOwnerSubscriptionConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<PubSubOwnerSubscription> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<PubSubOwnerSubscription>) SWIFTEN_OVERRIDE;
            virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;
    };
}
