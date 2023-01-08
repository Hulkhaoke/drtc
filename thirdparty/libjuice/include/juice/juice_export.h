
#ifndef JUICE_EXPORT_H
#define JUICE_EXPORT_H

#define JUICE_STATIC

#ifdef JUICE_STATIC
#  define JUICE_EXPORT
#  define JUICE_NO_EXPORT
#else
#  ifndef JUICE_EXPORT
#    ifdef juice_EXPORTS
        /* We are building this library */
#      define JUICE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define JUICE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef JUICE_NO_EXPORT
#    define JUICE_NO_EXPORT 
#  endif
#endif

#ifndef JUICE_DEPRECATED
#  define JUICE_DEPRECATED __declspec(deprecated)
#endif

#ifndef JUICE_DEPRECATED_EXPORT
#  define JUICE_DEPRECATED_EXPORT JUICE_EXPORT JUICE_DEPRECATED
#endif

#ifndef JUICE_DEPRECATED_NO_EXPORT
#  define JUICE_DEPRECATED_NO_EXPORT JUICE_NO_EXPORT JUICE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef JUICE_NO_DEPRECATED
#    define JUICE_NO_DEPRECATED
#  endif
#endif

#endif /* JUICE_EXPORT_H */
