#include "ssh_responder.h"

#include <config.h>

#include <cstdio>

#include "../expressions/utils.h"

namespace scom {

SshResponder::SshResponder(const std::string& log_file_name) {
  log_file_.open(log_file_name);
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  std::freopen(nullptr, "rb", stdin);
  CHECK(!std::ferror(stdin)) << "Could not reopen stdin in binary mode";
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  std::freopen(nullptr, "wb", stdout);
  CHECK(!std::ferror(stdin)) << "Could not reopen stdout in binary mode";
}

int SshResponder::GetNextMessageSize() {
  int message_size = 0;
  LOG_ASSERT(sizeof(message_size) == kSizeOfMsgLen)
      << "Unexpected size mismatch";
  auto bytes_read = Read(&message_size, sizeof(message_size));
  CHECK(bytes_read == sizeof(message_size)) << "Error reading size";
  log_file_ << "Read next message size: " << message_size << std::endl;
  return message_size;
}

int SshResponder::Read(void* buffer, int buffer_size) {
  if (buffer_size <= 0) {
    return 0;
  }
  auto bytes_read = std::fread(buffer, 1, buffer_size, stdin);
  CHECK(!std::ferror(stdin)) << "Error reading from stdin";
  log_file_ << "Read size: " << bytes_read << std::endl;
  LOG_ASSERT(bytes_read <= buffer_size)
      << "Potential buffer overflow when reading";
  return bytes_read;
}

void SshResponder::HandleNextMessage(int message_size) {
  buffer_.resize(message_size);
  auto bytes_read = Read(buffer_.data(), buffer_.size());
  CHECK(bytes_read == message_size) << "Read incomplete message";
  ProcessMessage();
}

void SshResponder::ProcessMessage() {
  log_file_ << "Received message with size: " << buffer_.size() << std::endl;
  log_file_ << "Received message: " << buffer_ << std::endl;
  scom::HeaderReadFields header;
  scom::ReadMessage(buffer_, header);
  CHECK(header.version == kProtocolVersion)
      << "Unexpected version: " << header.version;
  log_file_ << "Read message with version " << header.version << ", request id "
            << header.request_id << " and details: " << std::endl;
  log_file_ << header.payload << std::endl;
  ConstructProtobufResponse(header.version, header.request_id, header.payload);
  Send(buffer_);
  log_file_ << "Sent pb response with size: " << buffer_.size() << std::endl;
  log_file_ << "Sent pb response: " << buffer_ << std::endl;
  log_file_ << "Done with request " << header.request_id << "." << std::endl;
}

std::string SshResponder::ConstructResponse(
    const std::string& request_payload) {
  int result = code_experiments::Compute(request_payload);
  std::string response = std::to_string(result);
  log_file_ << "Created resonse: " << response << std::endl;
  return response;
}

void SshResponder::ConstructProtobufResponse(
    int version,
    int request_id,
    const std::string& request_payload) {
  std::string response = ConstructResponse(request_payload);
  scom::HeaderWriteFields header{
      .version = version,
      .request_id = request_id,
      .payload = response};
  scom::WriteMessage(header, buffer_);
  log_file_ << "Protobuf output: " << buffer_ << std::endl;
}

void SshResponder::Send(std::string& output) {
  if (output.empty()) {
    return;
  }
  int size = output.size();
  log_file_ << "Writing response size: " << size << std::endl;
  std::fwrite(&size, sizeof(size), 1, stdout);
  log_file_ << "Writing response: " << output << std::endl;
  std::fwrite(output.data(), sizeof(output[0]), output.size(), stdout);
  std::fflush(stdout);
}

}  // namespace scom