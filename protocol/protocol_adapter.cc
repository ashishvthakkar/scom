#include "protocol_adapter.h"

#include <glog/logging.h>
#include <scom/spec.pb.h>

namespace scom {

void InitProtobuf() { GOOGLE_PROTOBUF_VERIFY_VERSION; }

bool WriteMessage(
    int version,
    int request_id,
    const std::string &message,
    std::ostream &out) {
  scom::Header header;
  header.set_version(version);
  header.set_request_id(request_id);
  header.set_payload(message);
  return header.SerializeToOstream(&out);
}

bool WriteMessage(
    int version,
    int request_id,
    const std::string &message,
    std::string &out) {
  scom::Header header;
  header.set_version(version);
  header.set_request_id(request_id);
  header.set_payload(message);
  return header.SerializeToString(&out);
}

bool ReadMessage(
    std::istream &in,
    int &version,
    int &request_id,
    std::string &payload) {
  scom::Header header;
  auto result = header.ParseFromIstream(&in);
  if (!result) {
    return result;
  }
  version = header.version();
  payload = header.payload();
  request_id = header.request_id();
  return true;
}

bool ReadMessage(
    const std::string &in,
    int &version,
    int &request_id,
    std::string &payload) {
  scom::Header header;
  auto result = header.ParseFromString(in);
  if (!result) {
    return result;
  }
  version = header.version();
  request_id = header.request_id();
  payload = header.payload();
  return true;
}

void ShutdownProtobufLibrary() { google::protobuf::ShutdownProtobufLibrary(); }

}  // namespace scom