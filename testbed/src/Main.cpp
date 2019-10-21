/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2016 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
********************************************************************************/

// Libraries
#include "TestbedApplication.h"
#include "nanogui/nanogui.h"

using namespace nanogui;

// Main function
int main(int argc, char** argv) {

    nanogui::init();

    {
        bool isFullscreen = false;

		// Get the primary monitor
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		// Window size
		int windowWidth = mode->width;
		int windowHeight = mode->height;

		if (!isFullscreen) {

			windowWidth *= 0.9;
			windowHeight *= 0.9;
		}

        // Create and start the testbed application
        nanogui::ref<TestbedApplication> application = new TestbedApplication(isFullscreen, windowWidth, windowHeight);
        application->setVisible(true);

        nanogui::mainloop();
    }

    nanogui::shutdown();

    return 0;
}
