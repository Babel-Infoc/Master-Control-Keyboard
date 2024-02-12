#ifndef EGL_MISSING_H
#define EGL_MISSING_H
#include <egl_math.h>
namespace egl{

// It would be nice to allow the use of vec2 vec3 etc as the default float version
//  


template<typename T> EGL_INLINE T fract(T x){ return x - floor(x);}

template<typename T, typename S> EGL_INLINE T mix(T a, T b, S alpha){return lerp(a,b,alpha);}

template<typename T> EGL_INLINE T length(T v            ){return norm(v);}
template<typename T> EGL_INLINE T length(vec2<T> v      ){return norm(v);}
template<typename T> EGL_INLINE T length(vec3<T> v      ){return norm(v);}
template<typename T> EGL_INLINE T length(vec4<T> v      ){return norm(v);}
template<typename T> EGL_INLINE T length(quat<T> v      ){return norm(v);}
template<typename T> EGL_INLINE T length(complex<T> v   ){return norm(v);}

template<typename T> EGL_INLINE T distance(T a         , T b          ){return norm(a - b);}
template<typename T> EGL_INLINE T distance(vec2<T> a   , vec2<T> b    ){return norm(a - b);}
template<typename T> EGL_INLINE T distance(vec3<T> a   , vec3<T> b    ){return norm(a - b);}
template<typename T> EGL_INLINE T distance(vec4<T> a   , vec4<T> b    ){return norm(a - b);}
template<typename T> EGL_INLINE T distance(quat<T> a   , quat<T> b    ){return norm(a - b);}
template<typename T> EGL_INLINE T distance(complex<T> a, complex<T> b ){return norm(a - b);}


//https://registry.khronos.org/OpenGL-Refpages/gl4/html/smoothstep.xhtml
template<typename T> EGL_INLINE T smoothstep (T a, T b, T x){
    T t = clamp((x - a) / (b - a), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

template<typename T> EGL_INLINE T normalize(T v){return v*(1/norm(v));}

template<typename T> EGL_INLINE T sign(T v){return sgn(v);}

template<typename T> EGL_INLINE T round(T v){return floor(v+0.5);}


// distributive property of cos(vector)


}
#endif // EGL_MISSING_H