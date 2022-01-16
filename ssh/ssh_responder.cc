#include "ssh_responder.h"

#include <config.h>

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

int SshResponder::ReadNextMessageSize() {
  std::string buffer;
  buffer.resize(kSizeOfMsgLen);
  LOG_ASSERT(sizeof(int) == kSizeOfMsgLen);  // << "Unexpected size mismatch";
  auto bytes_read = ReadNextMessage(buffer);
  CHECK(bytes_read == kSizeOfMsgLen);
  // if (std::feof(stdin) != 0) {
  //   return 0;
  // }
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  int next_message_size = *(reinterpret_cast<int*>(buffer.data()));
  CHECK(bytes_read == sizeof(next_message_size)) << "Error reading size";
  log_file_ << "Read next message size: " << next_message_size;
  return next_message_size;
}

int SshResponder::ReadNextMessage(std::string& buffer) {
  if (buffer.empty()) {
    return 0;
  }
  auto bytes_read =
      std::fread(buffer.data(), sizeof(buffer[0]), buffer.size(), stdin);
  CHECK(!std::ferror(stdin)) << "Error reading from stdin";
  log_file_ << "Read next message with size: " << bytes_read;
  return bytes_read;
}

void SshResponder::WriteOutputMessage(std::string& output) {
  if (output.empty()) {
    return;
  }
  log_file_ << "Writing response with size: " << output.size();
  std::fwrite(output.data(), sizeof(output[0]), output.size(), stdout);
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
  WriteOutputMessage(protobuf_response);
  // std::cout << protobuf_response;
  log_file_ << "Sent pb response with size: " << protobuf_response.size()
            << std::endl;
  log_file_ << "Sent pb response: " << protobuf_response << std::endl;
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
