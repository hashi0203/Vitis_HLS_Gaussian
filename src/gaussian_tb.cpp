#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "gaussian.h"

using namespace std;

void get_img(int img[height][width]);
void gaussian(AXI_STREAM &in_strm, AXI_STREAM &out_strm);
void gaussian_test(int input[height][width], int output[height][width]);

int main() {
    int img[height][width]; // 画像データ
    get_img(img);
    int src[height][width]; // gaussian_test の入力データ
    int dst[height][width]; // gaussian_test の出力データ
    AXI_STREAM in_strm("in_strm"); // gaussian の入力データ
    AXI_STREAM out_strm("out_strm"); // gaussian の出力データ
    AP_AXIS pix;

    // src と in_strm に画像データを入れる
    for(int py = 0; py < height; py++){
        for(int px = 0; px < width; px++){
            src[py][px] = img[py][px];
            pix.data = img[py][px];
            in_strm << pix;
        }
    }

    // ハードウェアで実行する関数をシミュレーションする
    gaussian(in_strm, out_strm);

    // 結果を検証するためにテスト用のガウシアンフィルタを実行する
    gaussian_test(src, dst);

    // gaussian と gaussian_test の結果を比較する
    AP_AXIS output;
    for(int py = 0; py < height; py++){
        for(int px = 0; px < width; px++){
            output = out_strm.read();
            if (py == height-1 && px == width-1 && output.last != 1) {
                // 最後の出力なのに last が 1 でないとき
                printf("ERROR last is not equal to 1\n");
                return 1;
            }
            if(output.data != dst[py][px]){
                // 結果が一致しない時
                printf("ERROR HW and SW results mismatch\n");
                return 1;
            }
        }
    }
    printf("Success HW and SW results match\n");
    return 0;
}

// ガウシアンフィルタを計算する関数
// この結果と gaussian.cpp で計算した結果が一致するかを確かめる
void gaussian_test(int input[height][width], int output[height][width]) {
    for (int py = 0; py < height; py++) {
        for (int px = 0; px < width; px++) {
            int acc = 0;
            for (int wy = -r; wy <= r; wy++) {
                for (int wx = -r; wx <= r; wx++) {
                    if (bounds_ok(py + wy, px + wx)) {
                        acc += input[py + wy][px + wx] * coeff[wy + r][wx + r];
                    }
                }
            }
            acc = bitshift(acc, shift);
            output[py][px] = acc;
        }
    }
}

// 画像を読み込む
void get_img(int img[height][width]) {
    cv::Mat src = cv::imread("lena-gray.png", cv::IMREAD_GRAYSCALE);
    int h = std::min(height, src.rows);
    int w = std::min(width, src.cols);
    for (int py = 0; py < height; py++) {
        for (int px = 0; px < width; px++) {
            if (py < h && px < w) {
                img[py][px] = src.data[py*src.cols + px];
            } else {
                img[py][px] = 0;
            }
        }
    }
}