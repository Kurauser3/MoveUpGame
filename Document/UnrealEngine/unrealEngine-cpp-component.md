# Unreal C++ のコンポーネント

- アクタにアタッチするふるまい、外観のこと
- 多そうなのでこちらに記述

---
## 関連

- [[unrealEngine-cpp]]
- [[unrealEngine-cpp-actor]]

---
## UActorComponent

- [公式ドキュメント](https://docs.unrealengine.com/5.3/ja/adding-components-to-an-actor-in-unreal-engine/)
- アクタに追加する機能の単位
- 複数のアクタで共有したい機能をまとめることができる
- UEエディタ > Tools > NewC++Class からActorComponentを選択して作成する
- `GetOwner()`で自身を所有しているアクタを取得し、これを通じてアクタのプロパティを変更することができる

### BeginPlay

- `AActor`と同じくゲーム開始時に実行される関数

### TickComponent

- `AActor`と同じく毎フレーム実行される関数

---
## UChildActorComponent

- アクタを持つコンポーネント
- コンポーネントなのでアクタにアタッチすることができ、アクタがアクタを持つ構造にすることができる
- ただ、割と不評なようで**実用は避けたほうがよさそう**?
  - [参考](https://www.reddit.com/r/unrealengine/comments/1awjste/whats_wrong_with_child_actor_components/)
  - [参考](https://forums.unrealengine.com/t/store-child-actor-variable-inside-actor-component/484837)
- 例えば武器アクタをプレイヤーのアクタにつけるなどといったユースケースで紹介されることがあるが、これはスポーンした武器をプレイヤーのソケットにアタッチするという方法で解決できる
- アクタがアクタを持つという構造は、[[unrealEngine-cpp#マクロ#UPROPERTY |UPROPERTY]]と[[unrealEngine-cpp#TSubclassOf |TSubclassOf]]を使って、アクタのプロパティに別のアクタを持っておくという形で実現するとよさそう
  - そのほか、アセットを参照する方法は[公式ドキュメント](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/referencing-assets-in-unreal-engine?application_version=5.2)でも言及されているので参考にする

---
## USceneComponent

- [参考](https://forums.unrealengine.com/t/defaultsceneroot/366953/2)
- よくルートコンポーネントとして使われる
  - そのアクタの中心となるコンポーネント
  - それ以外のコンポーネントはルートコンポーネントからの相対的な位置、回転を持つことになる
- これ自体はあまり情報を持っていない
  - エディタ上は球の形で表されるが、実体はなく、ゲーム中は見えなくなる
  - コンポーネントの階層情報などを保持しているダミーコンポーネントのような感じ
- 他のコンポーネントの継承元となっており、メンバ関数は継承先で使われることが多い

### SetupAttachment

- 引数にとったコンポーネントを親とする

---
## UPrimitiveComponent

- ~~実体(ゲーム内での物体の表示)があるコンポーネントの継承元っぽい~~
  - キャラクターが持っているカプセルコンポーネント(コリジョンを判定する範囲)の継承元にもなっていることから、実体として表示されているかどうかは特に関係ないっぽい
  - **「[[unrealEngine#コリジョン |コリジョン]]を持つもの」** 全ての継承元と考えたほうがよさそう?

### SetCollisionProfileName

- こいつで**衝突時の当たり判定を決めることができる**
- 当たり判定は、プリセットとして用意されているものがいくつかある(自分で作ることもできる)
  - この当たり判定の設定をプロファイルと呼んでるらしい
  - UEエディタ > Settings > ProjectSettings > Engine > Collision > Preset
- [コリジョンのプリセット(profile)やオブジェクトの種類について](https://docs.unrealengine.com/5.3/ja/collision-response-reference-in-unreal-engine/)
- [そもそもコリジョンの概念が分からない](https://docs.unrealengine.com/5.3/ja/collision-in-unreal-engine---overview/)

### OnComponentHit

- [公式ドキュメント](https://docs.unrealengine.com/5.3/en-US/using-the-onhit-event/)
- **衝突を検知したときに発火する**デリゲート

#### AddDynamic

- メンバ関数っぽく書くことができるが、実際はマクロでありサジェストには出てこない
- その正体は`__Internal_AddDynamic`メンバ関数のラッパーマクロ
  - `__Internal_AddDynamic`をそのまま呼ぶとよくないという旨がコメントに書いてあるので、なんかきもいけどこのマクロを使おう
- 原因はまだわかってないが、[[unrealEngine-cpp-actor#Constructor |コンストラクタ]]でこいつを呼び出すとハンドラの設定がうまくいかない
  - [[unrealEngine-cpp-actor#BeginPlay |BeginPlay]]で呼び出せば普通に動く
- こいつに渡す関数の引数リストは、いちいち書くのがしんどいのでテンプレを以下に載せとく

```cpp
UPrimitiveComponent* HitComp, 
AActor* OtherActor, 
UPrimitiveComponent* OtherComp, 
FVector NormalImpulse, 
const FHitResult& Hit
```

##### ハンドラの引数の意味について

- 公式ドキュメントからリファレンスを見つけきれてないため、ところどころコンソール出力で確認したところがある
  - リファレンス見つけたら都度載せていく
- 第一引数(上述の引数リストでは`HitComp`)
  - イベントを発行した側のコンポーネント
- 第二引数(上述の引数リストでは`OtherActor`)
  - ぶつかった相手(アクタ)
- 第三引数(上述の引数リストでは`OtherComp`)
  - ぶつかった相手(コンポーネント)
- 第四引数(上述の引数リストでは`NormalImpulse`)
  - よくわからない...
- 第五引数(上述の引数リストでは`Hit`)
  - [リファレンス](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/Engine/FHitResult/)
  - ぶつかってきた方向など、衝突に関する情報が入っている

### GetMaterial

- [公式ドキュメント](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/Components/UPrimitiveComponent/GetMaterial/)
- **マテリアル(メッシュの表面の質感)のインスタンスを取得する**関数
- マテリアルインスタンスは複数持つことができるらしく、インデックスで管理されているため、引数には取得したいインスタンスのインデックスを指定する

---
## UStaticMeshComponent

- [[#USceneComponent]]からの継承あり
- [[unrealEngine#スタティックメッシュ |スタティックメッシュ]](外観)を持つコンポーネント
- StaticMeshを一つだけ持つことができるコンポーネント
  - [参考](https://forums.unrealengine.com/t/can-somebody-explain-instanced-static-mesh-components-to-me-static-mesh-ue4-27/588846/2)

### SetStaticMesh

- `UStaticMesh*`を引数にとり、**外観の情報をセットする**メンバ関数
- `LoadObject`関数を使用してスタティックメッシュを取得し、それをセットするといった使い方をする
  - 追記 : 基本的にはこのようにメッシュをハードコーディングすることはせず、UEエディタ上でメッシュを設定できるようにプロパティだけ公開するやり方が多いっぽい
    - [公式ドキュメント](https://docs.unrealengine.com/5.2/ja/gameplay-classes-in-unreal-engine/#%E3%82%A2%E3%82%BB%E3%83%83%E3%83%88%E3%81%AE%E5%8F%82%E7%85%A7)

```cpp
// Sample
Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
UStaticMesh* Mesh = LoadObject<UStaticMesh>(
  NULL,
  TEXT("/Game/Developers/tkr31/Meshes/SM_Floor400")
);
Plane->SetStaticMesh(Mesh);
```

---
## UShapeComponent

- [[#UPrimitiveComponent]]から継承
- 球、カプセル、ボックスなどの簡単な形状コンポーネントの継承元
  - [[#UBoxComponent]]
- これをそのまま使うことはあまりないかも?

---
## UBoxComponent

- [[#UShapeComponent]]から継承
- ボックス形状の領域をゲーム内に配置するためのコンポーネント
- **領域内だけに何かをスポーンする、領域内にいる間だけダメージを与えるなどのユースケース**が考えられる

### GetScaledBoxExtent

- [COLLISION BOX へのテクスチャの適用に関する簡単な質問 - プログラミング & スクリプティング / ブループリント - Epic Developer Community フォーラム (unrealengine.com)](https://forums.unrealengine.com/t/simple-question-about-applying-a-texture-to-a-collision-box/90867/2)
- 公式のドキュメントとかAPIリファレンスではよくわからなかったが、フォーラムによると**Scaleの拡大縮小設定がされた後**のボックスの範囲が取得できる
- Scaleの拡大縮小設定はエディタのDetailsに出てくる以下の設定のことだと思われる

![](https://i.imgur.com/hCxEJKi.png)

- 使ってみたところ、ボックスの範囲というのは**中心から各軸の端までの絶対値**が[[unrealEngine-cpp-type#FVector |FVector]]で返ってくるらしい
