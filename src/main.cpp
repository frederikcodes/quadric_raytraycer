#include <fstream>
#include <iostream>

int main() {
    const int width = 800;
    const int height = 600;

    std::ofstream image("images/output.ppm");

    if (!image) {
        std::cerr << "Could not open output file.\n";
        return 1;
    }

    image << "P3\n";
    image << width << " " << height << "\n";
    image << "255\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r = static_cast<int>(255.0 * x / width);
            int g = static_cast<int>(255.0 * y / height);
            int b = 80;

            image << r << " " << g << " " << b << "\n";
        }
    }

    std::cout << "Rendered images/output.ppm\n";
    return 0;
}