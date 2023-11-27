#include <cstdint>
#include <iostream>
#include <ratio>
namespace usu
{
    // Default ratio is 1 to 1, default type is std::uint64_t
    template <typename T = std::ratio<1, 1>, typename R = std::uint64_t>
    class mass
    {
      public:
        mass();
        mass(R count);
        R count() const;
        using conversion = T;
        using localType = R;

        // Convert other type of mass to 'current' type first.
        template <typename X>
        mass operator+(const X& other) const
        {
            return mass(count() + convert(other));
        }
        // typename X is used for most operators, X is potentially differing mass type
        template <typename X>

        bool operator==(const X& other) const
        {

            return count() == convert(other);
        }

        template <typename X>

        bool operator!=(const X& other) const
        {
            return count() != convert(other);
        }

        template <typename X>

        bool operator<(const X& other) const
        {
            return count() < convert(other);
        }

        template <typename X>

        bool operator<=(const X& other) const
        {
            return count() <= convert(other);
        }

        template <typename X>

        bool operator>(const X& other) const
        {
            return count() > convert(other);
        }

        template <typename X>

        bool operator>=(const X& other) const
        {
            return count() >= convert(other);
        }

        template <typename X>
        auto operator<=>(const X& other) const
        {

            auto converted = mass_cast<mass<T, R>>(other).count();
            if constexpr (std::is_integral<decltype(converted)>::value)
            {
                if (count() > converted)
                {
                    return std::strong_ordering::greater;
                }
                else if (count() < converted)
                {
                    return std::strong_ordering::less;
                }
                return std::strong_ordering::equal;
            }
            else
            {
                if (count() > converted)
                {
                    return std::partial_ordering::greater;
                }
                else if (count() < converted)
                {
                    return std::partial_ordering::less;
                }
                return std::partial_ordering::equivalent;
            }
        }

        template <typename X>
        mass& operator+=(const X& other)
        {

            m_counter += convert(other);
            return *this;
        }
        template <typename X>
        mass operator-(const X& other) const
        {
            return mass(count() - convert(other));
        }
        template <typename X>
        mass& operator-=(const X& other)
        {
            m_counter -= convert(other);
            return *this;
        }

        // Scalar is on right side
        template <typename S>
        mass operator*(const S scalar) const
        {
            return mass(count() * static_cast<localType>(scalar));
        }
        template <typename S>
        mass& operator*=(const S scalar)
        {
            m_counter *= static_cast<localType>(scalar);
            return *this;
        }

        template <typename S>
        mass operator/(const S scalar) const
        {
            return mass(count() / static_cast<localType>(scalar));
        }
        template <typename S>

        mass& operator/=(const S scalar)
        {
            m_counter /= static_cast<localType>(scalar);
            return *this;
        }

      private:
        R m_counter;

        // Helper to get the ratio
        template <typename X>
        R convert(const X& other) const
        {

            double otherRatio = other.count() * (static_cast<double>(X::conversion::num) / X::conversion::den);
            double newRatio = static_cast<double>(otherRatio * (static_cast<double>(conversion::den) / conversion::num));
            return static_cast<R>(newRatio);
        }
    };

    // Mass cast, returns new mass object.
    template <typename L, typename S>
    L mass_cast(S other)
    {

        double otherRatio = other.count() * (static_cast<double>(S::conversion::num) / S::conversion::den);
        double newRatio = static_cast<double>(otherRatio * (static_cast<double>(L::conversion::den) / L::conversion::num));
        return L(static_cast<L::localType>(newRatio));
    }

    template <typename T, typename R>
    mass<T, R>::mass()
    {
        m_counter = 0;
    }

    template <typename T, typename R>
    mass<T, R>::mass(R count) :
        m_counter(count)
    {
    }

    template <typename T, typename R>
    R mass<T, R>::count() const
    {
        return m_counter;
    }

    // Scalar is on left side
    template <typename T, typename R, typename S>
    mass<T, R> operator*(S scalar, const mass<T, R>& oldMass)
    {
        return mass<T, R>(static_cast<R>(scalar) * oldMass.count());
    }

    using gram = mass<std::ratio<1, 1>>;
    using microgram = mass<std::ratio<1, 1000000>>;
    using kilogram = mass<std::ratio<1000, 1>, double>;
    using ounce = mass<std::ratio<28349523125, 1000000000>, double>;
    using pound = mass<std::ratio<45359237, 100000>, double>;
    using ton = mass<std::ratio<90718474, 100>, double>;

}; // namespace usu