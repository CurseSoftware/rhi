//
// Created by ralex on 9/28/2025.
//

#ifndef RHI_EXPECTED_H
#define RHI_EXPECTED_H
#include <exception>
#include <type_traits>
#include <utility>
#include <variant>

namespace rhi
{
    template <typename T, typename E>
    class expected;

    template <typename E>
    class unexpected
    {
    public:
        unexpected() = delete;

        constexpr explicit unexpected(const E& e) noexcept : _value{ e } {}
        constexpr explicit unexpected(E&& e) noexcept : _value{ std::move(e) } {}

        [[nodiscard]] constexpr const E& value() const & noexcept { return _value; }
        [[nodiscard]] constexpr const E&& value() const && noexcept { return std::move(_value); }

        template <typename T>
        constexpr explicit operator expected<T, E>() const & noexcept
        {
            return expected<T, E>{ unexpected(_value) };
        }

        template <typename T>
        constexpr explicit operator expected<T, E>() const && noexcept
        {
            return expected<T, E>{ unexpected(std::move(_value)) };
        }

    private:
        E _value;
    };

    template <typename T>
    class ok
    {
    public:
        explicit constexpr ok(const T& t) noexcept : _value{ t } {}
        explicit constexpr ok(T&& t) noexcept : _value{ std::move(t) } {}

        [[nodiscard]] constexpr const T& value() const & noexcept { return _value; }
        [[nodiscard]] constexpr const T&& value() const && noexcept { return std::move(_value); }

        [[nodiscard]] constexpr ok& operator=(const ok& other) noexcept = default;
        [[nodiscard]] constexpr ok& operator=(ok&& other) noexcept = default;

        template <typename E>
        constexpr explicit operator expected<T, E>() const & noexcept
        {
            return expected<T, E>( ok(_value) );
        }

        template <typename E>
        constexpr explicit operator expected<T, E>() const && noexcept
        {
            return expected<T, E>( ok(std::move(_value)) );
        }

    private:
        T _value;
    };

    template <typename T, typename E>
    class expected
    {
    public:
        [[nodiscard]] expected() noexcept : _is_ok{ false } {}
        [[nodiscard]] expected(ok<T> value) noexcept : _value{ value.value() }, _is_ok{ true } {}
        [[nodiscard]] expected(unexpected<E> value) noexcept : _value{ value.value() }, _is_ok { false } {}

        template <typename U>
        [[nodiscard]] explicit expected(ok<U> value) noexcept : _value{ value }
        {
            static_assert(std::is_constructible_v<T, U>(), "Cannot construct instance  of expected<T> from non-convertible type");
        }

        template <typename U>
        [[nodiscard]] explicit expected(unexpected<U> value) noexcept : _value{ value }
        {
            static_assert(std::is_constructible_v<T, U>(), "Cannot construct instance  of expected<T> from non-convertible type");
        }

        expected(const expected& other) noexcept
        {
            _is_ok = other._is_ok;
            _value = other._value;
        }

        expected& operator=(const expected& other) noexcept
        {
            if (*this != other)
            {
                _is_ok = other._is_ok;
                _value = other._value;
            }

            return *this;
        }

        expected(expected&& other) noexcept
        {
            _is_ok = other._is_ok;
            _value = std::move(other._value);
        }

        [[nodiscard]] expected& operator=(expected&& other) noexcept
        {
            if (*this != other)
            {
                _is_ok = other._is_ok;
                _value = std::move(other._value);
            }

            return *this;
        }

    public:
        [[nodiscard]] constexpr bool has_value() const noexcept { return _is_ok; }

        [[nodiscard]] constexpr T&& unwrap() const && noexcept
        {
            static_assert(std::is_move_assignable_v<T>, "T must be movable");
            if (_is_ok)
            {
                return std::get<T>(std::move(_value));
            }

            std::terminate();
        }

        [[nodiscard]] constexpr const T& unwrap() const & noexcept
        {
            static_assert(std::is_copy_assignable_v<T>, "T must be copyable");
            if (_is_ok)
            {
                return std::get<T>(std::move(_value));
            }

            std::terminate();
        }

        [[nodiscard]] constexpr const T& unwrap_or(const T& other) const noexcept
        {
            if (_is_ok)
            {
                return std::get<T>(_value);
            }

            return other;
        }

        [[nodiscard]] constexpr E unwrap_error() const noexcept
        {
            if (!_is_ok)
            {
                std::terminate();
            }

            return std::get<E>(_value);
        }

        // TODO: support move-only types

    private:
        std::variant<T, E> _value;
        bool _is_ok { false };
    };
}

#endif //RHI_EXPECTED_H