#include <iostream>

#include "dex/invoke_type.h"

// This was automatically generated by art/tools/generate_operator_out.py --- do not edit!
namespace art {
std::ostream& operator<<(std::ostream& os, InvokeType rhs) {
  switch (rhs) {
    case kStatic: os << "static"; break;
    case kDirect: os << "direct"; break;
    case kVirtual: os << "virtual"; break;
    case kSuper: os << "super"; break;
    case kInterface: os << "interface"; break;
    case kPolymorphic: os << "polymorphic"; break;
    case kCustom: os << "custom"; break;
    default: os << "InvokeType[" << static_cast<int>(rhs) << "]"; break;
  }
  return os;
}
}  // namespace art

