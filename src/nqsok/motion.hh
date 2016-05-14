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
        static float bounce_in(float);
        static float bounce_out(float);
        static float elastic_in(float);
        static float elastic_out(float);
        static float quint_inout(float);

        using Motionf = float(*)(float);
        struct Point final {
            float time {0.0};
            float value {0.0};
        };

        float value(float) const;
        void apply(const Point& current,
                   const Point& next,
                   Motionf mfunction) {
            if (current.time >= next.time)
                throw Motion_error{"wat"};
            past.value = current.value;
            future.value = next.value;
            past.time = current.time;
            future.time = next.time;
            function = mfunction;
        }

        Motion(const Point& current, Motionf f)
              : function{f}, past{current},
                             future{current} {}

    private:
        Motionf function;
        Point past, future;
    };
}

#endif
