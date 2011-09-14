#pragma once

#include "CommonDefs.hpp"

#include <math.h>

namespace Common
{
    namespace Math
    {
        static const float Pi = 3.141592654f;

        template<typename T>
        inline T ToRadians(T degrees)
        {
            return ( degrees * ( Pi / 180.0f ) );
        };

        template<typename T>
        inline T ToDegrees(T radians)
        {
            return ( radians * ( 180.0f / Pi ) );
        };

        /**
         * Wraps a numerical value into a given range. Similar to the modulo operator, but
         * functions on all numerical types (including floats).
         */
        template<typename T>
        T Wrap(T input, T max, T step)
        {
            T value = input;
            while ( value > max )
            {
                value = value - step;
            }
            return value;
        };

        template<typename T>
        T WrapUp(T input, T min, T step)
        {
            T value = input;
            while ( value < min )
            {
                value = value + step;
            }
            return value;
        }

        template<typename T>
        T WrapBoth(T input, T min, T max, T step)
        {
            T value = input;
            if ( value < min )
            {
                while ( value < min )
                {
                    value = value + step;
                }
            } else if ( value > max ) {
                while ( value > max )
                {
                    value = value - step;
                }
            }
            return value;
        }

        /**
         * Wraps a numerical object into a given range, using a ping-pong method. If input
         * is greater than max but less than 2*max, it is wrapped into the 0-max range and
         * inverted (max - value).
         */
        template<typename T>
        T Bounce(T input, T max)
        {
            T cap = max * 2;
            T value = Wrap<T>(input, cap, cap);
            if ( value > max )
            {
                return max - value;
            } else {
                return value;
            }
        };

        /**
         * Forces a value to be within a given range. If value is less than bottom, bottom
         * will be returned. If value is greater than top, top will be returned. Otherwise,
         * value will be returned.
         */
        template<typename T>
        inline T Clamp(T bottom, T top, T value)
        {
            return ( value < bottom ) ? bottom : ( ( value > top ) ? top : value );
        };

        /**
         * Performs a simple linear interpolation between two values.
         */
        template<typename T>
        inline T Lerp(T a, T b, float x)
        {
            return ( a * ( 1 - x ) ) + ( b * x );
        };

        /**
         * Performs a cosine interpolation between two values. This is a much smoother
         * curve than linear interpolation.
         */
        template<typename T>
        T CosInterp(T a, T b, float x)
        {
            float ft = x * Pi;
            float f = ( 1 - cos(ft) ) * 0.5f;
            return ( a * ( 1 - f ) ) + ( b * f );
        };

        template<typename T>
        T Abs(T x)
        {
            return ( x < 0 ) ? -x : x;
        }

        template<typename T>
        bool Compare(T a, T b, T epsilon)
        {
            T c = Abs(a - b);
            return ( c < epsilon );
        }
    }
}
