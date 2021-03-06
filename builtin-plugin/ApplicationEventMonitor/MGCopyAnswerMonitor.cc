#include "./dobby_monitor.h"

#include <dlfcn.h>
#include <CoreFoundation/CoreFoundation.h>

#define LOG_TAG "MGCopyAnswer"

static uintptr_t getCallFirstArg(RegisterContext *reg_ctx) {
  uintptr_t result;
#if defined(_M_X64) || defined(__x86_64__)
#if defined(_WIN32)
  result = reg_ctx->general.regs.rcx;
#else
  result = reg_ctx->general.regs.rdi;
#endif
#elif defined(__arm64__) || defined(__aarch64__)
  result = reg_ctx->general.regs.x0;
#elif defined(__arm__)
  result = reg_ctx->general.regs.r0;
#else
#error "Not Support Architecture."
#endif
  return result;
}

void common_handler(RegisterContext *reg_ctx, const HookEntryInfo *info) {
  CFStringRef key_ = 0;
  key_             = (CFStringRef)getCallFirstArg(reg_ctx);

  char str_key[256] = {0};
  CFStringGetCString(key_, str_key, 256, kCFStringEncodingUTF8);
  LOG("[#] MGCopyAnswer:: %s\n", str_key);
}

#if 1
__attribute__((constructor)) static void ctor() {
  void *lib               = dlopen("/usr/lib/libMobileGestalt.dylib", RTLD_NOW);
  void *MGCopyAnswer_addr = DobbySymbolResolver("libMobileGestalt.dylib", "MGCopyAnswer");

  dobby_enable_near_branch_trampoline();
  DobbyInstrument((void *)MGCopyAnswer_addr, common_handler);
  dobby_disable_near_branch_trampoline();
}
#endif
