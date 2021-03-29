#include "gaussian.h"

void gaussian(AXI_STREAM &in_strm, AXI_STREAM &out_strm) {
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS
#pragma HLS INTERFACE axis port=in_strm
#pragma HLS INTERFACE axis port=out_strm

    int linebuf[d-1][width];
    int window[d][d];
#pragma HLS ARRAY_PARTITION variable=linebuf complete dim=1
#pragma HLS ARRAY_PARTITION variable=window complete dim=0

    AP_AXIS pix;
    // initialize linebuf and window
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

    // initialize val with the value of pix
    AP_AXIS val;
    val.keep = pix.keep;
    val.strb = pix.strb;
    val.user = pix.user;
    val.id = pix.id;
    val.dest = pix.dest;
    val.last = 0;

    // already read width * r + r + 1 to initialize
    int read_cnt = width * r + r + 1;
    for_y: for (int py = 0; py < height; py++) {
        for_x: for (int px = 0; px < width; px++) {
#pragma HLS PIPELINE II=1
            int val_out = 0;
            // calculate output value
            for (int wy = -r; wy <= r; wy++) {
                for (int wx = -r; wx <= r; wx++) {
                    if (bounds_ok(py + wy, px + wx))
                        val_out += window[wy + r][wx + r] * coeff[wy + r][wx + r];
                }
            }
            // set values to val and return (val.last shoud be 1 for the last input)
            val.data = bitshift(val_out, shift);
            if (py == height-1 && px == width-1) val.last = 1;
            out_strm << val;

            int val_in = 0;
            // read input if any and set
            if (read_cnt < height * width) {
                pix = in_strm.read();
                val_in = pix.data;
                read_cnt++;
            }

            // shift window to 1 row left
            for (int wy = 0; wy < d; wy++) {
                for (int wx = 0; wx < d - 1; wx++) {
                    window[wy][wx] = window[wy][wx+1];
                }
            }

            // store linebuf[:][px] in window[:][d-1] and shift linebuf[:][px] to 1 col above
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