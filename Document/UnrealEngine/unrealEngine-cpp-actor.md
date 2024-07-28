# Unreal C++ のアクタ

- `AActor`についてのまとめ

---
## 関連

- [[unrealEngine-cpp]]
- [[unrealEngine-cpp-component]]

---
## AActor

- [ドキュメント](https://docs.unrealengine.com/5.3/en-US/actors-in-unreal-engine/)
- [C++リファレンス](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/GameFramework/AActor/)
- [[unrealEngine-cpp#UObject |UObject]]のメンバを継承している

### Constructor

- [これ](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_03_constructor_destructor#destructor)
- コンストラクタには、そのクラスの**メンバの初期化だけ書くのがよさそう**
  - [これ](https://www.reddit.com/r/unrealengine/comments/107vid2/comment/j6s70h3/)によると、コンストラクタはC++コードをビルドした後、エディタですぐ実行されるらしい
    - そのため、他のクラスに依存したコードを書くとクラッシュすることがある

### Destructor

- [これ](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_03_constructor_destructor#destructor)

### OnConstruction

- [[unrealEngine-blueprint |Blueprint]]でいうところの[[unrealEngine-blueprint#Construction Script |Construction Script]]
- ゲーム中には実行されないが編集時に実行される内容
- ~~ゲーム開始時点で確定させてもよい内容は、この関数内に記述し、ゲーム中の負荷を減らす~~
  - なんかよく調べたらあまり**負荷のかかる作業はここに書かないほうがよさそう**(詳しくは下記)
- [この記事](https://isaratech.com/ue4-be-careful-with-the-construction-script/)によると、以下のタイミングで呼ばれるらしい
  - アクタの配置
  - アクタの移動
  - アクタのプロパティ変更
- レベルエディタで作業しているとかなり頻繁に呼ばれるみたいなので、できるだけ軽い処理にしないとやばい(最悪で循環呼び出しとかになってるとエディタクラッシュする)

### BeginPlay

- ゲーム開始時に実行される

### EndPlay

- アクタが破棄されるときに実行される(ゲーム終了時も?)
- デストラクタはアクタの破棄(画面から消える)からしばらくして処理が走るようになっているため、終了処理はこちらに入れたほうがいい感じになる
  - [参考](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_03_constructor_destructor#destructor)

### Tick

- 毎フレーム実行される

### EnableInput

- コントローラーを引数にとって、そのコントローラーからの入力を有効にする
  - コントローラーは[GetPlayerController](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/Kismet/UGameplayStatics/GetPlayerController/)で持ってこれる

```cpp
// 基本的な使い方
EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
```

### InputComponent

- [[#EnableInput]]で入力を有効化した場合に、その入力のハンドリングを設定できる構造体

#### BindKey

- メンバ関数
- 指定したキー入力に対するハンドリングを設定
  - キーは`EKey`列挙体に定義されている

#### BindAction

- メンバ関数
- UEエディタの ProjectSettings ＞ Input ＞ Bindings ＞ ActionMappings で設定したキー設定を呼び出す
- 設定した名前を第一引数に文字列リテラルで指定すればよい

### GetActorLocation

- 自身の位置を[[unrealEngine-cpp#FVector |FVector]]型で取得

### Destroy

- アクタを破棄して[[#EndPlay]]とデストラクタを呼び出す

---
## オーバーライドできるイベント処理

- [公式ドキュメント](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/events-in-unreal-engine?application_version=5.2)
- [[#BeginPlay]]のような、**ゲーム内で起こる何かをトリガーに実行される関数は、継承先のクラスでオーバーライドすることができる**
  - AActorクラスのvirtualなメンバ関数を検索すれば大体わかりそう
- アクタの基底クラスに定義されているイベント以外は、独自でカスタムイベントを作り、発行するタイミングなどを定義する必要がある
  - [CustomEventについて](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/custom-events-in-unreal-engine?application_version=5.2)

---
## ワールド内のアクタの検索

- [公式ドキュメント](https://docs.unrealengine.com/4.26/ja/InteractiveExperiences/HowTo/FindingActors/)
