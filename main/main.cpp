#include <GlobalDefines.hpp>
#include "source/app.hpp"

static string tag = "MAIN";


extern "C" int app_main(void)
{
	unique_ptr<App> app = make_unique<App>();
	return 0;
}