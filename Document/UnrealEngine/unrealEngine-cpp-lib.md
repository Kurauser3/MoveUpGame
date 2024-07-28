# Unreal C++ で使うライブラリ

- 主に[Engine](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/)に入ってる機能群を載せる
- 多すぎるのでよく使うやつのメモだけ
- [[unrealEngine-cpp-module |モジュール]]の**依存関係を設定しないとヘッダーを`#include`しても動かせない可能性あり**

---
## 関連

- [[unrealEngine]]
- [[unrealEngine-cpp]]

---
## Kismet

- Engineモジュールに入ってる
- 一言でまとめるには難しいライブラリ
- 個人的にはゲームの状態を俯瞰しているやつみたいな印象を持ってる

### UKismetSystemLibrary

#### PrintString

- 文字をコンソールか画面上(システムメッセージ的な感じ)に出力する
- 基本デバッグ用でゲームに組み込むってことはなさそう

### UGameplayStatics

- ゲーム内から何かを探したり、レベルの遷移をしてくれたり...
- ゲームの外側から助けてくれるような関数群

#### GetPlayerController

- ワールドに存在するコントローラを取得できる
  - マルチプレイの他のプレイヤーのコントローラーも取得できる

#### OpenLevel

- Levelの名前を指定するだけで何故かアセットの中から見つけ出してそのマップに遷移してくれるやつ
- 仕組みがよくわかってないので後で調べる
  - どこからレベルを検索している???
  - アセットの中に名前が同じレベルがあったらどうする???

---
## Math

- [Math | Unreal Engine 5.4 Documentation | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/Math)
- Coreモジュールに入ってる
- まああるよな
- 数学関数が入ってるライブラリ

### FMath

- `#include "Math/UnrealMathUtility.h"`

#### RoundToZero

- [FMath::RoundToZero | Unreal Engine 5.4 Documentation | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/Math/FMath/RoundToZero/1)
- 小数点数を整数型に丸める
- 丸めの際に、**0に近くなるように**丸めてくれる
  - 0.5なら0に、-0.5でも0に

### FRandomStream

- [FRandomStream | Unreal Engine 5.4 Documentation | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/Math/FRandomStream)
- `#include "Math/RandomStream.h"`
- UEの疑似乱数生成はこの中
- ざっくりした使用方法は[UE5 C++乱数 #UnrealEngine5 - Qiita](https://qiita.com/gdgdkuro/items/e6bc3988297c6eef7245)

#### GenerateNewSeed

- シード値までランダムで決めてくれる(ありがとう)

#### FRandRange

- `int32 RandRange( int32 Min, int32 Max ) const`
- 最小と最大を決めてランダム値を生成
- 整数型で返ってくる
