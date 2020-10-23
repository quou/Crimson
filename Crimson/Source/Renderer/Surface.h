#pragma once

namespace Crimson {
	struct Surface {
		int width;
		int height;
		int componentCount;
		unsigned char* pixels;
		unsigned int id;
	};
}
