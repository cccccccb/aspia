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

#include "client/input_event_filter.h"

#include "base/optional.hpp"
#include "base/logging.h"

#include <QDebug>

namespace client {

//--------------------------------------------------------------------------------------------------
InputEventFilter::InputEventFilter() = default;

//--------------------------------------------------------------------------------------------------
InputEventFilter::~InputEventFilter() = default;

//--------------------------------------------------------------------------------------------------
void InputEventFilter::setSessionType(proto::SessionType session_type)
{
    qInfo() << "Session type changed: " << session_type;
    session_type_ = session_type;
}

//--------------------------------------------------------------------------------------------------
void InputEventFilter::setClipboardEnabled(bool enable)
{
    qInfo() << "Clipboard enabled: " << enable;
    clipboard_enabled_ = enable;
}

//--------------------------------------------------------------------------------------------------
void InputEventFilter::setNetworkOverflow(bool enable)
{
    network_overflow_ = enable;
}

//--------------------------------------------------------------------------------------------------
tl::optional<proto::MouseEvent> InputEventFilter::mouseEvent(const proto::MouseEvent& event)
{
    if (session_type_ != proto::SESSION_TYPE_DESKTOP_MANAGE)
        return tl::nullopt;

    if (network_overflow_)
        return tl::nullopt;

    int32_t delta_x = std::abs(event.x() - last_pos_x_);
    int32_t delta_y = std::abs(event.y() - last_pos_y_);

    if (delta_x > 1 || delta_y > 1 || event.mask() != last_mask_)
    {
        static const uint32_t kWheelMask =
            proto::MouseEvent::WHEEL_DOWN | proto::MouseEvent::WHEEL_UP;

        last_pos_x_ = event.x();
        last_pos_y_ = event.y();
        last_mask_ = event.mask() & ~kWheelMask;

        ++send_mouse_count_;
        return event;
    }
    else
    {
        ++drop_mouse_count_;
    }

    return tl::nullopt;
}

//--------------------------------------------------------------------------------------------------
tl::optional<proto::KeyEvent> InputEventFilter::keyEvent(const proto::KeyEvent& event)
{
    if (session_type_ != proto::SESSION_TYPE_DESKTOP_MANAGE)
        return tl::nullopt;

    if (network_overflow_)
        return tl::nullopt;

    ++send_key_count_;
    return event;
}

//--------------------------------------------------------------------------------------------------
tl::optional<proto::TextEvent> InputEventFilter::textEvent(const proto::TextEvent& event)
{
    if (session_type_ != proto::SESSION_TYPE_DESKTOP_MANAGE)
        return tl::nullopt;

    if (network_overflow_)
        return tl::nullopt;

    ++send_text_count_;
    return event;
}

//--------------------------------------------------------------------------------------------------
tl::optional<proto::ClipboardEvent> InputEventFilter::readClipboardEvent(
    const proto::ClipboardEvent& event)
{
    if (session_type_ != proto::SESSION_TYPE_DESKTOP_MANAGE)
        return tl::nullopt;

    if (!clipboard_enabled_)
        return tl::nullopt;

    ++read_clipboard_count_;
    return event;
}

//--------------------------------------------------------------------------------------------------
tl::optional<proto::ClipboardEvent> InputEventFilter::sendClipboardEvent(
    const proto::ClipboardEvent& event)
{
    if (session_type_ != proto::SESSION_TYPE_DESKTOP_MANAGE)
        return tl::nullopt;

    if (network_overflow_)
        return tl::nullopt;

    if (!clipboard_enabled_)
        return tl::nullopt;

    ++send_clipboard_count_;
    return event;
}

} // namespace client
