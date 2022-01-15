#ifndef SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_
#define SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_

#include <string>

namespace scom {

void InitProtobuf();

bool WriteMessage(
    int version,
    int request_id,
    const std::string &payload,
    std::ostream &out);

bool WriteMessage(
    int version,
    int request_id,
    const std::string &payload,
    std::string &out);

bool ReadMessage(
    std::istream &in,
    int &version,
    int &request_id,
    std::string &payload);

bool ReadMessage(
    const std::string &in,
    int &version,
    int &request_id,
    std::string &payload);

void ShutdownProtobufLibrary();

}  // namespace scom

#endif