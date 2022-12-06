#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b) {
	return (b<<16) + (g<<8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b) {
	r = (color >>  0) & 255;
	g = (color >>  8) & 255;
	b = (color >> 16) & 255;
}

void write_ppm(const std::string filename, const std::vector<uint32_t> &image,
		const size_t w, const size_t h) {
	assert(image.size() == w*h);
	std::ofstream ofs(filename, std::ios::binary);
	ofs << "P6\n" << w << "\n" << h << "\n255\n";
	for(size_t i=0; i < w*h; ++i) {
		uint8_t r, g, b;
		unpack_color(image[i], r, g, b);
		ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
	}
	ofs.close();
}

int main() {
	const size_t win_w = 512;
	const size_t win_h = 512;
	std::vector<uint32_t> framebuffer(win_w*win_h, 255);

	for(size_t j=0; j<win_h; j++) {
		for(size_t i=0; i<win_w; i++) {
			uint8_t r = 255*j/float(win_h);
			uint8_t g = 255*i/float(win_w);
			uint8_t b = 0;
			framebuffer[win_w*j+i] = pack_color(r, g, b);
		}
	}

	write_ppm("./out.ppm", framebuffer, win_w, win_h);
	return 0;
}
