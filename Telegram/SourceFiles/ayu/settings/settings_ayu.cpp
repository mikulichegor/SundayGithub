#include <ui/boxes/single_choice_box.h>
#include "ayu/boxes/edit_edited_mark.h"
#include "ayu/boxes/edit_deleted_mark.h"
#include "ayu/ayu_settings.h"
#include "ayu/settings/settings_ayu.h"

#include "lang_auto.h"
#include "mainwindow.h"
#include "settings/settings_common.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/checkbox.h"
#include "boxes/connection_box.h"
#include "ui/boxes/confirm_box.h"
#include "platform/platform_specific.h"
#include "window/window_session_controller.h"
#include "lang/lang_instance.h"
#include "core/application.h"
#include "storage/localstorage.h"
#include "data/data_session.h"
#include "main/main_session.h"
#include "styles/style_settings.h"
#include "apiwrap.h"
#include "api/api_blocked_peers.h"

namespace Settings {

    rpl::producer<QString> Ayu::title() {
        return tr::ayu_AyuPreferences();
    }

    Ayu::Ayu(
            QWidget *parent,
            not_null<Window::SessionController *> controller)
            : Section(parent) {
        setupContent(controller);
    }

    void Ayu::SetupGhostEssentials(not_null<Ui::VerticalLayout *> container) {
        auto settings = &AyuSettings::getInstance();

        AddSubsectionTitle(container, tr::ayu_GhostEssentialsHeader());

        AddButton(
                container,
                tr::ayu_SendReadPackets(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->sendReadPackets)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->sendReadPackets);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_sendReadPackets(enabled);
            AyuSettings::save();
        }, container->lifetime());

        AddButton(
                container,
                tr::ayu_SendOnlinePackets(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->sendOnlinePackets)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->sendOnlinePackets);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_sendOnlinePackets(enabled);
            AyuSettings::save();
        }, container->lifetime());

        AddButton(
                container,
                tr::ayu_SendUploadProgress(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->sendUploadProgress)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->sendUploadProgress);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_sendUploadProgress(enabled);
            AyuSettings::save();
        }, container->lifetime());

        AddButton(
                container,
                tr::ayu_SendOfflinePacketAfterOnline(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->sendOfflinePacketAfterOnline)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->sendOfflinePacketAfterOnline);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_sendOfflinePacketAfterOnline(enabled);
            AyuSettings::save();
        }, container->lifetime());

        AddButton(
                container,
                tr::ayu_MarkReadAfterSend(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->markReadAfterSend)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->markReadAfterSend);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_markReadAfterSend(enabled);
            AyuSettings::save();
        }, container->lifetime());

        AddButton(
                container,
                tr::ayu_UseScheduledMessages(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->useScheduledMessages)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->useScheduledMessages);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_useScheduledMessages(enabled);
            AyuSettings::save();
        }, container->lifetime());
    }

    void Ayu::SetupSpyEssentials(not_null<Ui::VerticalLayout *> container) {
        auto settings = &AyuSettings::getInstance();

        AddSubsectionTitle(container, tr::ayu_SpyEssentialsHeader());

        AddButton(
                container,
                tr::ayu_KeepDeletedMessages(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->keepDeletedMessages)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->keepDeletedMessages);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_keepDeletedMessages(enabled);
            AyuSettings::save();
        }, container->lifetime());

        AddButton(
                container,
                tr::ayu_KeepMessagesHistory(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->keepMessagesHistory)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->keepMessagesHistory);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_keepMessagesHistory(enabled);
            AyuSettings::save();
        }, container->lifetime());
    }

    void Ayu::SetupCustomization(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
        auto settings = &AyuSettings::getInstance();

        AddSubsectionTitle(container, tr::ayu_CustomizationHeader());

        auto btn = AddButtonWithLabel(
                container,
                tr::ayu_DeletedMarkText(),
                AyuSettings::get_deletedMarkReactive(),
                st::settingsButtonNoIcon
        );
        btn->addClickHandler([=]() {
            auto box = Box<EditDeletedMarkBox>();
            Ui::show(std::move(box));
        });

        auto btn2 = AddButtonWithLabel(
                container,
                tr::ayu_EditedMarkText(),
                AyuSettings::get_editedMarkReactive(),
                st::settingsButtonNoIcon
        );
        btn2->addClickHandler([=]() {
            auto box = Box<EditEditedMarkBox>();
            Ui::show(std::move(box));
        });

        AddButton(
                container,
                tr::ayu_ShowGhostToggleInDrawer(),
                st::settingsButtonNoIcon
        )->toggleOn(
                rpl::single(settings->showGhostToggleInDrawer)
        )->toggledValue(
        ) | rpl::filter([=](bool enabled) {
            return (enabled != settings->showGhostToggleInDrawer);
        }) | rpl::start_with_next([=](bool enabled) {
            settings->set_showGhostToggleInDrawer(enabled);
            AyuSettings::save();
        }, container->lifetime());

        SetupShowPeerId(container, controller);
    }

    void Ayu::SetupShowPeerId(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
        auto settings = &AyuSettings::getInstance();

        const auto options = std::vector{
                QString(tr::ayu_SettingsShowID_Hide(tr::now)),
                QString("Telegram API"),
                QString("Bot API")
        };

        auto currentVal = AyuSettings::get_showPeerId() | rpl::map([=] (int val) {
            return options[val];
        });

        const auto button = AddButtonWithLabel(
                container,
                tr::ayu_SettingsShowID(),
                currentVal,
                st::settingsButtonNoIcon);
        button->addClickHandler([=] {
            controller->show(Box([=](not_null<Ui::GenericBox*> box) {
                const auto save = [=](int index) {
                    settings->set_showPeerId(index);
                    AyuSettings::save();
                };
                SingleChoiceBox(box, {
                        .title = tr::ayu_SettingsShowID(),
                        .options = options,
                        .initialSelection = settings->showPeerId,
                        .callback = save,
                });
            }));
        });
    }

    void Ayu::SetupAyuGramSettings(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller) {
        AddSkip(container);
        SetupGhostEssentials(container);

        AddDivider(container);

        AddSkip(container);
        SetupSpyEssentials(container);

        AddDivider(container);

        AddSkip(container);
        SetupCustomization(container, controller);

        AddDividerText(container, tr::ayu_SettingsWatermark());
    }

    void Ayu::setupContent(not_null<Window::SessionController *> controller) {
        const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

        SetupAyuGramSettings(content, controller);

        Ui::ResizeFitChild(this, content);
    }
} // namespace Settings

