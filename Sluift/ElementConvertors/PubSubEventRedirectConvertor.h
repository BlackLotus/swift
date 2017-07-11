/*
 * Copyright (c) 2013-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/PubSubEventRedirect.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class PubSubEventRedirectConvertor : public GenericLuaElementConvertor<PubSubEventRedirect> {
        public:
            PubSubEventRedirectConvertor();
            virtual ~PubSubEventRedirectConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<PubSubEventRedirect> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<PubSubEventRedirect>) SWIFTEN_OVERRIDE;
            virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;
    };
}
