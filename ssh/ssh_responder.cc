#include <config.h>
#include <glog/logging.h>

#include <fstream>
#include <iostream>

#include "../protocol/protocol_adapter.h"

class SshResponder {
public:
  explicit SshResponder(const std::string& log_file_name) {
    // NOTE: No std::ios_base::app is used
    log_file_.open(log_file_name);
  }

  void ProcessInput(const std::string& input) {
    log_file_ << "Received input with size: " << input.size() << std::endl;
    log_file_ << "Received input: " << input << std::endl;
    int version = 0;
    int request_id = 0;
    std::string details;
    scom::ReadMessage(input, version, request_id, details);
    CHECK(version == kProtocolVersion) << "Unexpected version: " << version;
    log_file_ << "Read message with version " << version
              << " and details: " << std::endl;
    log_file_ << details << std::endl;
    auto protobuf_response =
        ConstructProtobufResponse(version, request_id, details);
    std::cout << protobuf_response;
    log_file_ << "Sent protobuf response: " << protobuf_response << std::endl;
    // TODO(ashish): Add request id
    log_file_ << "Done with request." << std::endl;
  }

private:
  std::string ConstructResponse(const std::string& request_payload) {
    std::string response = "Response for " + request_payload;
    log_file_ << "Created resonse: " << response << std::endl;
    return response;
  }

  std::string ConstructProtobufResponse(
      int version,
      int request_id,
      const std::string& request_payload) {
    std::string response = ConstructResponse(request_payload);
    std::string protobuf_response;
    scom::WriteMessage(version, request_id, response, protobuf_response);
    log_file_ << "Protobuf output: " << protobuf_response << std::endl;
    return protobuf_response;
  }

  std::ofstream log_file_;
};

int main() {
  SshResponder ssh_responder(kResponderLog);
  std::string input;
  while (std::cin >> input) {
    ssh_responder.ProcessInput(input);
  }
}