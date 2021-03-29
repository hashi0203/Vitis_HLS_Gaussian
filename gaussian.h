#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

using namespace hls;
typedef ap_axis<32, 1, 1, 1> AP_AXIS;
typedef stream<AP_AXIS> AXI_STREAM;

const int height = 512;
const int width = 512;

const int d = 3;
const int r = 1;
const int shift = 4;
const int coeff[d][d] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};

inline bool bounds_ok(int y, int x) {
    return (0 <= y && y < height && 0 <= x && x < width);
}

inline int bitshift(int x, int s) {
    int bit = (1<<(s-1));
    if (x & bit) {
        x >>= s;
        x++;
    } else {
        x >>= s;
    }
    return x;
}