/*
 * Copyright (C) 2011, 2012, 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef WTF_Compiler_h
#define WTF_Compiler_h

/* COMPILER() - the compiler being used to build the project */
#define COMPILER(WTF_FEATURE) (defined WTF_COMPILER_##WTF_FEATURE  && WTF_COMPILER_##WTF_FEATURE)

/* COMPILER_SUPPORTS() - whether the compiler being used to build the project supports the given feature. */
#define COMPILER_SUPPORTS(WTF_COMPILER_FEATURE) (defined WTF_COMPILER_SUPPORTS_##WTF_COMPILER_FEATURE  && WTF_COMPILER_SUPPORTS_##WTF_COMPILER_FEATURE)

/* COMPILER_QUIRK() - whether the compiler being used to build the project requires a given quirk. */
#define COMPILER_QUIRK(WTF_COMPILER_QUIRK) (defined WTF_COMPILER_QUIRK_##WTF_COMPILER_QUIRK  && WTF_COMPILER_QUIRK_##WTF_COMPILER_QUIRK)

/* COMPILER_HAS_CLANG_BUILTIN() - wether the compiler supports a particular clang builtin. */
#ifdef __has_builtin
#define COMPILER_HAS_CLANG_BUILTIN(x) __has_builtin(x)
#else
#define COMPILER_HAS_CLANG_BUILTIN(x) 0
#endif

/* ==== COMPILER() - primary detection of the compiler being used to build the project, in alphabetical order ==== */

/* COMPILER(CLANG) - Clang  */

#if defined(__clang__)
#define WTF_COMPILER_CLANG 1
#define WTF_COMPILER_SUPPORTS_BLOCKS __has_feature(blocks)
#define WTF_COMPILER_SUPPORTS_C_STATIC_ASSERT __has_feature(c_static_assert)
#define WTF_COMPILER_SUPPORTS_CXX_CONSTEXPR __has_feature(cxx_constexpr)
#define WTF_COMPILER_SUPPORTS_CXX_REFERENCE_QUALIFIED_FUNCTIONS __has_feature(cxx_reference_qualified_functions)
#define WTF_COMPILER_SUPPORTS_CXX_USER_LITERALS __has_feature(cxx_user_literals)
#define WTF_COMPILER_SUPPORTS_FALLTHROUGH_WARNINGS __has_feature(cxx_attributes) && __has_warning("-Wimplicit-fallthrough")
#endif

/* COMPILER(GCC) - GNU Compiler Collection */

/* Note: This section must come after the Clang section since we check !COMPILER(CLANG) here. */

#if defined(__GNUC__)
#define WTF_COMPILER_GCC 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define GCC_VERSION_AT_LEAST(major, minor, patch) (GCC_VERSION >= (major * 10000 + minor * 100 + patch))
#endif

/* Define GCC_VERSION_AT_LEAST for all compilers, so we can write things like GCC_VERSION_AT_LEAST(4, 1, 0). */
/* FIXME: Doesn't seem all that valuable. Can we remove this? */
#if !defined(GCC_VERSION_AT_LEAST)
#define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif

#if COMPILER(GCC) && !COMPILER(CLANG) && !GCC_VERSION_AT_LEAST(4, 7, 0)
#error "Please use a newer version of GCC. WebKit requires GCC 4.7.0 or newer to compile."
#endif

#if COMPILER(GCC) && !COMPILER(CLANG)
#define WTF_COMPILER_SUPPORTS_CXX_CONSTEXPR 1
#define WTF_COMPILER_SUPPORTS_CXX_USER_LITERALS 1
#endif

#if COMPILER(GCC) && !COMPILER(CLANG) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define WTF_COMPILER_SUPPORTS_C_STATIC_ASSERT 1
#endif

#if COMPILER(GCC) && !COMPILER(CLANG) && GCC_VERSION_AT_LEAST(4, 8, 0)
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif

#if COMPILER(GCC) && !COMPILER(CLANG) && (defined(__GXX_EXPERIMENTAL_CXX0X__) || (defined(__cplusplus) && __cplusplus >= 201103L))
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif

/* COMPILER(MINGW) - MinGW GCC */

#if defined(__MINGW32__)
#define WTF_COMPILER_MINGW 1
#include <_mingw.h>
#endif

/* COMPILER(MINGW64) - mingw-w64 GCC - used as additional check to exclude mingw.org specific functions */

/* Note: This section must come after the MinGW section since we check COMPILER(MINGW) here. */

#if COMPILER(MINGW) && defined(__MINGW64_VERSION_MAJOR) /* best way to check for mingw-w64 vs mingw.org */
#define WTF_COMPILER_MINGW64 1
#endif

/* COMPILER(MSVC) - Microsoft Visual C++ */

#if defined(_MSC_VER)
#define WTF_COMPILER_MSVC 1
#endif

#if defined(_MSC_VER) && _MSC_VER < 1800
#error "Please use a newer version of Visual Studio. WebKit requires VS2013 or newer to compile."
#endif

/* COMPILER(SUNCC) */

#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#define WTF_COMPILER_SUNCC 1
#endif

#if !COMPILER(CLANG) && !COMPILER(MSVC)
#define WTF_COMPILER_QUIRK_CONSIDERS_UNREACHABLE_CODE 1
#endif

/* ==== COMPILER_SUPPORTS - additional compiler feature detection, in alphabetical order ==== */

/* COMPILER_SUPPORTS(EABI) */

#if defined(__ARM_EABI__) || defined(__EABI__)
#define WTF_COMPILER_SUPPORTS_EABI 1
#endif

#if defined(__has_feature)
#define ASAN_ENABLED __has_feature(address_sanitizer)
#else
#define ASAN_ENABLED 0
#endif

#if ASAN_ENABLED
#define SUPPRESS_ASAN __attribute__((no_sanitize_address))
#else
#define SUPPRESS_ASAN
#endif

/* ==== Compiler-independent macros for various compiler features, in alphabetical order ==== */

/* ALWAYS_INLINE */

#if !defined(ALWAYS_INLINE) && COMPILER(GCC) && defined(NDEBUG) && !COMPILER(MINGW)
#define ALWAYS_INLINE inline __attribute__((__always_inline__))
#endif

#if !defined(ALWAYS_INLINE) && COMPILER(MSVC) && defined(NDEBUG)
#define ALWAYS_INLINE __forceinline
#endif

#if !defined(ALWAYS_INLINE)
#define ALWAYS_INLINE inline
#endif

/* CONSTEXPR */

#if !defined(CONSTEXPR) && COMPILER_SUPPORTS(CXX_CONSTEXPR)
#define CONSTEXPR constexpr
#endif

#if !defined(CONSTEXPR)
#define CONSTEXPR
#endif

/* WTF_EXTERN_C_{BEGIN, END} */

#ifdef __cplusplus
#define WTF_EXTERN_C_BEGIN extern "C" {
#define WTF_EXTERN_C_END }
#else
#define WTF_EXTERN_C_BEGIN
#define WTF_EXTERN_C_END
#endif

/* FIXME: Remove this once we have transitioned to WTF_EXTERN_C_BEGIN/WTF_EXTERN_C_END. */
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

/* FALLTHROUGH */

#if !defined(FALLTHROUGH) && COMPILER_SUPPORTS(FALLTHROUGH_WARNINGS) && COMPILER(CLANG)
#define FALLTHROUGH [[clang::fallthrough]]
#endif

#if !defined(FALLTHROUGH)
#define FALLTHROUGH
#endif

/* LIKELY */

#if !defined(LIKELY) && COMPILER(GCC)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif

#if !defined(LIKELY)
#define LIKELY(x) (x)
#endif

/* NEVER_INLINE */

#if !defined(NEVER_INLINE) && COMPILER(GCC)
#define NEVER_INLINE __attribute__((__noinline__))
#endif

#if !defined(NEVER_INLINE) && COMPILER(MSVC)
#define NEVER_INLINE __declspec(noinline)
#endif

#if !defined(NEVER_INLINE)
#define NEVER_INLINE
#endif

/* NO_RETURN */

#if !defined(NO_RETURN) && COMPILER(GCC)
#define NO_RETURN __attribute((__noreturn__))
#endif

#if !defined(NO_RETURN) && COMPILER(MSVC)
#define NO_RETURN __declspec(noreturn)
#endif

#if !defined(NO_RETURN)
#define NO_RETURN
#endif

/* NO_RETURN_WITH_VALUE */

#if !defined(NO_RETURN_WITH_VALUE) && !COMPILER(MSVC)
#define NO_RETURN_WITH_VALUE NO_RETURN
#endif

#if !defined(NO_RETURN_WITH_VALUE)
#define NO_RETURN_WITH_VALUE
#endif

/* OBJC_CLASS */

#if !defined(OBJC_CLASS) && defined(__OBJC__)
#define OBJC_CLASS @class
#endif

#if !defined(OBJC_CLASS)
#define OBJC_CLASS class
#endif

/* PURE_FUNCTION */

#if !defined(PURE_FUNCTION) && COMPILER(GCC)
#define PURE_FUNCTION __attribute__((__pure__))
#endif

#if !defined(PURE_FUNCTION)
#define PURE_FUNCTION
#endif

/* REFERENCED_FROM_ASM */

#if !defined(REFERENCED_FROM_ASM) && COMPILER(GCC)
#define REFERENCED_FROM_ASM __attribute__((__used__))
#endif

#if !defined(REFERENCED_FROM_ASM)
#define REFERENCED_FROM_ASM
#endif

/* UNLIKELY */

#if !defined(UNLIKELY) && COMPILER(GCC)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

#if !defined(UNLIKELY)
#define UNLIKELY(x) (x)
#endif

/* UNUSED_LABEL */

/* Keep the compiler from complaining for a local label that is defined but not referenced. */
/* Helpful when mixing hand-written and autogenerated code. */

#if !defined(UNUSED_LABEL) && COMPILER(MSVC)
#define UNUSED_LABEL(label) if (false) goto label
#endif

#if !defined(UNUSED_LABEL)
#define UNUSED_LABEL(label) UNUSED_PARAM(&& label)
#endif

/* UNUSED_PARAM */

#if !defined(UNUSED_PARAM) && COMPILER(MSVC)
#define UNUSED_PARAM(variable) (void)&variable
#endif

#if !defined(UNUSED_PARAM)
#define UNUSED_PARAM(variable) (void)variable
#endif

/* WARN_UNUSED_RETURN */

#if !defined(WARN_UNUSED_RETURN) && COMPILER(GCC)
#define WARN_UNUSED_RETURN __attribute__((__warn_unused_result__))
#endif

#if !defined(WARN_UNUSED_RETURN)
#define WARN_UNUSED_RETURN
#endif

#if !defined(__has_include) && COMPILER(MSVC)
#define __has_include(path) 0
#endif

#endif /* WTF_Compiler_h */
