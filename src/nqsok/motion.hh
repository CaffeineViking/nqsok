#ifndef NQSOK_MOTION_HH
#define NQSOK_MOTION_HH

#include <stdexcept>

namespace nq {
    class Motion_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Motion final {
    public:
        // Robert Penner's Easing.
        // Refs: AHEasing, DWTFYWTPLv2.
        static float bounce_out(float);
        static float elastic_out(float);
        static float quint_inout(float);

        using Motionf = float(*)(float);
        struct Point final {
            float time {0.0};
            float value {0.0};
        };

        bool done(float) const; float value(float) const;
        Motion(const Point& current, const Point& future, Motionf f)
              : function{f}, past{current}, future{future} {}

    private:
        Motionf function;
        Point past, future;
    };
}

#endif
