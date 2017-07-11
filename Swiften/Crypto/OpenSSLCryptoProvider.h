/*
 * Copyright (c) 2013-2017 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/Override.h>
#include <Swiften/Crypto/CryptoProvider.h>

namespace Swift {
    class OpenSSLCryptoProvider : public CryptoProvider {
        public:
            OpenSSLCryptoProvider();
            virtual ~OpenSSLCryptoProvider() SWIFTEN_OVERRIDE;

            virtual Hash* createSHA1() SWIFTEN_OVERRIDE;
            virtual Hash* createMD5() SWIFTEN_OVERRIDE;
            virtual ByteArray getHMACSHA1(const SafeByteArray& key, const ByteArray& data) SWIFTEN_OVERRIDE;
            virtual ByteArray getHMACSHA1(const ByteArray& key, const ByteArray& data) SWIFTEN_OVERRIDE;
            virtual bool isMD5AllowedForCrypto() const SWIFTEN_OVERRIDE;
    };
}
