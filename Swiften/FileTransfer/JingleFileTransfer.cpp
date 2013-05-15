/*
 * Copyright (c) 2013 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <Swiften/FileTransfer/JingleFileTransfer.h>

#include <boost/typeof/typeof.hpp>

#include <Swiften/Base/foreach.h>
#include <Swiften/JID/JID.h>
#include <Swiften/Crypto/CryptoProvider.h>
#include <Swiften/StringCodecs/Hexify.h>
#include <Swiften/Jingle/JingleSession.h>
#include <Swiften/FileTransfer/FileTransferTransporter.h>
#include <Swiften/Base/Log.h>

using namespace Swift;

JingleFileTransfer::JingleFileTransfer(
		boost::shared_ptr<JingleSession> session, 
		const JID& target,
		FileTransferTransporterFactory* transporterFactory) :
			session(session),
			target(target),
			transporterFactory(transporterFactory),
			transporter(NULL),
			ourCandidateSelectFinished(false), 
			theirCandidateSelectFinished(false) {

	session->addListener(this);

}

JingleFileTransfer::~JingleFileTransfer() {
	session->removeListener(this);
}

void JingleFileTransfer::fillCandidateMap(CandidateMap& map, const std::vector<JingleS5BTransportPayload::Candidate>& candidates) {
	map.clear();
	foreach (JingleS5BTransportPayload::Candidate candidate, candidates) {
		map[candidate.cid] = candidate;
	}
}

/*
std::string JingleFileTransfer::getS5BDstAddr(const JID& requester, const JID& target) const {
	return Hexify::hexify(crypto->getSHA1Hash(
				createSafeByteArray(s5bSessionID + requester.toString() + target.toString())));
}
*/

const JID& JingleFileTransfer::getInitiator() const {
	return session->getInitiator();
}

const JID& JingleFileTransfer::getResponder() const {
	return target;
}

FileTransfer::State::Type JingleFileTransfer::getExternalFinishedState(JinglePayload::Reason::Type reason) {
	if (reason == JinglePayload::Reason::Cancel || reason == JinglePayload::Reason::Decline) {
		return FileTransfer::State::Canceled;
	}
	else if (reason == JinglePayload::Reason::Success) {
		return FileTransfer::State::Finished;
	}
	else {
		return FileTransfer::State::Failed;
	}
}

boost::optional<FileTransferError> JingleFileTransfer::getFileTransferError(JinglePayload::Reason::Type reason) {
	if (reason == JinglePayload::Reason::Success) {
		return boost::optional<FileTransferError>();
	}
	else {
		return boost::optional<FileTransferError>(FileTransferError::UnknownError);
	}
}

void JingleFileTransfer::handleRemoteTransportCandidateSelectFinished(
		const std::string& s5bSessionID, const boost::optional<JingleS5BTransportPayload::Candidate>& candidate) {
	SWIFT_LOG(debug) << std::endl;

	ourCandidateChoice = candidate;
	ourCandidateSelectFinished = true;

	JingleS5BTransportPayload::ref s5bPayload = boost::make_shared<JingleS5BTransportPayload>();
	s5bPayload->setSessionID(s5bSessionID);
	if (candidate) {
		s5bPayload->setCandidateUsed(candidate->cid);
	}
	else {
		s5bPayload->setCandidateError(true);
	}
	candidateSelectRequestID = session->sendTransportInfo(getContentID(), s5bPayload);

	decideOnCandidates();
}

// decide on candidates according to http://xmpp.org/extensions/xep-0260.html#complete
void JingleFileTransfer::decideOnCandidates() {
	SWIFT_LOG(debug) << std::endl;
	if (!ourCandidateSelectFinished || !theirCandidateSelectFinished) {
		SWIFT_LOG(debug) << "Can't make a decision yet!" << std::endl;
		return;
	}
	if (!ourCandidateChoice && !theirCandidateChoice) {
		SWIFT_LOG(debug) << "No candidates succeeded." << std::endl;
		fallback();
	}
	else if (ourCandidateChoice && !theirCandidateChoice) {
		startTransferViaRemoteCandidate();
	}
	else if (theirCandidateChoice && !ourCandidateChoice) {
		startTransferViaLocalCandidate();
	}
	else {
		SWIFT_LOG(debug) << "Choosing between candidates " 
			<< ourCandidateChoice->cid << "(" << ourCandidateChoice->priority << ")" << " and " 
			<< theirCandidateChoice->cid << "(" << theirCandidateChoice->priority << ")" << std::endl;
		if (ourCandidateChoice->priority > theirCandidateChoice->priority) {
			startTransferViaRemoteCandidate();
		}
		else if (ourCandidateChoice->priority < theirCandidateChoice->priority) {
			startTransferViaLocalCandidate();
		}
		else {
			if (hasPriorityOnCandidateTie()) {
				startTransferViaRemoteCandidate();
			}
			else {
				startTransferViaLocalCandidate();
			}
		}
	}
}

void JingleFileTransfer::handleProxyActivateFinished(
		const std::string& s5bSessionID, ErrorPayload::ref error) {
	SWIFT_LOG(debug) << std::endl;
	if (!isWaitingForLocalProxyActivate()) { SWIFT_LOG(warning) << "Incorrect state" << std::endl; return; }

	if (error) {
		SWIFT_LOG(debug) << "Error activating proxy" << std::endl;
		JingleS5BTransportPayload::ref proxyError = boost::make_shared<JingleS5BTransportPayload>();
		proxyError->setSessionID(s5bSessionID);
		proxyError->setProxyError(true);
		session->sendTransportInfo(getContentID(), proxyError);
		fallback();
	} 
	else {
		JingleS5BTransportPayload::ref proxyActivate = boost::make_shared<JingleS5BTransportPayload>();
		proxyActivate->setSessionID(s5bSessionID);
		proxyActivate->setActivated(theirCandidateChoice->cid);
		session->sendTransportInfo(getContentID(), proxyActivate);
		startTransferring(createRemoteCandidateSession());
	}
}

void JingleFileTransfer::handleTransportInfoReceived(
		const JingleContentID& /* contentID */, JingleTransportPayload::ref transport) {
	SWIFT_LOG(debug) << std::endl;

	if (JingleS5BTransportPayload::ref s5bPayload = boost::dynamic_pointer_cast<JingleS5BTransportPayload>(transport)) {
		if (s5bPayload->hasCandidateError() || !s5bPayload->getCandidateUsed().empty()) {
			SWIFT_LOG(debug) << "Received candidate decision from peer" << std::endl;
			if (!isTryingCandidates()) { SWIFT_LOG(warning) << "Incorrect state" << std::endl; return; }

			theirCandidateSelectFinished = true;
			if (!s5bPayload->hasCandidateError()) {
				BOOST_AUTO(theirCandidate, localCandidates.find(s5bPayload->getCandidateUsed()));
				if (theirCandidate == localCandidates.end()) {
					SWIFT_LOG(warning) << "Got invalid candidate" << std::endl;
					terminate(JinglePayload::Reason::GeneralError);
					return;
				}
				theirCandidateChoice = theirCandidate->second;
			}
			decideOnCandidates();
		} 
		else if (!s5bPayload->getActivated().empty()) {
			SWIFT_LOG(debug) << "Received peer activate from peer" << std::endl;
			if (!isWaitingForPeerProxyActivate()) { SWIFT_LOG(warning) << "Incorrect state" << std::endl; return; }

			if (ourCandidateChoice->cid == s5bPayload->getActivated()) {
				startTransferring(createRemoteCandidateSession());
			} 
			else {
				SWIFT_LOG(warning) << "ourCandidateChoice doesn't match activated proxy candidate!" << std::endl;
				terminate(JinglePayload::Reason::GeneralError);
			}
		}
		else {
			SWIFT_LOG(debug) << "Ignoring unknown info" << std::endl;
		}
	}
	else {
		SWIFT_LOG(debug) << "Ignoring unknown info" << std::endl;
	}
}

void JingleFileTransfer::setTransporter(FileTransferTransporter* transporter) {
	this->transporter = transporter;
	localTransportCandidatesGeneratedConnection = transporter->onLocalCandidatesGenerated.connect(
		boost::bind(&JingleFileTransfer::handleLocalTransportCandidatesGenerated, this, _1, _2));
	remoteTransportCandidateSelectFinishedConnection = transporter->onRemoteCandidateSelectFinished.connect(
		boost::bind(&JingleFileTransfer::handleRemoteTransportCandidateSelectFinished, this, _1, _2));
	proxyActivatedConnection = transporter->onProxyActivated.connect(
		boost::bind(&JingleFileTransfer::handleProxyActivateFinished, this, _1, _2));
}
