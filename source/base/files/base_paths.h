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

#ifndef BASE_FILES_BASE_PATHS_H
#define BASE_FILES_BASE_PATHS_H

#include "base/macros_magic.h"
#include "base/filesystem.hpp"

#include "build/build_config.h"


namespace base {

class BasePaths
{
public:
#if defined(OS_WIN)
    // Windows directory, usually "c:\windows".
    static bool windowsDir(ghc::filesystem::path* result);

    // Usually c:\windows\system32".
    static bool systemDir(ghc::filesystem::path* result);
#endif // defined(OS_WIN)

    // Application Data directory under the user profile.
    static bool userAppData(ghc::filesystem::path* result);

    // The current user's Desktop.
    static bool userDesktop(ghc::filesystem::path* result);

    // The current user's Home.
    static bool userHome(ghc::filesystem::path* result);

    // Usually "C:\ProgramData".
    static bool commonAppData(ghc::filesystem::path* result);

    // Directory for the common desktop (visible on all user's Desktop).
    static bool commonDesktop(ghc::filesystem::path* result);

    // Directory of the current executable.
    static bool currentExecDir(ghc::filesystem::path* result);

    // The path to the current executable file.
    static bool currentExecFile(ghc::filesystem::path* result);

private:
    DISALLOW_COPY_AND_ASSIGN(BasePaths);
};

} // namespace base

#endif // BASE_FILES_BASE_PATHS_H
