/* modules */
#include <iostream>
#include <chrono>
#include <thread>
/* files */
#include "utils/conio.hpp"
#include "objects.cpp"
#include "gmeng.hpp"
#include "utils/textures.cpp"

/* index */
// this file is currently used for testing purposes
int main(int argc, char** argv) {
	Gmeng::Renderer<10, 10> renderer;
	Gmeng::Unit test[100] = {
		g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), 
		g_unit(1), g_spike(4, 0, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(7, true), g_unit(1), 
		g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), g_unit(1), 
	};
	renderer.setUnits(test);
	Gmeng::WorldMap<10, 10> map = Gmeng::UseRenderer(renderer);
	Objects::G_Player player = {.c_ent_tag="o",.colored=true,.colorId=2,.entityId=0,.textured=false,.textureId=0};
	map.SetPlayer(0, player, 2, 2);

	for ( ;; ) {
		map.update();
		std::cout << "\033[2J\033[1;1H";
		std::cout << map.draw() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		int keypress = getch();
		if (keypress == 119) { // W
			Objects::coord coords = map.player.coords;
			map.MovePlayer(0, coords.x, coords.y-1);
		}
		if (keypress == 115) { // S
			Objects::coord coords = map.player.coords;
			map.MovePlayer(0, coords.x, coords.y+1);
		};
		if (keypress == 97) { // A
			Objects::coord coords = map.player.coords;
			map.MovePlayer(0, coords.x-1, coords.y);
		};
		if (keypress == 100) { // D
			Objects::coord coords = map.player.coords;
			map.MovePlayer(0, coords.x+1, coords.y);
		};
	};
	return 0;
};