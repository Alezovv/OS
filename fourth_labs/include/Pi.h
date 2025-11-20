#ifndef PI_H
#define PI_H

#ifdef _WIN32
#ifdef BUILD_DLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
#else
#define EXPORT __attribute__((visibility("default")))
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

EXPORT double calculate_pi_leibniz(int iterations);
EXPORT double calculate_pi_wallis(int iterations);

#endif