#include "protocol_adapter.h"

#include <glog/logging.h>
#include <scom/spec.pb.h>

namespace scom {

void InitProtobuf() { GOOGLE_PROTOBUF_VERIFY_VERSION; }

bool WriteMessage(int version, const std::string &message, std::ostream &out) {
  scom::Header header;
  header.set_version(version);
  header.set_payload(message);
  return header.SerializeToOstream(&out);
}

bool WriteMessage(int version, const std::string &message, std::string &out) {
  scom::Header header;
  header.set_version(version);
  header.set_payload(message);
  return header.SerializeToString(&out);
}

bool ReadMessage(int &version, std::string &message, std::istream &in) {
  scom::Header header;
  auto result = header.ParseFromIstream(&in);
  if (!result)
    return result;
  version = header.version();
  message = header.payload();
  return true;
}

bool ReadMessage(int &version, std::string &message, const std::string &in) {
  scom::Header header;
  auto result = header.ParseFromString(in);
  if (!result)
    return result;
  version = header.version();
  message = header.payload();
  return true;
}

void ShutdownProtobufLibrary() { google::protobuf::ShutdownProtobufLibrary(); }

}  // namespace scom