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

#include "router/win/service_util.h"

#include "base/filesystem.hpp"
#include "base/files/base_paths.h"
#include "base/win/service_controller.h"
#include "router/service_constants.h"

#include <iostream>

namespace router {

//--------------------------------------------------------------------------------------------------
void startService()
{
    base::win::ServiceController controller =
        base::win::ServiceController::open(router::kServiceName);
    if (!controller.isValid())
    {
        std::cout << "Failed to access the service. Not enough rights or service not installed."
            << std::endl;
    }
    else
    {
        if (!controller.start())
        {
            std::cout << "Failed to start the service." << std::endl;
        }
        else
        {
            std::cout << "The service started successfully." << std::endl;
        }
    }
}

//--------------------------------------------------------------------------------------------------
void stopService()
{
    base::win::ServiceController controller =
        base::win::ServiceController::open(router::kServiceName);
    if (!controller.isValid())
    {
        std::cout << "Failed to access the service. Not enough rights or service not installed."
            << std::endl;
    }
    else
    {
        if (!controller.stop())
        {
            std::cout << "Failed to stop the service." << std::endl;
        }
        else
        {
            std::cout << "The service has stopped successfully." << std::endl;
        }
    }
}

//--------------------------------------------------------------------------------------------------
void installService()
{
    ghc::filesystem::path file_path;

    if (!base::BasePaths::currentExecFile(&file_path))
    {
        std::cout << "Failed to get the path to the executable." << std::endl;
    }
    else
    {
        base::win::ServiceController controller = base::win::ServiceController::install(
            router::kServiceName, router::kServiceDisplayName, file_path);
        if (!controller.isValid())
        {
            std::cout << "Failed to install the service." << std::endl;
        }
        else
        {
            controller.setDescription(router::kServiceDescription);
            std::cout << "The service has been successfully installed." << std::endl;
        }
    }
}

//--------------------------------------------------------------------------------------------------
void removeService()
{
    if (base::win::ServiceController::isRunning(router::kServiceName))
    {
        stopService();
    }

    if (!base::win::ServiceController::remove(router::kServiceName))
    {
        std::cout << "Failed to remove the service." << std::endl;
    }
    else
    {
        std::cout << "The service was successfully deleted." << std::endl;
    }
}

} // namespace router
