#include "openexr/openexr-c.h"
#include "tinyexr/tinyexr.h"

#include <stdio.h>
#include <stdlib.h>

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
        printf("FAILED to read openexr %s: %i\n", filepath, res);
        return false;
    }
    
    printf("- open %ix%i data y %i..%i ch %i type %i\n", img.width, img.height, img.dataWindowMinY, img.dataWindowMaxY, img.channelCount, img.pixelType);
    
    nanoexr_release_image_data(&img);
    return true;
}

static bool read_tinyexr(const char *filepath)
{
    float* out = nullptr;
    int width;
    int height;
    const char* err = nullptr;
    int ret = LoadEXR(&out, &width, &height, filepath, &err);
    if (ret != TINYEXR_SUCCESS) {
        printf("FAILED to read tinyexr %s: %s\n", filepath, err);
        FreeEXRErrorMessage(err);
        return false;
    }
    
    printf("- open %ix%i\n", width, height);
    
    free(out);
    return true;
}

int main(int argc, const char** argv)
{
    if (argc < 2) {
        printf("USAGE: test_exrcore_tinyexr <exrfile> ...\n");
        return 1;
    }
    
    for (int i = 1; i < argc; ++i) {
        printf("Reading %s:\n", argv[i]);
        read_openexr(argv[i]);
        read_tinyexr(argv[i]);
    }
	return 0;
}
