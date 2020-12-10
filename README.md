# myled
ロボットシステム　課題

# 動作内容
講義内で作製したデバイスドライバに改良を加える  
 LEDを3桁の２進数として、1秒間隔で点灯させる  
 最大までいったら、1秒間隔で3回点滅させ、点灯しているときはブザーを鳴らす
# 使用したもの
・Raspberry Pi 3 B+  
・ブレッドボード  
・LED*3  
・電子ブザー  
・220Ω抵抗  
・F-Mジャンパー線  
・コの字型ワイヤ  
# 実行方法
$ git clone https://github.com/TNKseiya/myled.git  
$ cd myled  
$ sudo insmod myled.ko  
$ sudo chmod 666 /dev/myled0  
$ echo 1 > /dev/myled0  
# 実行動画リンク
https://youtu.be/XxwMSyTCG_Q

# ライセンス

slackの課題1に上がっていた人の表記を真似てLICENSEをCOPYINGに変更した
