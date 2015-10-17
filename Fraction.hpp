#ifndef FRACTION_HPP_INCLUDED
#define FRACTION_HPP_INCLUDED

#include <iostream>
#include <ostream>


///  \file   Fraction.hpp
///  \brief  This file is a short mathematical Fraction library.
///          The Fraction format is Num/Den (with "num" and "den" integer)
///          This library does not manage exception, and can return Inf or NaN
///  \author Dedeun
///  \date   30 sept 2015
///  \version Beta 1.0

///  \class Fraction
///  \brief This class is the mathematical fraction, with the 4 operations (+, -, *, /)
///         and comparison (>, >=, <, <=, ==, and !=)
///         (the fraction are stored in reduted forme)
namespace dd {
    template<typename T>
    class Fraction final {
    public:
        /// \fn    Fraction ();
        /// \brief Constructor (with only numerator)
        ///        default numerator is set to 1
        /// \param Numerator
        Fraction<T> (T num=0): m_num{num}, m_den{1}
        {
            static_assert(std::is_integral<T>::value, "Integer required.");
        }

        /// \fn    Fraction ();
        /// \brief Constructor (with numerator and denominator)
        /// \param Numerator and denominator
        /// \pre   Denominator shall be not null
        /// \invariant Denominator shall be not null
        Fraction<T> (T num, T den): m_num{num}
        {
            static_assert(std::is_integral<T>::value, "Integer required.");
            if (den > 0) {
                m_den=den;
            } else {
                m_den=-den;
                m_num *=-1;
            } //end if
            reduction ();
        }

        /// \fn    isFinite
        /// \brief return true if Fraction is finite
        bool isFinite() const
        {
            return (m_den != 0);
        }
        /// \fn    isInf
        /// \brief return true if Fraction is Infinite
        bool isInf() const
        {
            return ((m_den == 0)&&(m_num != 0));
        }
        /// \fn    isNan
        /// \brief return true if Fraction is Not a Number
        bool isNan() const
        {
            return ((m_den == 0)&&(m_num == 0));
        }

        /// \fn    +=
        /// \brief Self addition
        /// \param the Fraction to be added
        Fraction<T> operator+= (Fraction<T> const& f)
        {
            m_num = (m_num * f.m_den) + (f.m_num * m_den);
            m_den = m_den*f.m_den;
            reduction();
            return (*this);
        }

        /// \fn    -=
        /// \brief Self subtraction
        /// \param the Fraction to be subtract
        Fraction<T> operator-= (Fraction<T> const& f)
        {
            m_num = (m_num * f.m_den) - (f.m_num * m_den);
            m_den = m_den*f.m_den;
            reduction();
            return (*this);
        }

        /// \fn    *=
        /// \brief Self multiplication
        /// \param the Fraction to be multiply
        Fraction<T> operator*= (Fraction<T> const& f)
        {
            m_num = m_num * f.m_num;
            m_den = m_den*f.m_den;
            reduction();
            return (*this);
        }

        /// \fn    /=
        /// \brief Self division
        /// \param the Fraction to be divided
        Fraction<T> operator/= (Fraction<T> const& f)
        {
            m_num = m_num * f.m_den;
            m_den = m_den * f.m_num;
            reduction();
            return (*this);
        }

        /// \fn    <<
        /// \brief Output on flux
        /// \param reference to the output flux
        /// \param reference of the Fraction
        friend std::ostream& operator<< (std::ostream& flux, Fraction<T>const& f)
        {
            if (f.isNan()) {
                flux << "NaN";
            } else if (f.isInf()) {
                flux << "Inf";
            } else {
                flux << f.m_num << "/" << f.m_den;
            } // endif
            return flux;
        }

        /// \fn    ==
        /// \brief comparison: is equal to
        /// \param the Fraction to be compared
        friend bool operator== (Fraction<T> const& f1, Fraction<T> const& f2)
        {
            return ((f1.m_num == f2.m_num) & (f1.m_den == f2.m_den));
        }

        /// \fn    >
        /// \brief comparison: is greater than
        /// \param the Fraction to be compared
        friend bool operator> (Fraction<T> const& f1, Fraction<T> const& f2)
        {
            return ((f1.m_num * f2.m_den) > (f2.m_num * f1.m_den));
        }

    protected:
    private:
        // This function compute the "greater commum divisor
        // (algorithm find on web: http://codes-sources.commentcamarche.net/source/10495
        T PGCD(T a, T b)
        {
            T r= a%b;
            while(r) {
                a=b;
                b=r;
                r=a%b;
            } // end while
            return b;
        }

        /// \fn    reduction()
        /// \brief Reduction of the fraction: Computation of PGDC and divide Numerator/denominator by this PGDC
        void reduction ()
        {
            if (m_den != 0){
                // case negative denominator, change the numerator and denominator signs

                if (m_den < 0){
                    m_num *= -1;
                    m_den *= -1;
                } //end if
                // standard case compute PGDC, and divide num/den by PGDC
                T div {1};
                if (m_num >= 0){
                    div = PGCD (m_num, m_den);
                } else {
                    div = PGCD (-1*m_num, m_den);
                } // end if
                m_num /= div;
                m_den /= div;
            } //end if
       }

        /// \var   m_num
        /// \brief member variable: value of numerator
        T  m_num;
        /// \var   m_den
        /// \brief member variable: value of denominator
        T m_den;
    }; // end class

    /// \fn    +
    /// \brief Addition
    /// \param the Fraction to be added
    template<typename T>
    Fraction<T> operator+ (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        Fraction<T> f {f1};
        return (f+=f2);
    }

    /// \fn    -
    /// \brief Subtraction
    /// \param the Fraction to be subtract
    template<typename T>
    Fraction<T> operator- (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        Fraction<T> f {f1};
        return (f-=f2);
    }

    /// \fn    *
    /// \brief Multiplication
    /// \param the Fraction to be multiplied
    template<typename T>
    Fraction<T> operator* (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        Fraction<T> f {f1};
        return (f*=f2);
    }

    /// \fn    /
    /// \brief Division
    /// \param the Fraction to be divided
    template<typename T>
    Fraction<T> operator/ (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        Fraction<T> f {f1};
        return (f/=f2);
    }

    /// \fn    !=
    /// \brief comparison: is not equal to
    /// \param the Fraction to be compared
    template<typename T>
    bool operator!= (Fraction<T> const& f1, Fraction<T> const& f2)
    {
       return (!(f1==f2));
    }

    /// \fn    >=
    /// \brief comparison: is greater or equal to
    /// \param the Fraction to be compared
    template<typename T>
    bool operator>= (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        return (!(f2>f1));
    }

    /// \fn    <
    /// \brief comparison: is smaller than
    /// \param the Fraction to be compared
    template<typename T>
    bool operator< (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        return (f2>f1);
    }

    /// \fn    <=
    /// \brief comparison: is smaller or equal to
    /// \param the Fraction to be compared
    template<typename T>
    bool operator<= (Fraction<T> const& f1, Fraction<T> const& f2)
    {
        return (!(f1>f2));
    }

} //end namespace
#endif // FRACTION_HPP_INCLUDED
