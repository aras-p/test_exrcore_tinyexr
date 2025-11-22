
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

static uint8_t* really_bad_downsample(int src_width, int src_height, int channels, bool is_half, const void* src, int &dst_width, int &dst_height)
{
    const uint8_t* src_data = (const uint8_t*)src;

    const int downsampling = 2;
    dst_width = src_width / downsampling;
    dst_height = src_height / downsampling;
    
    const size_t pixel_size = (is_half ? 2 : 4) * channels;
    uint8_t* dst_data = (uint8_t*)malloc(dst_width * dst_height * pixel_size);
    if (dst_data == nullptr)
        return nullptr;
    
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
    if (res != EXR_ERR_SUCCESS) {
        printf("FAILED %s (#%i)\n", nanoexr_get_default_error_message(res), res);
        return false;
    }
    
    printf("- open %ix%i ch %i type %i\n", img.width, img.height, img.channelCount, img.pixelType);
    
    int thumb_width, thumb_height;
    uint8_t *thumb_data = really_bad_downsample(img.width, img.height, img.channelCount, img.pixelType == EXR_PIXEL_HALF, img.data, thumb_width, thumb_height);
    
    std::string dst_filepath = filepath;
    dst_filepath.resize(dst_filepath.size() - 4);
    dst_filepath += "_open.exr";
    
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
    float* img = nullptr;
    int width;
    int height;
    const char* err = nullptr;
    int ret = LoadEXR(&img, &width, &height, filepath, &err);
    if (ret != TINYEXR_SUCCESS) {
        printf("FAILED %s\n", err);
        FreeEXRErrorMessage(err);
        return false;
    }
    
    printf("- tiny %ix%i\n", width, height);
    
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
        printf("USAGE: test_exrcore_tinyexr [--openexr] [--tinyexr] <exrfile> ...\n");
        return 1;
    }

    bool do_openexr = false;
    bool do_tinyexr = false;
    int arg_index = 1;
    while (strstr(argv[arg_index], "--") == argv[arg_index]) {
        if (strcmp(argv[arg_index], "--openexr") == 0)
            do_openexr = true;
        if (strcmp(argv[arg_index], "--tinyexr") == 0)
            do_tinyexr = true;
        ++arg_index;
    }
    if (!do_openexr && !do_tinyexr) {
        do_openexr = true;
        do_tinyexr = true;
    }
    
    printf("Input files: %i, openexr=%i tinyexr=%i\n", argc-arg_index, do_openexr, do_tinyexr);
    bool ok = true;
    clock_t t0 = clock();
    for (int i = arg_index; i < argc; ++i) {
        printf("Reading %s:\n", argv[i]);
#ifdef USE_OPENEXR
        if (do_openexr)
            ok &= read_openexr(argv[i]);
#endif
#ifdef USE_TINYEXR
        if (do_tinyexr)
            ok &= read_tinyexr(argv[i]);
#endif
    }
    clock_t t1 = clock();
    printf("Time taken: %.3f sec\n", double(t1-t0) / CLOCKS_PER_SEC);
    if (!ok) {
        printf("There were failures!\n");
        return 1;
    }
	return 0;
}
