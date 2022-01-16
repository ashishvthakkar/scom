#include "ssh_responder.h"

#include <cstdio>

SshResponder::SshResponder(const std::string& log_file_name) {
  // NOTE: No std::ios_base::app is used
  log_file_.open(log_file_name);
  // std::string expression("5+2");
  // log_file_ << expression
  //           << " computes to: " << code_experiments::Compute(expression);
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  std::freopen(nullptr, "rb", stdin);
  CHECK(!std::ferror(stdin)) << "Could not reopen stdin in binary mode";
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  std::freopen(nullptr, "wb", stdout);
  CHECK(!std::ferror(stdin)) << "Could not reopen stdout in binary mode";
}

int SshResponder::ReadNextMessage(std::string& buffer) {
  if (buffer.empty()) {
    return 0;
  }
  auto bytes_read =
      std::fread(buffer.data(), sizeof(buffer[0]), buffer.size(), stdin);
  CHECK(!std::ferror(stdin)) << "Error reading from stdin";
  log_file_ << "Read next message with size: " << bytes_read;
  buffer.resize(
      bytes_read - 1);  // TODO(ashish): This is for the terminating \n
  return bytes_read;
}

void SshResponder::ProcessInput(const std::string& input) {
  log_file_ << "Received input with size: " << input.size() << std::endl;
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
  std::cout << protobuf_response << std::endl;
  log_file_ << "Sent protobuf response with size: " << protobuf_response.size()
            << std::endl;
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
