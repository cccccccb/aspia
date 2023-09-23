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

#ifndef ROUTER_SHARED_KEY_POOL_H
#define ROUTER_SHARED_KEY_POOL_H

#include "base/optional.hpp"
#include "base/macros_magic.h"
#include "base/memory/local_memory.h"
#include "proto/router_common.pb.h"
#include "router/session.h"

#include <cstdint>
#include <memory>

namespace router {

class SharedKeyPool
{
public:
    class Delegate
    {
    public:
        virtual ~Delegate() = default;

        virtual void onPoolKeyUsed(Session::SessionId session_id, uint32_t key_id) = 0;
    };

    explicit SharedKeyPool(Delegate* delegate);
    ~SharedKeyPool();

    std::unique_ptr<SharedKeyPool> share();

    struct Credentials
    {
        Session::SessionId session_id;
        proto::RelayKey key;
    };

    void addKey(Session::SessionId session_id, const proto::RelayKey& key);
    tl::optional<Credentials> takeCredentials();
    void removeKeysForRelay(Session::SessionId session_id);
    void clear();
    size_t countForRelay(Session::SessionId session_id) const;
    size_t count() const;
    bool isEmpty() const;

private:
    class Impl;
    explicit SharedKeyPool(base::local_shared_ptr<Impl> impl);

    base::local_shared_ptr<Impl> impl_;
    const bool is_primary_;

    DISALLOW_COPY_AND_ASSIGN(SharedKeyPool);
};

} // namespace router

#endif // ROUTER_SHARED_KEY_POOL_H
