#pragma once

// Keep Tracy behind a project-owned interface so profiling can be compiled out
// without scattering conditional compilation throughout the engine.
#ifdef ENABLE_PROFILING
#include <tracy/Tracy.hpp>

#define PROFILE_ZONE() ZoneScoped
#define PROFILE_ZONE_NAMED(name) ZoneScopedN(name)
#define PROFILE_FRAME() FrameMark
#define PROFILE_PLOT(name, value) TracyPlot(name, value)
#define PROFILE_APP_INFO(text, size) TracyAppInfo(text, size)
#else
#define PROFILE_ZONE() ((void)0)
#define PROFILE_ZONE_NAMED(name) ((void)0)
#define PROFILE_FRAME() ((void)0)
#define PROFILE_PLOT(name, value) ((void)0)
#define PROFILE_APP_INFO(text, size) ((void)0)
#endif
