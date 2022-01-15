#include <config.h>
#include <gflags/gflags.h>

#include "../protocol/protocol_adapter.h"
#include "ssh_initiator.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_responder]");

class SshRequestor {
private:
  bool SshResponseHandler(
      const char* buffer,
      int bytes_read,
      ssh::Channel& channel) {
    if (bytes_read > 0) {
      int version = 0;
      int request_id = 0;
      std::string payload;
      scom::ReadMessage(buffer, version, request_id, payload);
      CHECK(version == kProtocolVersion) << "Unexpected version: " << version;
      LOG(INFO) << "Got payload: " << payload;
    }
    if (next_request_id_ >= requests_.size()) {
      return true;  // done
    }
    LOG(INFO) << "Sending request: " << requests_.at(next_request_id_);
    std::string protobuf_request;
    scom::WriteMessage(
        kProtocolVersion,
        next_request_id_,
        requests_.at(next_request_id_),
        protobuf_request);
    protobuf_request.append("\n");
    channel.write(protobuf_request.data(), protobuf_request.size());
    next_request_id_++;
    return false;  // more responses and/or requests to follow
  }

public:
  explicit SshRequestor(
      const std::vector<std::string>& requests,
      const std::string& host,
      const std::string& user,
      const std::string& command)
      : requests_(requests),
        next_request_id_(0) {
    SshInitiator(
        host,
        user,
        command,
        [this](const char* buffer, int bytes_read, ssh::Channel& channel) {
          return SshResponseHandler(buffer, bytes_read, channel);
        });
  }

private:
  const std::vector<std::string>& requests_;
  int next_request_id_;
};

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::vector<std::string> requests{"Request1", "Request2"};
  // TODO(ashish): Remove as part of no_except change
  try {
    SshRequestor requestor(requests, FLAGS_host, FLAGS_user, FLAGS_command);
  } catch (...) {
    LOG(ERROR) << "Error executing ssh demo.";
    return -1;
  }
  return 0;
}