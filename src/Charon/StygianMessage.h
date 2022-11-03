#ifndef STYGIAN_MESSAGE_H
#define STYGIAN_MESSAGE_H

#include <vector>

namespace Charon {

	//Starting point for the data we want to transit. I don't exactly
	//remember what the data from the ring buffer actually looks like,
	//so for now I assume it's a raw byte array (here wrapped in std::vector)
	//Might need to write utils to convert ring buffer data to raw bytes.

	struct StygianMessage
	{
		StygianMessage() = default;
		StygianMessage(const std::vector<uint8_t>& data)
		{
			size = data.size() * sizeof(uint8_t);
			body = data;
		}


		uint64_t size = 0;
		std::vector<uint8_t> body;
	};
}


#endif