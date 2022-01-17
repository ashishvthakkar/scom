#ifndef SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_
#define SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_

#include <string>

namespace scom {

struct HeaderReadFields {
  int version;
  int request_id;
  std::string payload;
};

struct HeaderWriteFields {
  int version;
  int request_id;
  const std::string &payload;
};

void InitProtobuf();
// NOTE: The below functions are for convenience only and it is possible to call
// into protobuf calls directly from the using files if the difference in
// performance is important.
bool WriteMessage(
    const HeaderWriteFields &header_write_fields,
    std::ostream &out);
bool WriteMessage(
    const HeaderWriteFields &header_write_fields,
    std::string &out);
bool ReadMessage(std::istream &in, HeaderReadFields &header_read_fields);
bool ReadMessage(const std::string &in, HeaderReadFields &header_read_fields);
void ShutdownProtobufLibrary();

}  // namespace scom

#endif