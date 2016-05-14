#ifndef NQSOK_MOTION_HH
#define NQSOK_MOTION_HH

namespace nq {
    class Motion final {
    public:
        // Robert Penner's Easing.
        // Refs: AHEasing, DWTFYWTPLv2.
        static float bounce_out(float);
        static float elastic_out(float);
        static float quint_inout(float);
    };
}

#endif
