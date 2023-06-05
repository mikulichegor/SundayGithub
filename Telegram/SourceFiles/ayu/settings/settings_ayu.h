#pragma once

#include "settings/settings_common.h"

class BoxContent;

namespace Window {
    class Controller;

    class SessionController;
} // namespace Window

namespace Settings {
    class Ayu : public Section<Ayu> {
    public:
        Ayu(QWidget *parent, not_null<Window::SessionController *> controller);

        [[nodiscard]] rpl::producer<QString> title() override;

    private:
        void SetupGhostEssentials(not_null<Ui::VerticalLayout *> container);
        void SetupSpyEssentials(not_null<Ui::VerticalLayout *> container);
        void SetupCustomization(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller);
        void SetupShowPeerId(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> controller);

        void SetupAyuGramSettings(not_null<Ui::VerticalLayout *> container, not_null<Window::SessionController *> null);
        void setupContent(not_null<Window::SessionController *> controller);
    };

} // namespace Settings
