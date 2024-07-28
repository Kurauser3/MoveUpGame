# UEのC++で使用される型

- UnrealEngineでコーディングするときは、ネイティブなC++で使われている型を使わず、同じような意味合いでも別の型を使用しないといけない場合があるため、ここにそれらをまとめる
- クラスとかで実装されてるので厳密じゃないかもしれないけど、ここでは型と言わせてもらう
- またベクトル計算をする[[#FVector]]などのゲーム制作向けの拡張的な型も用意されている
- 主要な型の一覧は[Unreal Engine UProperties | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/unreal-engine-uproperties?application_version=5.2)

---
## 関連

- [[unrealEngine-cpp]]
- [[unrealEngine-cpp-class]]

---
## 接頭辞F系

- [[UE5] 構造体やクラスの名前に付いている接頭辞 'F' の意味 | Resolution : 御堂レゾの技術ブログ (midoureso.net)](https://midoureso.net/p/2023-02-16_1/)
- 記事によるともともとは`Float`の`F`だったらしいが、なぜか浮動小数点数型と関係ないものにもつけられるようになってしまったとのこと。うんちすぎる
- 最初につくられた[[#FVector]]すら今や`double`型に変更されているらしく、もはや意味が通らない接頭辞になっている
- とりあえずUnrealEngineで使える型なんだと考えればよさそう

### FName

- [公式ドキュメント](https://docs.unrealengine.com/5.3/ja/fname-in-unreal-engine/)
- 独自のストレージシステムと変更を受け付けない静的な性質により、アクセスが高速

### FText

- [公式ドキュメント](https://docs.unrealengine.com/5.3/ja/ftext-in-unreal-engine/)
- テキストローカライゼーションをサポートしているため、**ユーザーに向けて表示するテキスト**はこのクラスを使用したほうが良い

### FString

- [公式ドキュメント](https://docs.unrealengine.com/5.3/ja/fstring-in-unreal-engine/)
- UEではC++標準のstring型ではなくこちらを使う
- 他の文字列型([[#FName]]、[[#FText]])と違い、**検索、修正、文字列比較などが可能**

#### Printf

- C標準のprintf関数と同じようにフォーマット指定子(`%d`など)を使って書けるやつ
- 生成された文字列が戻り値として返ってくるだけで、何かに出力されるわけではなさそう?

### FVector

- [C++リファレンス](https://docs.unrealengine.com/4.27/en-US/API/Runtime/Core/Math/FVector/)
- 3Dの座標、ベクトル
- `.X`、`.Y`、`.Z`プロパティで3軸の座標を取得する(`double`型で取得できる)
- オペレータはベクトル計算用にオーバーライドされている

#### Set

- ベクトルの各軸の値を再度設定する
- 普通に`Vector.X = 100.0`のように再代入でも設定できる

#### GetSignVector

- ベクトルの要素の符号(+方向か-方向か)だけ取得する
- +1か-1で返却されるっぽい

#### GetSafeNoamal

- 正規化されたベクトルを取得する
  - ベクトルの長さを1として考えた場合の各軸の値を取得できる
  - ベクトルの長さを1とするので、元の長さの情報は失われ、向きと各軸の比率だけわかる
- `GetUnsafeNormal`という関数もあるが、こちらは0ベクトルのときの例外対応が書かれていないようなので、基本使わないと思う

### FVector2D

- 2Dの座標、ベクトル

### FRotator

- アクタの方向
- ロール、ピッチ、ヨーのオイラー角を使用する
- デフォルトコンストラクタは初期化をしないのですべて0に設定したいときは、`FRotator::ZeroRotator`などを使う
  - [参考](https://forums.unrealengine.com/t/feature-request-frotator-default-constructor/630641/2)

### FWorldContext

- [[#UWorld]]を持っているやつ
- まだよくわかってない
- `UEngine`はこいつのリストである`WorldList`というメンバを持っている

---
## 接頭辞T系

- `Templete Class`の`T`
- ポインタとか配列とかの代わりになるようなものが用意されている

### TFunction

- [参考](https://qiita.com/kamitani_08/items/b1d804a6d6d9bf18a70a)
- `std::function`と同等の機能
- 関数オブジェクト型

### TArray

- [公式ドキュメント](https://docs.unrealengine.com/4.27/ja/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TArrays/)
- [大体ここに書いてる使い方で足りる](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_90_cpp-array_control)
- コンテナ型
- C++言語使用の配列じゃなく、`std::vector`と同じ感じ(動的配列)
  - [[cpp-std#vector]](参考までであり、当たり前だが使い方は違う)

### TObjectPtr

- [公式ドキュメント](https://docs.unrealengine.com/5.0/ja/unreal-engine-5-migration-guide/#c++%E3%82%AA%E3%83%96%E3%82%B8%E3%82%A7%E3%82%AF%E3%83%88%E3%83%9D%E3%82%A4%E3%83%B3%E3%82%BF%E3%81%AE%E3%83%97%E3%83%AD%E3%83%91%E3%83%86%E3%82%A3)
- [[#UObject]]のポインタ、コンテナクラスには`T*`よりも`TObject<T>`の使用が推奨されている
- パフォーマンスに影響はないが、開発時のエクスペリエンスが向上するらしい(よくわかってない)

### TSubclassOf

- [公式ドキュメント](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/typed-object-pointer-properties-in-unreal-engine?application_version=5.2)
- `UClass`を安全に提供するためのテンプレートクラス
- `UClass`をそのまま[[#UPROPERTY]]で公開すると、制限なしにいろいろなアセットをプロパティに追加できる
  - デザイナーが予期しないクラスを入れてきたときに、最悪クラッシュしたりするかもしれない
- `TSubclassOf<T>`で`T`クラスの**派生クラスだけプロパティにセットできるように制限できる**
- そのままでポインタ型なので`*`つけなくてよい

```cpp
/** type of damage */
UPROPERTY(EditDefaultsOnly, Category=Damage)
UClass* DamageType; // ダメージタイプから派生したクラス以外もセットできる
```

```cpp
/** type of damage */
UPROPERTY(EditDefaultsOnly, Category=Damage)
TSubclassOf<UDamageType> DamageType; // ダメージタイプの派生クラスだけセット可能
```

- ブループリントエディタで設定できるアセット(クラス)に制限がかかる

![](https://i.imgur.com/DskGanC.png)
