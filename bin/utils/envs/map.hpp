#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#ifdef __GMENG_INIT__
#ifndef __GMENG_MAP_INIT__

#define stob(str) (str == std::string("true") || str.substr(1) == std::string("true"))
#define cpps(str) ( std::string(str) )
using namespace std;

static bool endsWith(const std::string& str, const char* suffix, unsigned suffixLen)
{
    return str.size() >= suffixLen && 0 == str.compare(str.size()-suffixLen, suffixLen, suffix, suffixLen);
};

static bool endsWith(const std::string& str, const char* suffix)
{
    return endsWith(str, suffix, std::string::traits_type::length(suffix));
};

static bool startsWith(const std::string& str, const char* prefix, unsigned prefixLen)
{
    return str.size() >= prefixLen && 0 == str.compare(0, prefixLen, prefix, prefixLen);
};

static bool startsWith(const std::string& str, const char* prefix)
{
    return startsWith(str, prefix, std::string::traits_type::length(prefix));
};

std::string g_readFile(const string &fileName)
{
    ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    return std::string(bytes.data(), fileSize);
};

void g_rmChar( string &str, char* charsToRemove ) {
   for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
      str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
   }
}

std::string g_joinStr(std::vector<std::string> v, std::string delimiter) {
	std::string result;
	for ( auto i : v) { result += i + delimiter; };
	return result;
};

std::vector<std::string> g_splitStr(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


namespace Gmeng {
	namespace MapParser {
		namespace syntax {
			struct line {
				int target; int type; std::string base_line;
			};
			struct obj : line { int target; int type; std::string base_line; Gmeng::Unit unit; };
			struct pscript : line {	int target; int type; std::string base_line; Objects::G_Player player; };
			// parse string from syntax.
			// MapParser strings use [] as delimiters.
			std::string pstr(std::string __s) {
				std::string s = __s;
				if (!startsWith(__s, "[") && !startsWith(__s, " [")) throw std::invalid_argument("string does not start with '['.");
				if (!endsWith(__s, "]") && !endsWith(__s, "] ")) throw std::invalid_argument("string does not start with '['.");
				s = s.substr((startsWith(__s, " [") ? 2 : 1), s.length()); // remove first '[' char
				s.pop_back(); // remove last ']' char
				if (endsWith(__s, "] ")) s.pop_back(); // remove last ']' char if the base str ends with ' '
				return s;
			};
		};

		inline Gmeng::Unit ParseUnit(std::string __s) {
			Gmeng::Unit unit;
			std::string params = __s.substr(4, __s.size()); // 4 = 'obj('
			g_rmChar(params, ")"); // remove trailing ')'
			std::vector<std::string> v = g_splitStr(params, ",");
			for (int i = 0; i < v.size(); i++) {
				std::string u ( v[i] );
				if ( i == 0 ) { unit.color = std::atoi(u.c_str()); continue; }; // UNIT COLOR
				if ( i == 1 ) { unit.collidable = stob(u); continue; };  // UNIT COLLIDABLE?
				if ( i == 2 ) { unit.is_entity = stob(u); continue; };    // UNIT ENTITY STATE
				if ( i == 3 ) { unit.is_player = stob(u); continue; };    // UNIT PLAYER STATE
				if ( i == 4 ) { unit.special = stob(u); continue; };      // UNITT SPECIAL?
				if ( i == 5 ) { unit.special_clr = std::atoi(u.c_str()); continue; }; // UNIT SPECIAL COLOR (BACKGROUND)
				if ( i == 6 ) { unit.special_c_unit = syntax::pstr(u); continue; }; // UNIT SPECIAL CHARACTER
			};
			return unit;
		};

		inline int ParseTargetId(std::string __s) {
			std::vector<std::string> v = g_splitStr(__s, " ");
			return std::atoi(v[0].c_str());
		};

		inline int ParseMethodType(std::string __s) {
			int __t = 0;
			if (__s == "OS") __t = 1;
			else if (__s == "PS") __t = 2;
			else if (__s == "US") __t = 3;
			return __t;
		};

		inline Objects::G_Player ParsePlayerScript(std::string __s) {
			Objects::G_Player player;
			std::string params = __s.substr(8, __s.size()); // 8 = 'pscript('
			g_rmChar(params, ")"); // remove trailing ')'
			std::vector<std::string> v = g_splitStr(params, ",");
			for ( int i = 0; i < v.size(); i++ ) {
				std::string u ( v[i] );
				/// TODO: switch
				if ( i == 0 ) { player.entityId = std::atoi(u.c_str()); continue; };
				if ( i == 1 ) { player.textureId = std::atoi(u.c_str()); continue; };
				if ( i == 2 ) { player.colorId = std::atoi(u.c_str()); continue; };
				if ( i == 3 ) { player.textured = stob(u); continue; };
				if ( i == 4 ) { player.colored = stob(u); continue; };
				if ( i == 5 ) { player.c_ent_tag = syntax::pstr(u); continue; };
			};
			return player;
		};

		inline syntax::line ParseBaseLine(std::string __s) {
			std::vector<std::string> v = g_splitStr(__s, " ");
			int target = std::atoi(v[0].c_str()); int type = ParseMethodType(v[1]);
			v.erase(v.begin()); v.erase(v.begin());
			std::string line = g_joinStr(v, " ");
			return syntax::line{ .base_line=line,.target=target,.type=type };
		};

		inline syntax::pscript ParsePScript(syntax::line __l) {};

		inline syntax::obj ParseOScript(syntax::line __l) {
			Gmeng::Unit unit = Gmeng::MapParser::ParseUnit(__l.base_line);
			return syntax::obj{.target=__l.target,.type=__l.type,.unit=unit};
		};
	};
	namespace Environments {
		template<std::size_t x, std::size_t y>
		inline void WriteMap(Gmeng::WorldMap<x, y> r, std::string fname) {
			std::fstream mapFile(fname, std::ios::app);
			if (!mapFile.is_open()) throw std::invalid_argument(fname + " is not a valid mapfile.");
			std::string content(g_readFile(fname));
			Gmeng::Unit unit = Gmeng::MapParser::ParseUnit(content);
		};
		template<std::size_t x, std::size_t y>
		inline Gmeng::WorldMap<x, y> ReadMap(std::string fname) {
			std::fstream mapFile(fname, std::ios::app);
			if (!mapFile.is_open()) throw std::invalid_argument(fname + " is not a valid mapfile.");
			std::string content(g_readFile(fname));
			Gmeng::Renderer<x, y> renderer;
			Gmeng::Unit unitmap[x*y] = {};
			Gmeng::MapParser::syntax::pscript player_loc = Gmeng::MapParser::syntax::pscript{.target=-1};
			std::vector<std::string> v = g_splitStr(content, "\n");
			for ( int i = 0; i < v.size(); i++ ) {
				std::string u ( v[i] );
				Gmeng::MapParser::syntax::line ln = Gmeng::MapParser::ParseBaseLine(u);
				if (ln.type == 1) { // OBJECT [obj() method]
					Gmeng::MapParser::syntax::obj obj = Gmeng::MapParser::ParseOScript(ln);
					unitmap[ln.target] = obj.unit;
				}
				else if (ln.type == 2) { // PLAYER [pscript() method]
					Objects::G_Player player = Gmeng::MapParser::ParsePlayerScript(ln.base_line);
					player_loc = {.player=player,.target=ln.target,.type=ln.type,.base_line=ln.base_line};
				};
			};
			renderer.setUnits(unitmap);
			Gmeng::WorldMap<x,y> map = Gmeng::UseRenderer<x,y>(renderer);
			
			if (player_loc.target != -1) map.SetPlayer(player_loc.player.entityId, player_loc.player, player_loc.target);
			return map;
		};
	};
};

#define __GMENG_MAP_INIT__ true
#endif
#endif
