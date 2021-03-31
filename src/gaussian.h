#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

using namespace hls;
typedef ap_axis<32, 0, 0, 0> AP_AXIS;
typedef stream<AP_AXIS> AXI_STREAM;

const int height = 512; // 画像の高さ
const int width = 512; // 画像の幅

const int d = 3; // フィルタ直径 (奇数のみ)
const int r = d/2; // フィルタの半径
const int coeff[d][d] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}}; // ガウシアンフィルタの係数 (を shift ビット左シフトしたもの)
const int shift = 4; // 係数を何ビット左シフトしたか

// 座標 (x, y) が画像の範囲内かどうかを返す
inline bool bounds_ok(int y, int x) {
    return (0 <= y && y < height && 0 <= x && x < width);
}

// x を右に s ビットシフトする (繰り上げがあれば繰り上げる)
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