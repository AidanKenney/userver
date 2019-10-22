#include <utils/jemalloc.hpp>

#ifdef JEMALLOC_ENABLED
#include <jemalloc/jemalloc.h>
#else
#include <errno.h>
#endif

namespace utils::jemalloc {

namespace {

#ifndef JEMALLOC_ENABLED
int mallctl(const char*, void*, size_t*, void*, size_t) { return ENOTSUP; }

void malloc_stats_print(void (*write_cb)(void*, const char*), void* je_cbopaque,
                        const char*) {
  write_cb(je_cbopaque, "(libjemalloc support is disabled)");
}
#endif

std::error_code MakeErrorCode(int rc) {
  return std::error_code(rc, std::system_category());
}

}  // namespace

std::error_code MallCtlBool(const std::string& name, bool new_value) {
  int rc =
      mallctl(name.c_str(), nullptr, nullptr, &new_value, sizeof(new_value));
  return MakeErrorCode(rc);
}

std::error_code MallCtl(const std::string& name) {
  int rc = mallctl(name.c_str(), nullptr, nullptr, nullptr, 0);
  return MakeErrorCode(rc);
}

namespace cmd {

namespace {
void MallocStatPrintCb(void* data, const char* msg) {
  auto* s = static_cast<std::string*>(data);
  *s += msg;
}
}  // namespace

std::string Stats() {
  std::string result;
  malloc_stats_print(&MallocStatPrintCb, &result, nullptr);
  return result;
}

std::error_code ProfActivate() {
  bool active = true;
  return MallCtlBool("prof.active", active);
}

std::error_code ProfDeactivate() {
  bool active = false;
  return MallCtlBool("prof.active", active);
}

std::error_code ProfDump() { return MallCtl("prof.dump"); }

}  // namespace cmd
}  // namespace utils::jemalloc