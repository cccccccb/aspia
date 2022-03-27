//
// Aspia Project
// Copyright (C) 2016-2022 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef CONSOLE_COMPUTER_GROUP_DIALOG_DESKTOP_H
#define CONSOLE_COMPUTER_GROUP_DIALOG_DESKTOP_H

#include "base/macros_magic.h"
#include "console/computer_group_dialog_tab.h"
#include "proto/common.pb.h"
#include "proto/desktop.pb.h"
#include "ui_computer_group_dialog_desktop.h"

namespace console {

class ComputerGroupDialogDesktop : public ComputerGroupDialogTab
{
    Q_OBJECT

public:
    ComputerGroupDialogDesktop(int type, QWidget* parent);
    ~ComputerGroupDialogDesktop() override = default;

    void restoreSettings(proto::SessionType session_type, const proto::DesktopConfig& config);
    void saveSettings(proto::DesktopConfig* config);

private slots:
    void onCodecChanged(int item_index);
    void onCompressionRatioChanged(int value);

private:
    Ui::ComputerGroupDialogDesktop ui;

    DISALLOW_COPY_AND_ASSIGN(ComputerGroupDialogDesktop);
};

} // namespace console

#endif // CONSOLE_COMPUTER_GROUP_DIALOG_DESKTOP_H