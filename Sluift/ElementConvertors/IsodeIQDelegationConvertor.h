/*
 * Copyright (c) 2014-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/IsodeIQDelegation.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class IsodeIQDelegationConvertor : public GenericLuaElementConvertor<IsodeIQDelegation> {
        public:
            IsodeIQDelegationConvertor(LuaElementConvertors* convertors);
            virtual ~IsodeIQDelegationConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<IsodeIQDelegation> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<IsodeIQDelegation>) SWIFTEN_OVERRIDE;
            virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;

        private:
            LuaElementConvertors* convertors;
    };
}
