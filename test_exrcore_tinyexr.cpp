
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>

inline std::chrono::high_resolution_clock::time_point time_now()
{
    return std::chrono::high_resolution_clock::now();
}

inline float time_duration_ms(std::chrono::high_resolution_clock::time_point t0)
{
    std::chrono::duration<float, std::milli> dt = std::chrono::high_resolution_clock::now() - t0;
    return dt.count();
}

static uint8_t* really_bad_downsample(int src_width, int src_height, int channels, bool is_half, const void* src, int &dst_width, int &dst_height)
{
    const uint8_t* src_data = (const uint8_t*)src;

    const int downsampling = 4;
    dst_width = src_width / downsampling;
    dst_height = src_height / downsampling;
    
    const size_t pixel_size = (is_half ? 2 : 4) * channels;
    uint8_t* dst_data = (uint8_t*)malloc(dst_width * dst_height * pixel_size);
    
    uint8_t* dst_ptr = dst_data;
    for (int dst_y = 0; dst_y < dst_height; ++dst_y) {
        const uint8_t* src_ptr = src_data + (dst_y * downsampling) * src_width * pixel_size;
        for (int dst_x = 0; dst_x < dst_width; ++dst_x) {
            memcpy(dst_ptr, src_ptr, pixel_size);
            src_ptr += downsampling * pixel_size;
            dst_ptr += pixel_size;
        }
    }
    
    return dst_data;
}

#ifdef USE_OPENEXR
#include "openexr/openexr-c.h"

static bool read_openexr(const char *filepath)
{
    printf("- open ");
    auto t0 = time_now();
    nanoexr_ImageData_t img = {};
    exr_result_t res = nanoexr_read_exr(
                                        filepath,
                                        nullptr,
                                        nullptr,
                                        &img,
                                        nullptr,
                                        4,
                                        0,
                                        0);
    float read_ms = time_duration_ms(t0);
    if (res != EXR_ERR_SUCCESS) {
        printf("FAILED %s (#%i)\n", nanoexr_get_default_error_message(res), res);
        return false;
    }
    
    printf("%ix%i %.1fms data y %i..%i ch %i type %i\n", img.width, img.height, read_ms, img.dataWindowMinY, img.dataWindowMaxY, img.channelCount, img.pixelType);
    
    int thumb_width, thumb_height;
    uint8_t *thumb_data = really_bad_downsample(img.width, img.height, img.channelCount, img.pixelType == EXR_PIXEL_HALF, img.data, thumb_width, thumb_height);
    
    std::string dst_filepath = filepath;
    dst_filepath.resize(dst_filepath.size() - 4);
    dst_filepath += "_open.exr";
    
    t0 = time_now();
    int comp_stride = img.pixelType == EXR_PIXEL_HALF ? 2 : 4;
    int pix_stride = img.channelCount * comp_stride;
    int line_stride = pix_stride * thumb_width;
    res = nanoexr_write_exr(dst_filepath.c_str(), nullptr, nullptr,
                            thumb_width, thumb_height, false,
                            img.pixelType,
                            thumb_data + comp_stride * 0, pix_stride, line_stride,
                            thumb_data + comp_stride * 1, pix_stride, line_stride,
                            thumb_data + comp_stride * 2, pix_stride, line_stride,
                            thumb_data + comp_stride * 3, pix_stride, line_stride);
    float write_ms = time_duration_ms(t0);
    if (res != EXR_ERR_SUCCESS) {
        printf("FAILED to write openexr %s: %i\n", dst_filepath.c_str(), res);
        return false;
    }
    free(thumb_data);

    nanoexr_release_image_data(&img);
    return true;
}
#endif // USE_OPENEXR

#ifdef USE_TINYEXR
#include "tinyexr/tinyexr.h"

static bool read_tinyexr(const char *filepath)
{
    printf("- tiny ");
    auto t0 = time_now();
    float* img = nullptr;
    int width;
    int height;
    const char* err = nullptr;
    int ret = LoadEXR(&img, &width, &height, filepath, &err);
    float read_ms = time_duration_ms(t0);
    if (ret != TINYEXR_SUCCESS) {
        printf("FAILED %s\n", err);
        FreeEXRErrorMessage(err);
        return false;
    }
    
    printf("%ix%i %.1fms\n", width, height, read_ms);
    
    int thumb_width, thumb_height;
    uint8_t *thumb_data = really_bad_downsample(width, height, 4, false, img, thumb_width, thumb_height);
    
    std::string dst_filepath = filepath;
    dst_filepath.resize(dst_filepath.size() - 4);
    dst_filepath += "_tiny.exr";

    err = nullptr;
    ret = SaveEXR((const float*)thumb_data, thumb_width, thumb_height, 4, true, dst_filepath.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        printf("FAILED to write tinyexr %s: %s\n", dst_filepath.c_str(), err);
        FreeEXRErrorMessage(err);
        return false;
    }

    free(thumb_data);
    free(img);
    return true;
}
#endif // USE_TINYEXR

int main(int argc, const char** argv)
{
    if (argc < 2) {
        printf("USAGE: test_exrcore_tinyexr <exrfile> ...\n");
        return 1;
    }
    
    for (int i = 1; i < argc; ++i) {
        printf("Reading %s:\n", argv[i]);
#ifdef USE_OPENEXR
        read_openexr(argv[i]);
#endif
#ifdef USE_TINYEXR
        read_tinyexr(argv[i]);
#endif
    }
	return 0;
}
