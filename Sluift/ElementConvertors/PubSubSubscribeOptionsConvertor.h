/*
 * Copyright (c) 2013-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/PubSubSubscribeOptions.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class PubSubSubscribeOptionsConvertor : public GenericLuaElementConvertor<PubSubSubscribeOptions> {
        public:
            PubSubSubscribeOptionsConvertor();
            virtual ~PubSubSubscribeOptionsConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<PubSubSubscribeOptions> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<PubSubSubscribeOptions>) SWIFTEN_OVERRIDE;
            virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;
    };
}
