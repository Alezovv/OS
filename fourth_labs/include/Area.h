#ifndef AREA_H
#define AREA_H

#ifdef _WIN32
#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#define RECTANGLE 1
#define TRIANGLE 2

EXPORT float area(float a, float b, int figure_type);
EXPORT const char *get_figure_name(int figure_type);

#endif