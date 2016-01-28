#ifndef NQSOK_CONTEXT_HH
#define NQSOK_CONTEXT_HH

#include <stdexcept>

namespace nq {
    class Context_error : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    struct Context {
        int major_version;
        int minor_version;
        bool core_profile;
        bool forward_compatible;
    };
}

#endif
