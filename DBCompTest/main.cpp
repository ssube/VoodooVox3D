
#include <time.h>

#define FILEMANAGER_API
#include "WorldLoader.hpp"

int main(int argc, char * argv[])
{
    WorldLoader * wl = new WorldLoader("test_world.db");
    time_t now = time(NULL);
    srand(now);

    for ( size_t pass = 0; pass < 16; pass++ )
    {
        unsigned char input[16384];

        for ( size_t i = 0; i < 16384; ++i ) { input[i] = rand() % 1; }

        size_t compsize = 0;
        void * compressed = wl->Compress(sizeof(input) / sizeof(input[0]), input, &compsize);

        size_t decompsize = 0;
        void * decompressed = wl->Decompress(compsize, compressed, &decompsize);

        if ( decompsize != sizeof(input) / sizeof(input[0]) )
        {
            fprintf(stdout, "Pass %u: Compression test appears to have failed (size difference).\n", pass);
        } else {
            int result = memcmp(input, decompressed, decompsize);
            if ( result != 0 )
            {
                fprintf(stdout, "Pass %u: Compression test appears to have failed (data difference).\n", pass);
            } else {
                fprintf(stdout, "Pass %u: Compression test appears to have succeeded (%u : %u; %f).\n", pass, decompsize, compsize, ( compsize / (float)decompsize));
            }
        }

        free(compressed);
        free(decompressed);
    }

    fprintf(stdout, "Test complete.\n");
}
