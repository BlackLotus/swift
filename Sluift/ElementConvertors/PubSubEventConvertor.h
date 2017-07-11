/*
 * Copyright (c) 2013-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/PubSubEvent.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class PubSubEventConvertor : public GenericLuaElementConvertor<PubSubEvent> {
        public:
            PubSubEventConvertor(LuaElementConvertors* convertors);
            virtual ~PubSubEventConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<PubSubEvent> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<PubSubEvent>) SWIFTEN_OVERRIDE;

        private:
            LuaElementConvertors* convertors;
    };
}
