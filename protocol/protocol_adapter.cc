#include "protocol_adapter.h"

#include <glog/logging.h>
#include <scom/spec.pb.h>

namespace scom {

void InitProtobuf() { GOOGLE_PROTOBUF_VERIFY_VERSION; }

static scom::Header HeaderFromFields(
    const HeaderWriteFields &header_write_fields) {
  scom::Header header;
  header.set_version(header_write_fields.version);
  header.set_request_id(header_write_fields.request_id);
  header.set_payload(header_write_fields.payload);
  return header;
}

static void FieldsFromHeader(
    const scom::Header &header,
    HeaderReadFields &header_read_fields) {
  header_read_fields.version = header.version();
  header_read_fields.request_id = header.request_id();
  header_read_fields.payload = header.payload();
}

bool WriteMessage(
    const HeaderWriteFields &header_write_fields,
    std::ostream &out) {
  return HeaderFromFields(header_write_fields).SerializeToOstream(&out);
}

bool WriteMessage(
    const HeaderWriteFields &header_write_fields,
    std::string &out) {
  return HeaderFromFields(header_write_fields).SerializeToString(&out);
}

bool ReadMessage(std::istream &in, HeaderReadFields &header_read_fields) {
  scom::Header header;
  auto result = header.ParseFromIstream(&in);
  if (!result) {
    return result;
  }
  FieldsFromHeader(header, header_read_fields);
  return true;
}

bool ReadMessage(const std::string &in, HeaderReadFields &header_read_fields) {
  scom::Header header;
  auto result = header.ParseFromString(in);
  if (!result) {
    return result;
  }
  FieldsFromHeader(header, header_read_fields);
  return true;
}

void ShutdownProtobufLibrary() { google::protobuf::ShutdownProtobufLibrary(); }

}  // namespace scom