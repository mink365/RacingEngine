#include "FeatureTestsApp.h"

using namespace re;

void android_main(struct android_app* state)
{
	// Make sure glue isn't stripped.
    app_dummy();

    Application* app = new FeatureTestsApp();
    app->run(state);
}