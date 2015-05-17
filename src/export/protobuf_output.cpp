#include "export/protobuf_output.hpp"

namespace ct {

	ProtobufOutput::ProtobufOutput(std::FILE *file)
		: backendStream(fileToDescriptor(file)),
		output(&backendStream) {
	}

	int ProtobufOutput::fileToDescriptor(std::FILE *file) {
#if _MSC_VER
		return ::_fileno(file);
#else
		return ::fileno(file);
#endif //_MSC_VER
	}

}