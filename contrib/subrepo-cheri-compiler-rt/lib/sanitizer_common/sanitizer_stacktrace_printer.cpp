//===-- sanitizer_common.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file is shared between sanitizers' run-time libraries.
//
//===----------------------------------------------------------------------===//

#include "sanitizer_stacktrace_printer.h"

#include "sanitizer_file.h"
#include "sanitizer_flags.h"
#include "sanitizer_fuchsia.h"

namespace __sanitizer {

const char *StripFunctionName(const char *function) {
  if (!common_flags()->demangle)
    return function;
  if (!function)
    return nullptr;
  auto try_strip = [function](const char *prefix) -> const char * {
    const uptr prefix_len = internal_strlen(prefix);
    if (!internal_strncmp(function, prefix, prefix_len))
      return function + prefix_len;
    return nullptr;
  };
  if (SANITIZER_APPLE) {
    if (const char *s = try_strip("wrap_"))
      return s;
  } else if (SANITIZER_WINDOWS) {
    if (const char *s = try_strip("__asan_wrap_"))
      return s;
  } else {
    if (const char *s = try_strip("___interceptor_"))
      return s;
    if (const char *s = try_strip("__interceptor_"))
      return s;
  }
  return function;
}

// sanitizer_symbolizer_markup.cpp implements these differently.
#if !SANITIZER_SYMBOLIZER_MARKUP

static const char *DemangleFunctionName(const char *function) {
  if (!common_flags()->demangle)
    return function;
  if (!function)
    return nullptr;

  // NetBSD uses indirection for old threading functions for historical reasons
  // The mangled names are internal implementation detail and should not be
  // exposed even in backtraces.
#if SANITIZER_NETBSD
  if (!internal_strcmp(function, "__libc_mutex_init"))
    return "pthread_mutex_init";
  if (!internal_strcmp(function, "__libc_mutex_lock"))
    return "pthread_mutex_lock";
  if (!internal_strcmp(function, "__libc_mutex_trylock"))
    return "pthread_mutex_trylock";
  if (!internal_strcmp(function, "__libc_mutex_unlock"))
    return "pthread_mutex_unlock";
  if (!internal_strcmp(function, "__libc_mutex_destroy"))
    return "pthread_mutex_destroy";
  if (!internal_strcmp(function, "__libc_mutexattr_init"))
    return "pthread_mutexattr_init";
  if (!internal_strcmp(function, "__libc_mutexattr_settype"))
    return "pthread_mutexattr_settype";
  if (!internal_strcmp(function, "__libc_mutexattr_destroy"))
    return "pthread_mutexattr_destroy";
  if (!internal_strcmp(function, "__libc_cond_init"))
    return "pthread_cond_init";
  if (!internal_strcmp(function, "__libc_cond_signal"))
    return "pthread_cond_signal";
  if (!internal_strcmp(function, "__libc_cond_broadcast"))
    return "pthread_cond_broadcast";
  if (!internal_strcmp(function, "__libc_cond_wait"))
    return "pthread_cond_wait";
  if (!internal_strcmp(function, "__libc_cond_timedwait"))
    return "pthread_cond_timedwait";
  if (!internal_strcmp(function, "__libc_cond_destroy"))
    return "pthread_cond_destroy";
  if (!internal_strcmp(function, "__libc_rwlock_init"))
    return "pthread_rwlock_init";
  if (!internal_strcmp(function, "__libc_rwlock_rdlock"))
    return "pthread_rwlock_rdlock";
  if (!internal_strcmp(function, "__libc_rwlock_wrlock"))
    return "pthread_rwlock_wrlock";
  if (!internal_strcmp(function, "__libc_rwlock_tryrdlock"))
    return "pthread_rwlock_tryrdlock";
  if (!internal_strcmp(function, "__libc_rwlock_trywrlock"))
    return "pthread_rwlock_trywrlock";
  if (!internal_strcmp(function, "__libc_rwlock_unlock"))
    return "pthread_rwlock_unlock";
  if (!internal_strcmp(function, "__libc_rwlock_destroy"))
    return "pthread_rwlock_destroy";
  if (!internal_strcmp(function, "__libc_thr_keycreate"))
    return "pthread_key_create";
  if (!internal_strcmp(function, "__libc_thr_setspecific"))
    return "pthread_setspecific";
  if (!internal_strcmp(function, "__libc_thr_getspecific"))
    return "pthread_getspecific";
  if (!internal_strcmp(function, "__libc_thr_keydelete"))
    return "pthread_key_delete";
  if (!internal_strcmp(function, "__libc_thr_once"))
    return "pthread_once";
  if (!internal_strcmp(function, "__libc_thr_self"))
    return "pthread_self";
  if (!internal_strcmp(function, "__libc_thr_exit"))
    return "pthread_exit";
  if (!internal_strcmp(function, "__libc_thr_setcancelstate"))
    return "pthread_setcancelstate";
  if (!internal_strcmp(function, "__libc_thr_equal"))
    return "pthread_equal";
  if (!internal_strcmp(function, "__libc_thr_curcpu"))
    return "pthread_curcpu_np";
  if (!internal_strcmp(function, "__libc_thr_sigsetmask"))
    return "pthread_sigmask";
#endif

  return function;
}

static void MaybeBuildIdToBuffer(const AddressInfo &info, bool PrefixSpace,
                                 InternalScopedString *buffer) {
  if (info.uuid_size) {
    if (PrefixSpace)
      buffer->append(" ");
    buffer->append("(BuildId: ");
    for (uptr i = 0; i < info.uuid_size; ++i) {
      buffer->append("%02x", info.uuid[i]);
    }
    buffer->append(")");
  }
}

static const char kDefaultFormat[] = "    #%n %p %F %L";

void RenderFrame(InternalScopedString *buffer, const char *format, int frame_no,
                 uptr address, const AddressInfo *info, bool vs_style,
                 const char *strip_path_prefix) {
  // info will be null in the case where symbolization is not needed for the
  // given format. This ensures that the code below will get a hard failure
  // rather than print incorrect information in case RenderNeedsSymbolization
  // ever ends up out of sync with this function. If non-null, the addresses
  // should match.
  CHECK(!info || address == info->address);
  if (0 == internal_strcmp(format, "DEFAULT"))
    format = kDefaultFormat;
  for (const char *p = format; *p != '\0'; p++) {
    if (*p != '%') {
      buffer->append("%c", *p);
      continue;
    }
    p++;
    switch (*p) {
    case '%':
      buffer->append("%%");
      break;
    // Frame number and all fields of AddressInfo structure.
    case 'n':
      buffer->append("%u", frame_no);
      break;
    case 'p':
      buffer->append("0x%zx", address);
      break;
    case 'm':
      buffer->append("%s", StripPathPrefix(info->module, strip_path_prefix));
      break;
    case 'o':
      buffer->append("0x%zx", info->module_offset);
      break;
    case 'b':
      MaybeBuildIdToBuffer(*info, /*PrefixSpace=*/false, buffer);
      break;
    case 'f':
      buffer->append("%s",
                     DemangleFunctionName(StripFunctionName(info->function)));
      break;
    case 'q':
      buffer->append("0x%zx", info->function_offset != AddressInfo::kUnknown
                                  ? info->function_offset
                                  : 0x0);
      break;
    case 's':
      buffer->append("%s", StripPathPrefix(info->file, strip_path_prefix));
      break;
    case 'l':
      buffer->append("%d", info->line);
      break;
    case 'c':
      buffer->append("%d", info->column);
      break;
    // Smarter special cases.
    case 'F':
      // Function name and offset, if file is unknown.
      if (info->function) {
        buffer->append("in %s",
                       DemangleFunctionName(StripFunctionName(info->function)));
        if (!info->file && info->function_offset != AddressInfo::kUnknown)
          buffer->append("+0x%zx", info->function_offset);
      }
      break;
    case 'S':
      // File/line information.
      RenderSourceLocation(buffer, info->file, info->line, info->column,
                           vs_style, strip_path_prefix);
      break;
    case 'L':
      // Source location, or module location.
      if (info->file) {
        RenderSourceLocation(buffer, info->file, info->line, info->column,
                             vs_style, strip_path_prefix);
      } else if (info->module) {
        RenderModuleLocation(buffer, info->module, info->module_offset,
                             info->module_arch, strip_path_prefix);

#if !SANITIZER_APPLE
        MaybeBuildIdToBuffer(*info, /*PrefixSpace=*/true, buffer);
#endif
      } else {
        buffer->append("(<unknown module>)");
      }
      break;
    case 'M':
      // Module basename and offset, or PC.
      if (address & kExternalPCBit) {
        // There PCs are not meaningful.
      } else if (info->module) {
        // Always strip the module name for %M.
        RenderModuleLocation(buffer, StripModuleName(info->module),
                             info->module_offset, info->module_arch, "");
#if !SANITIZER_APPLE
        MaybeBuildIdToBuffer(*info, /*PrefixSpace=*/true, buffer);
#endif
      } else {
        buffer->append("(%p)", (void *)(uptr)address);
      }
      break;
    default:
      Report("Unsupported specifier in stack frame format: %c (%p)!\n", *p,
             (void *)p);
      Die();
    }
  }
}

bool RenderNeedsSymbolization(const char *format) {
  if (0 == internal_strcmp(format, "DEFAULT"))
    format = kDefaultFormat;
  for (const char *p = format; *p != '\0'; p++) {
    if (*p != '%')
      continue;
    p++;
    switch (*p) {
      case '%':
        break;
      case 'n':
        // frame_no
        break;
      case 'p':
        // address
        break;
      default:
        return true;
    }
  }
  return false;
}

void RenderData(InternalScopedString *buffer, const char *format,
                const DataInfo *DI, const char *strip_path_prefix) {
  for (const char *p = format; *p != '\0'; p++) {
    if (*p != '%') {
      buffer->append("%c", *p);
      continue;
    }
    p++;
    switch (*p) {
      case '%':
        buffer->append("%%");
        break;
      case 's':
        buffer->append("%s", StripPathPrefix(DI->file, strip_path_prefix));
        break;
      case 'l':
        buffer->append("%zu", DI->line);
        break;
      case 'g':
        buffer->append("%s", DI->name);
        break;
      default:
        Report("Unsupported specifier in stack frame format: %c (%p)!\n", *p,
               (void *)p);
        Die();
    }
  }
}

#endif  // !SANITIZER_SYMBOLIZER_MARKUP

void RenderSourceLocation(InternalScopedString *buffer, const char *file,
                          int line, int column, bool vs_style,
                          const char *strip_path_prefix) {
  if (vs_style && line > 0) {
    buffer->append("%s(%d", StripPathPrefix(file, strip_path_prefix), line);
    if (column > 0)
      buffer->append(",%d", column);
    buffer->append(")");
    return;
  }

  buffer->append("%s", StripPathPrefix(file, strip_path_prefix));
  if (line > 0) {
    buffer->append(":%d", line);
    if (column > 0)
      buffer->append(":%d", column);
  }
}

void RenderModuleLocation(InternalScopedString *buffer, const char *module,
                          uptr offset, ModuleArch arch,
                          const char *strip_path_prefix) {
  buffer->append("(%s", StripPathPrefix(module, strip_path_prefix));
  if (arch != kModuleArchUnknown) {
    buffer->append(":%s", ModuleArchToString(arch));
  }
  buffer->append("+0x%zx)", offset);
}

} // namespace __sanitizer
