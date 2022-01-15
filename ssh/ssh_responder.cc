#include "ssh_responder.h"

SshResponder::SshResponder(const std::string& log_file_name) {
  // NOTE: No std::ios_base::app is used
  log_file_.open(log_file_name);
  // std::string expression("5+2");
  // log_file_ << expression
  //           << " computes to: " << code_experiments::Compute(expression);
}

void SshResponder::ProcessInput(const std::string& input) {
  log_file_ << "Received input: " << input << std::endl;
  int version = 0;
  int request_id = 0;
  std::string details;
  scom::ReadMessage(input, version, request_id, details);
  CHECK(version == kProtocolVersion) << "Unexpected version: " << version;
  log_file_ << "Read message with version " << version << ", request id "
            << request_id << " and details: " << std::endl;
  log_file_ << details << std::endl;
  auto protobuf_response =
      ConstructProtobufResponse(version, request_id, details);
  std::cout << protobuf_response;
  log_file_ << "Sent protobuf response: " << protobuf_response << std::endl;
  log_file_ << "Done with request " << request_id << "." << std::endl;
}

std::string SshResponder::ConstructResponse(
    const std::string& request_payload) {
  std::string response = "Response for " + request_payload;
  log_file_ << "Created resonse: " << response << std::endl;
  return response;
}

std::string SshResponder::ConstructProtobufResponse(
    int version,
    int request_id,
    const std::string& request_payload) {
  std::string response = ConstructResponse(request_payload);
  std::string protobuf_response;
  scom::WriteMessage(version, request_id, response, protobuf_response);
  log_file_ << "Protobuf output: " << protobuf_response << std::endl;
  return protobuf_response;
}
