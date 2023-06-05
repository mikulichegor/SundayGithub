#include "ui/text/text_utilities.h"
#include "ayu/ayu_settings.h"
#include "ayu_profile_values.h"
#include "data/data_peer.h"


constexpr auto kMaxChannelId = -1000000000000;


QString IDString(not_null<PeerData*> peer) {
    auto resultId = QString::number(peerIsUser(peer->id)
                                    ? peerToUser(peer->id).bare
                                    : peerIsChat(peer->id)
                                      ? peerToChat(peer->id).bare
                                      : peerIsChannel(peer->id)
                                        ? peerToChannel(peer->id).bare
                                        : peer->id.value);

    auto const settings = &AyuSettings::getInstance();
    if (settings->showPeerId == 2) {
        if (peer->isChannel()) {
            resultId = QString::number(peerToChannel(peer->id).bare - kMaxChannelId).prepend("-");
        } else if (peer->isChat()) {
            resultId = resultId.prepend("-");
        }
    }

    return resultId;
}

rpl::producer<TextWithEntities> IDValue(not_null<PeerData*> peer) {
    return rpl::single(IDString(peer)) | Ui::Text::ToWithEntities();
}