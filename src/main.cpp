#include <iostream>
#include <ncurses.h>
#include "util.hpp"
#include "simulator.hpp"

int main(int argc, char** argv)
{
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " [bin file]" << std::endl;
            return 1;
        }

        // ncurses setting
        initscr();
        nocbreak();
        echo();
        start_color();
        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        // bkgd(COLOR_PAIR(0));

        Simulator sim{argv[1]};
        sim.run();

        getch();  // do not work?
        endwin();
    } catch (const std::exception& e) {
        FAIL(e.what());
    } catch (...) {
        FAIL("Unknown exception");
    }

    return 0;
}
