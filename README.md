# Vitis HLS と Vivado で作った回路を PYNQ を使って FPGA 上で動かすまで


FPGA を使って遊びたいなぁということで高位合成を使って FPGA 上に回路実装をしていこうと思います．

## 使用したツールのバージョンとそれぞれの役割
- Vitis HLS
    - バージョン: 2020.2
    - 高位合成 (高級言語からハードウェア記述言語に変換する) のためのツール
        - 今回は C++ を使って記述して，Verilog に変換します (Verilog を読んだりはしません)
- Vivado HLS
    - バージョン: 2020.2
    - Verilog で書かれた IP などから回路を作るのを支援し，bitstream を作ってくれるツール
- FPGA ボード
    - 種類: [Zynq UltraScale+ MPSoC ZCU104 評価キット](https://japan.xilinx.com/products/boards-and-kits/zcu104.html)
    - Vivado で作られた bitstream を元に回路を作って実行してくれるボード
- PYNQ
    - バージョン: PYNQ v2.6 (ZCU 104 用)
        - [開発ボード向けのダウンロードページ](http://www.pynq.io/board.html)からダウンロード可能
    - FPGA 上で動く Linux の一種で，回路にデータを送り込んだり，時間を計測したりするのに使用

## 大まかな流れ
FPGA 上で回路を動かすための流れをまず簡単に説明します．

1. C++ を使って Vitis HLS 用に実行したいコードを書く
1. Vitis HLS を使って，シミュレーションをしてデバッグする
1. Vitis HLS を使って，Verilog にコンパイルする
1. コンパイルしてできた IP を使って，Vivado で回路を組む (これが難しい…)
1. Vivado を使ってできた回路から bitstream を作成する (いわゆるジェネビ)
1. 生成された bitstream を PYNQ にコピーする
1. PYNQ で回路にデータを流して結果を得る

このような順番で回路を作って動かしていきたいと思います．

詳しい説明は以下の Qiita の記事に書いているのでご覧ください．

1. [コードを書く](https://blog.n-hassy.info/2021/05/vitis-hls-to-fpga-1/)
2. [シミュレーションをする](https://blog.n-hassy.info/2021/05/vitis-hls-to-fpga-2/)
3. [Vivado で回路を組む](https://blog.n-hassy.info/2021/05/vitis-hls-to-fpga-3/)
4. [PYNQ を使って実機で動かす](https://blog.n-hassy.info/2021/05/vitis-hls-to-fpga-4/)


## 各ディレクトリの中身について
- src: C++ で作成した高位合成用のソースファイルが入っている．
- solution1: src を Vitis HLS でコンパイルした時の出力結果の一部が入っている．
- pynq: Vivado で合成した回路を PYNQ を使って動かす際に必要なファイルが入っている．