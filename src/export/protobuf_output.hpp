#ifndef CPPTOOL_PROTOBUF_OUTPUT_H
#define CPPTOOL_PROTOBUF_OUTPUT_H

#include <cstdio>
#include <type_traits>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message.h>

namespace ct {
	class ProtobufOutput {
		google::protobuf::io::FileOutputStream backendStream;
		google::protobuf::io::CodedOutputStream output;
	public:
		ProtobufOutput(std::FILE *file);

		template<typename MessageLite>
		void writeMessage(typename std::remove_reference<MessageLite>::type &msg);
	private:
		static int fileToDescriptor(std::FILE *file);
	};

	template<typename MessageLite>
	void ProtobufOutput::writeMessage(typename std::remove_reference<MessageLite>::type &msg) {
		output.WriteVarint32(msg.ByteSize());
		msg.SerializeToCodedStream(&output);
		//TODO: validate return bool
#ifndef NDEBUG
		msg.PrintDebugString();
#endif //NDEBUG
	}
}

#endif //CPPTOOL_PROTOBUF_OUTPUT_H