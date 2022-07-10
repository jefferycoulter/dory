#ifndef DORY_NO_COPY_INCL
#define DORY_NO_COPY_INCL

namespace DORY
{
    /**
     * @brief base class from which non-copyable classes and structs inherit. just for reducing code so
     * that other more significant implementation details can be seen in the inheriting class or struct
     */
    class NoCopy
    {
        public:
            NoCopy(NoCopy const&) = delete;
            NoCopy& operator=(NoCopy const&) = delete;
            NoCopy() = default;
    };

} // namespace DORY

#endif // DORY_NO_COPY_INCL