//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_ERROR_H
#define RHI_ERROR_H

namespace rhi
{
    struct error
    {
        enum class code
        {
            INSTANCE_FAIL
        } code;

        const char* message { "[UNKNOWN RHI ERROR]" };

        [[nodiscard]] explicit error(enum code code, const char* message) noexcept
            : code { code }
            , message { message }
        {}
    };
} // namespace rhi

#endif //RHI_ERROR_H