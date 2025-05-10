#include <GlobalDefines.hpp>
#include "source/app.hpp"

extern "C" int app_main(void)
{
	static string tag = "MAIN";
	unique_ptr<App> app = make_unique<App>();
	return 0;
}