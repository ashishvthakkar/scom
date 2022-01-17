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
  LOG(INFO) << "Sending pb request with size: " << buffer_.size();
  request_id_++;
  ssh_initator_.Send(buffer_);
}

std::string SshRequestor::ReceiveAsStr() {
  ssh_initator_.Receive(buffer_);
  scom::HeaderReadFields header_read;
  scom::ReadMessage(buffer_, header_read);
  CHECK(header_read.version == kProtocolVersion)
      << "Unexpected version: " << header_read.version;
  LOG(INFO) << "Read message with version " << header_read.version
            << ", request id " << header_read.request_id
            << " and details: " << std::endl;
  LOG(INFO) << header_read.payload << std::endl;
  return std::move(header_read.payload);
}

std::string SshRequestor::SendReceive(const std::string& message) {
  Send(message);
  return ReceiveAsStr();
}

void SshRequestor::Send(const std::vector<char>& message) {
  std::string request_str(message.begin(), message.end());
  Send(request_str);
}

std::vector<char> SshRequestor::ReceiveAsBuffer() {
  auto response_payload = ReceiveAsStr();
  std::vector<char> payload(response_payload.begin(), response_payload.end());
  // Relying on RVO instead of using std::move
  return payload;
}

std::vector<char> SshRequestor::SendReceive(const std::vector<char>& message) {
  Send(message);
  return ReceiveAsBuffer();
}

}  // namespace scom