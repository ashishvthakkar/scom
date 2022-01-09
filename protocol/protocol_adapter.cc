#include "protocol_adapter.h"

#include <glog/logging.h>
#include <scom/spec.pb.h>

namespace scom {

void InitProtobuf() { GOOGLE_PROTOBUF_VERIFY_VERSION; }

bool WriteMessage(
    unsigned int version,
    const std::string &message,
    std::ostream *ostream) {
  scom::Header header;
  header.set_version(version);
  header.set_details(message);
  return header.SerializeToOstream(ostream);
}

bool ReadMessage(
    unsigned int *version,
    std::string *message,
    std::istream *istream) {
  scom::Header header;
  auto result = header.ParseFromIstream(istream);
  if (!result) return result;
  *version = header.version();
  *message = header.details();
  return true;
}

void ShutdownProtobufLibrary() { google::protobuf::ShutdownProtobufLibrary(); }

}  // namespace scom