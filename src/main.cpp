// internal
#include "common.hpp"
#include "world.hpp"

#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>
#include <iostream>
#include <algorithm>

using Clock = std::chrono::high_resolution_clock;

// Global
World world;
const int width = 1200;
const int height = 800;
const char* title = "Stealthy Sam";

// Entry point
int main(int argc, char* argv[])
{
	// Initializing world (after renderer.init().. sorry)
	if (!world.init({ (float)width, (float)height }))
	{
		// Time to read the error message
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
		return EXIT_FAILURE;
	}

	auto currentTime = Clock::now();
	const float dt = 16.667; // around 1/60th of a second, in milliseconds

	while (!world.is_over())
	{
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto newTime = Clock::now();
		float frameTime = (float) (std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime)).count() / 1000;
		currentTime = newTime;

		while (frameTime > 0.f) {
			float deltaTime = std::min(frameTime, dt);
			world.update(deltaTime);
			frameTime -= deltaTime;
		}

		world.draw();
	}

	world.destroy();

	return EXIT_SUCCESS;
}
