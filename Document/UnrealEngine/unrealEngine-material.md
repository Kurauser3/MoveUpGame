# UEのマテリアル

- 主に**マテリアルの概要とマテリアルエディタ**についての内容
- テクスチャも内包しているのでこちらで記載
- C++におけるマテリアルクラスについては[[unrealEngine-cpp-material]]で記述

---
## 関連

- [[unrealEngine]]
- [[unrealEngine-cpp-material]]

---
## マテリアルとは

- 「Unreal Engine 5 で極めるゲーム開発」 P634~
- マテリアルは、**物体の表面の質感を表す情報**
- 物体は、表面がツルツルなのかデコボコなのか、導体(金属)なのか誘電体(非金属)なのか、などの性質によって、人間の目に映る様子(光の反射など)が異なる
  - UnrealEngineではそれを再現するために **PBR(Physical Based Rendering : 物理ベースレンダリング)** が採用されており、これが計算に使うパラメータをマテリアルが持っている
- 物体表面の情報の中でも、色や柄については**テクスチャ**(画像データ)という別のアセットとして扱われており、それをマテリアルに取り込んで使うようになっている

---
## マテリアルエディタのインターフェース

![](https://i.imgur.com/bifjDC1.png)

- 「Unreal Engine 5 で極めるゲーム開発」 P639
- 見た目とか使い方はかなり[[unrealEngine-blueprint |ブループリント]]に近いノードベースのエディタ
- マテリアルにはステートがないので、状態変化は起こりえない
  - 同じデータを受け取れば必ず同じ値を返す**純粋関数**の体裁をとる

---
## マテリアルエディタのノード

### Panner

- [UV 座標をアニメートする | Unreal Engine 4.27 ドキュメント](https://docs.unrealengine.com/4.27/ja/RenderingAndGraphics/Materials/HowTo/AnimatingUVCoords/)
- [[#UVパニング]]を実現するためのノード

---
## 用語

### UV座標

- [【Unity 初心者】絵を見てざっくり学べるUnity　~UV座標って何？~ (zenn.dev)](https://zenn.dev/hitsumasf/articles/f18bf5548dd95d)
- 3Dのモデルを2Dの展開図にした時、その展開図にあてがわれた座標のこと
- U、Vは水平、垂直から来てるらしい

![](https://i.imgur.com/9mGREpe.png)

#### UVパニング

- [UV 座標をアニメートする | Unreal Engine 4.27 ドキュメント](https://docs.unrealengine.com/4.27/ja/RenderingAndGraphics/Materials/HowTo/AnimatingUVCoords/)
- テクスチャの[[#UV座標]]を水平、垂直、またはその両方を組み合わせて移動し、テクスチャが画像ではなく動画であるように錯覚させるもの
- UV座標のアニメートともいう
- 例えば、炎や水の揺らぎなどの表現に使用できる
- エディタでは[[#Panner]]ノードを使用することで実現する

---
## レシピ

### マグマ(溶岩)

- [UE4 How to make Lava Material l Unreal Engine 4.26 (Tutorial) (youtube.com)](https://www.youtube.com/watch?v=tuHu9cPjTo0)
  - スターターコンテンツだけで作れる