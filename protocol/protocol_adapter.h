#ifndef SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_
#define SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_

#include <string>

namespace scom {

void InitProtobuf();

bool WriteMessage(
    unsigned int version,
    const std::string &message,
    std::ostream *ostream);

bool WriteMessage(
    unsigned int version,
    const std::string &message,
    std::string *out_string);

bool ReadMessage(
    unsigned int *version,
    std::string *message,
    std::istream *istream);

bool ReadMessage(
    unsigned int *version,
    std::string *message,
    const std::string &in_string);

void ShutdownProtobufLibrary();

}  // namespace scom

#endif