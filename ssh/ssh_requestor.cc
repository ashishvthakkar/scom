#include "ssh_requestor.h"

namespace scom {

SshRequestor::SshRequestor(
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : ssh_initator_(SshInitiator(host, user, command)),
      request_id_(0) {}

std::string SshRequestor::SendReceive(const std::string& request) {
  scom::HeaderWriteFields header{
      .version = kProtocolVersion,
      .request_id = request_id_,
      .payload = request};
  scom::WriteMessage(header, buffer_);
  LOG(INFO) << "Sending pb request with size: " << buffer_.size();
  request_id_++;
  ssh_initator_.Send(buffer_);
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

std::vector<char> SshRequestor::SendReceive(const std::vector<char>& request) {
  std::string request_str(request.begin(), request.end());
  auto response_payload = SendReceive(request_str);
  std::vector<char> payload(response_payload.begin(), response_payload.end());
  // Relying on RVO instead of using std::move
  return payload;
}

}  // namespace scom