/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Swiften/Avatars/VCardAvatarManager.h"

#include <boost/bind.hpp>

#include "Swiften/Elements/VCard.h"
#include "Swiften/StringCodecs/SHA1.h"
#include "Swiften/StringCodecs/Hexify.h"
#include "Swiften/Avatars/AvatarStorage.h"
#include "Swiften/MUC/MUCRegistry.h"
#include "Swiften/VCards/VCardManager.h"

namespace Swift {

VCardAvatarManager::VCardAvatarManager(VCardManager* vcardManager, AvatarStorage* avatarStorage, MUCRegistry* mucRegistry) : vcardManager_(vcardManager), avatarStorage_(avatarStorage), mucRegistry_(mucRegistry) {
	vcardManager_->onVCardChanged.connect(boost::bind(&VCardAvatarManager::handleVCardChanged, this, _1));
}

void VCardAvatarManager::handleVCardChanged(const JID& from) {
	// We don't check whether the avatar actually changed. Direct use of this
	// manager could cause unnecessary updates, but in practice, this will be
	// caught by the wrapping CombinedAvatarManager anyway.
	onAvatarChanged(from);
}

String VCardAvatarManager::getAvatarHash(const JID& jid) const {
	VCard::ref vCard = vcardManager_->getVCard(getAvatarJID(jid));
	if (vCard && !vCard->getPhoto().isEmpty()) {
		String hash = Hexify::hexify(SHA1::getHash(vCard->getPhoto()));
		if (!avatarStorage_->hasAvatar(hash)) {
			avatarStorage_->addAvatar(hash, vCard->getPhoto());
		}
		return hash;
	}
	else {
		return "";
	}
}

JID VCardAvatarManager::getAvatarJID(const JID& jid) const {
	JID bareFrom = jid.toBare();
	return (mucRegistry_ && mucRegistry_->isMUC(bareFrom)) ? jid : bareFrom;
}

}
