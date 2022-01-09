#ifndef SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_
#define SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_

#include <string>

namespace scom {

void InitProtobuf();

bool WriteMessage(
    unsigned int version,
    const std::string &message,
    std::ostream *ostream);

bool ReadMessage(
    unsigned int *version,
    std::string *message,
    std::istream *istream);

void ShutdownProtobufLibrary();

}  // namespace scom

#endif