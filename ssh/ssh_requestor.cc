#include "ssh_requestor.h"

#include <glog/logging.h>

#include "../protocol/protocol_adapter.h"

namespace scom {

SshRequestor::SshRequestor(
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : ssh_initator_(SshInitiator(host, user, command)),
      request_id_(0) {}

void SshRequestor::Send(const std::string& message) {
  scom::HeaderWriteFields header{
      .version = kProtocolVersion,
      .request_id = request_id_,
      .payload = message};
  scom::WriteMessage(header, buffer_);
  request_id_++;
  ssh_initator_.Send(buffer_);
}

std::string SshRequestor::Receive() {
  ssh_initator_.Receive(buffer_);
  scom::HeaderReadFields header_read;
  scom::ReadMessage(buffer_, header_read);
  CHECK(header_read.version == kProtocolVersion)
      << "Unexpected version: " << header_read.version;
  return std::move(header_read.payload);
}

std::string SshRequestor::SendReceive(const std::string& message) {
  Send(message);
  return Receive();
}

}  // namespace scom