//
// Aspia Project
// Copyright (C) 2016-2023 Dmitry Chapyshev <dmitry@aspia.ru>
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

#ifndef CONSOLE_COMPUTER_DIALOG_H
#define CONSOLE_COMPUTER_DIALOG_H

#include "base/optional.hpp"
#include "console/settings.h"
#include "proto/address_book.pb.h"
#include "ui_computer_dialog.h"


#include <QDialog>

class QAbstractButton;

namespace console {

class ComputerDialog : public QDialog
{
    Q_OBJECT

public:
    enum class Mode { CREATE, COPY, MODIFY };

    ComputerDialog(QWidget* parent,
                   Mode mode,
                   const QString& parent_name,
                   const tl::optional<proto::address_book::Computer>& computer = tl::nullopt);
    ~ComputerDialog() override;

    const proto::address_book::Computer& computer() const { return computer_; }

protected:
    // QDialog implementation.
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onTabChanged(QTreeWidgetItem* current);
    void buttonBoxClicked(QAbstractButton* button);

private:
    void showTab(int type);
    bool saveChanges();

    Ui::ComputerDialog ui;
    QWidgetList tabs_;
    const Mode mode_;

    Settings settings_;

    proto::address_book::Computer computer_;

    DISALLOW_COPY_AND_ASSIGN(ComputerDialog);
};

} // namespace console

#endif // CONSOLE_COMPUTER_DIALOG_H
