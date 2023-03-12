/* modules */
#include <iostream>
#include <chrono>
#include <thread>
/* files */
#include "utils/conio.hpp"
#include "objects.cpp"
#include "gmeng.hpp"
#include "utils/textures.cpp"
#include "utils/envs/map.hpp"
/* index */
// this file is currently used for testing purposes
using std::endl;

int main(int argc, char** argv) {
	Gmeng::WorldMap<3, 3> map = Gmeng::Environments::ReadMap<3, 3>("test.txt");
	std::cout << map.draw() << endl;
};


int main3(int argc, char** argv) {
	Gmeng::MapParser::syntax::line line = Gmeng::MapParser::ParseBaseLine("23 PS pscript(5, 7, 200, false, true, [hello])");
	Objects::G_Player player = Gmeng::MapParser::ParsePlayerScript(line.base_line);
	std::cout << player.entityId << " " << player.textureId << " " << player.colorId << " " << player.textured << " " << player.colored << " " << player.c_ent_tag << endl;
	Gmeng::MapParser::syntax::line line2 = Gmeng::MapParser::ParseBaseLine("10 OS obj(77, false, false, false, true, 9923, X)");
	Gmeng::MapParser::syntax::obj obj = Gmeng::MapParser::ParseOScript(line2);
	Gmeng::Unit unit = obj.unit;
	std::cout << unit.color << " " << unit.collidable << " " << unit.is_entity << " " << unit.is_player << " " << unit.special << " " << unit.special_clr << " " << unit.special_c_unit << endl;
	return 0;
};

int main2(int argc, char** argv) {
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
		if (keypress == 9) { // TAB
			std::cout << "exit game." << std::endl;
			break; 
		};
		if (keypress == 119) { // W
			Objects::coord coords = map.player.coords;
			if (map.display_map.unitmap[((coords.y-1)*map.w)+coords.x].special_c_unit == "X") { std::cout << "game over" << std::endl; break;}
			map.MovePlayer(0, coords.x, coords.y-1);
		};
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
