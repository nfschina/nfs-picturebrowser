#define MINIZ_NO_STDIO
#define MINIZ_NO_ARCHIVE_APIS
#define MINIZ_NO_TIME
#define MINIZ_NO_ZLIB_APIS

extern "C" {
#include "nfspbMiniz.c"
}

#include "nfspbPsd.h"
#include "psd2png.h"


QString psd2png(QString &fileName,QString &desName)
{
    psd::psd img(std::ifstream(fileName.toStdString(), std::ios::binary));
    if (!img)
    {
        std::cerr << "cannot open .psd file" << std::endl;
        return fileName;
    }
    if (img.header.bit_depth == 1)
    {
        std::cerr << "unsupported bit depth: " << img.header.bit_depth << std::endl;
        return fileName;
    }
    std::vector<char> merged;
    uint32_t bytes_per_pixel = img.header.bit_depth/8;
    std::cout << bytes_per_pixel << std::endl;
	std::cout << std::endl;
    for(int y = 0; y < img.header.height; y++)
    {
        for(int x = 0; x < img.header.width; x++)
        {
            for(int ch = 0; ch < img.header.num_channels; ch++)
            {

                merged.push_back(img.merged_image.datas[ch][y][x*bytes_per_pixel + bytes_per_pixel-1]);
            }
        }
    }
    size_t png_size;
    std::cout << merged.size() << std::endl;
    void* buffer = tdefl_write_image_to_png_file_in_memory(merged.data(), img.header.width, img.header.height, img.header.num_channels, &png_size);
    std::cout << png_size << std::endl;
    std::ofstream outf(desName.toStdString().c_str(), std::ios::binary);
    if (!outf)
    {
        std::cerr << "cannot open output file" << std::endl;
        return fileName;
    }
    outf.write((char*)buffer, png_size);
    outf.close();

    return desName;
}
