#include "gaussian.h"

// メインの関数
void gaussian(AXI_STREAM &in_strm, AXI_STREAM &out_strm) {
// 入出力のインターフェースを定義
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS
#pragma HLS INTERFACE axis port=in_strm
#pragma HLS INTERFACE axis port=out_strm

    // line buffer と window を定義して，パーティションする
    int linebuf[d-1][width];
    int window[d][d];
#pragma HLS ARRAY_PARTITION variable=linebuf complete dim=1
#pragma HLS ARRAY_PARTITION variable=window complete dim=0

    AP_AXIS pix;
    // width * r + r + 1 個入力を読んで，linebuf と window を初期化する
    buf_x1: for (int px = width - r - 1; px < width; px++) {
#pragma HLS PIPELINE
        pix = in_strm.read();
        linebuf[r-1][px] = pix.data;
    }

    buf_y: for (int py = r; py < d - 1; py++) {
        buf_x2: for (int px = 0; px < width; px++) {
#pragma HLS PIPELINE
            pix = in_strm.read();
            linebuf[py][px] = pix.data;
        }
    }

    win_y: for (int wy = r; wy < d; wy++) {
        win_x: for (int wx = r; wx < d; wx++) {
#pragma HLS PIPELINE
            window[wy][wx] = linebuf[wy-1][wx+width-d];
        }
    }

    // pix の値を使って val を初期化する
    // これをしなくてもシミュレーションなどは通るが PYNQ で動かしても値が返って来なくなる
    AP_AXIS val;
    val.keep = pix.keep;
    val.strb = pix.strb;
    val.user = pix.user;
    val.id = pix.id;
    val.dest = pix.dest;
    val.last = 0; // 最後のビットの時だけ 1 で，その他の時は 0 にする

    int read_cnt = width * r + r + 1; // 何ビット読んだかを保存
    for_y: for (int py = 0; py < height; py++) {
        for_x: for (int px = 0; px < width; px++) {
#pragma HLS PIPELINE II=1
            int val_out = 0;
            // 出力値を計算
            // HLS PIPELINE の内側なので，for は全てインライン展開される
            for (int wy = -r; wy <= r; wy++) {
                for (int wx = -r; wx <= r; wx++) {
                    if (bounds_ok(py + wy, px + wx))
                        val_out += window[wy + r][wx + r] * coeff[wy + r][wx + r];
                }
            }
            // 計算した出力値を val.data に入れて，最後の出力なら val.last を 1 にする
            val.data = bitshift(val_out, shift);
            if (py == height-1 && px == width-1) val.last = 1;
            out_strm << val;

            int val_in = 0;
            // 入力が残っていれば読んで，val_in に格納する
            if (read_cnt < height * width) {
                pix = in_strm.read();
                val_in = pix.data;
                read_cnt++;
            }

            // window を 1 列左にシフトする
            for (int wy = 0; wy < d; wy++) {
                for (int wx = 0; wx < d - 1; wx++) {
                    window[wy][wx] = window[wy][wx+1];
                }
            }

            // linebuf[:][px] を window[:][d-1] に格納し，linebuf[:][px] を 1 行上にシフトする
            window[0][d-1] = linebuf[0][px];
            for (int ry = 1; ry < d-1; ry++) {
                window[ry][d-1] = linebuf[ry][px];
                linebuf[ry - 1][px] = linebuf[ry][px];
            }
            window[d-1][d-1]= val_in;
            linebuf[d-2][px] = val_in;
        }
    }
}