#ifndef SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_
#define SCOM_PROTOCOL_PROTOCOL_ADAPTER_H_

#include <string>

namespace scom {

void InitProtobuf();

bool WriteMessage(int version, const std::string &message, std::ostream &out);

bool WriteMessage(int version, const std::string &message, std::string &out);

bool ReadMessage(int &version, std::string &message, std::istream &in);

bool ReadMessage(int &version, std::string &message, const std::string &in);

void ShutdownProtobufLibrary();

}  // namespace scom

#endif