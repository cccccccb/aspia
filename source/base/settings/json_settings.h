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

#ifndef BASE_SETTINGS_JSON_SETTINGS_H
#define BASE_SETTINGS_JSON_SETTINGS_H

#include "base/macros_magic.h"
#include "base/settings/settings.h"
#include "base/filesystem.hpp"


namespace base {

class JsonSettings : public Settings
{
public:
    enum class Scope { USER, SYSTEM };
    enum class Encrypted { YES, NO };

    JsonSettings(std::string file_name,
                 Encrypted encrypted = Encrypted::NO);
    JsonSettings(Scope scope,
                 std::string application_name,
                 std::string file_name,
                 Encrypted encrypted = Encrypted::NO);
    ~JsonSettings() override;

    bool isWritable() const;
    void sync();
    bool flush();

    const ghc::filesystem::path& filePath() const { return path_; }

    static ghc::filesystem::path filePath(std::string file_name);
    static ghc::filesystem::path filePath(Scope scope,
                                          std::string application_name,
                                          std::string file_name);

    static bool readFile(const ghc::filesystem::path& file,
                         Map& map,
                         Encrypted encrypted = Encrypted::NO);
    static bool writeFile(const ghc::filesystem::path& file,
                          const Map& map,
                          Encrypted encrypted = Encrypted::NO);

private:
    const Encrypted encrypted_;
    ghc::filesystem::path path_;

    DISALLOW_COPY_AND_ASSIGN(JsonSettings);
};

} // namespace base

#endif // BASE_SETTINGS_JSON_SETTINGS_H
