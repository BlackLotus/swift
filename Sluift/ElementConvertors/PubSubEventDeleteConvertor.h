/*
 * Copyright (c) 2013-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/PubSubEventDelete.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class PubSubEventDeleteConvertor : public GenericLuaElementConvertor<PubSubEventDelete> {
        public:
            PubSubEventDeleteConvertor(LuaElementConvertors* convertors);
            virtual ~PubSubEventDeleteConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<PubSubEventDelete> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<PubSubEventDelete>) SWIFTEN_OVERRIDE;
            virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;

        private:
            LuaElementConvertors* convertors;
    };
}
