/*
 * Copyright (c) 2010-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Elements/SecurityLabel.h>

#include <Sluift/GenericLuaElementConvertor.h>

namespace Swift {
    class LuaElementConvertors;

    class SecurityLabelConvertor : public GenericLuaElementConvertor<SecurityLabel> {
        public:
            SecurityLabelConvertor();
            virtual ~SecurityLabelConvertor() SWIFTEN_OVERRIDE;

            virtual std::shared_ptr<SecurityLabel> doConvertFromLua(lua_State*) SWIFTEN_OVERRIDE;
            virtual void doConvertToLua(lua_State*, std::shared_ptr<SecurityLabel>) SWIFTEN_OVERRIDE;
            virtual boost::optional<Documentation> getDocumentation() const SWIFTEN_OVERRIDE;
    };
}
