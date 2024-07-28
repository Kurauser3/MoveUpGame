# UEのC++で使用するクラス

- ゲーム内で扱うクラスを記載する
- クラスの中でも「データ」というニュアンスが強いものについては[[unrealEngine-cpp-type]]で「型」として取り扱うことにする

---
## 関連

- メンバ関数やプロパティの説明まで含めると多いので、一部他のファイルにまとめる
  - [[unrealEngine-cpp-actor]]
  - [[unrealEngine-cpp-character]]
  - [[unrealEngine-cpp-component]]

---
## GEngine

- ゲーム全体を管理
- デバッグに使える関数も持ってる

### AddOnScreenDebugMessage

- [UE5/UE4 C++でデバッグ文字列（FString）をビューポート（Viewport）に表示する（UEngine::AddOnScreenDebugMessage） 凛(kagring)のUE5/UE4とゲーム制作と雑記ブログ (fc2.com)](http://kagring.blog.fc2.com/blog-entry-659.html)
- **デバッグ用**
- 画面に適当な文字を表示できる
- [[unrealEngine-cpp-type#FString#Printf|FString::Printf]]を合わせて使用することが多いかも

| 引数                          | 意味                          |
| --------------------------- | --------------------------- |
| uint64 Key                  | 同じ値のとき重複表示を避ける(?)指定しない場合は-1 |
| float TimeToDisplay         | 表示時間                        |
| FColor DisplayColor         | 色                           |
| const FString& DebugMessage | メッセージ                       |

---
## UObject

- UEに登場するほぼすべてのオブジェクトが継承するクラス
- その他のクラスまで書くと情報量が多すぎるので、別ファイルで記載する

### CreateDefaultSubobject

- [使い方](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_02_cpp-component)
- コンポーネントを作るときに使用するメンバ関数

---
## UClass

- [参考](https://gamedev.stackexchange.com/questions/161783/whats-the-difference-between-uclass-and-uobject)
- C++で**リフレクションを実現するため**に作成されるクラス
  - リフレクションとは？
    - あるクラスでは何ができるのか、どんなプロパティを持っているのかなどのプログラムの内容を、プログラムそのものが見に行き、それによって処理の内容を変化させる
    - [[#マクロ]]にも関連している
  - `UClass`は何かの**クラスの基本的な情報を持っているインスタンス**であり、プログラム内でクラスの中身(定義)を見たいときに使用する

---
## UWorld

- ゲーム内のすべてのマップデータとその中身のオブジェクトを持っているやつ
  - =[[#ULevel]]の配列を持っている
- よく`UGameplayStatics`などが持ってる静的関数で、`WorldContextObject`という引数名があるが、あれはこの`UWorld`を参照できる(自身が属しているWorldを`GetWorld`関数で呼び出せる)オブジェクトだという意味
  - [WorldContextObject についての理解 #C++ - Qiita](https://qiita.com/koorinonaka/items/3dd51c29676ddebe39f8)

---
## AGameModeBase

- [Unreal Engine の Game Mode と Game State | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/game-mode-and-game-state-in-unreal-engine?application_version=5.2)
- 昔のバージョンでは`AGameMode`だったとか
  - なんかもっと抽象化をうまいことやったのが`AGameModeBase`で、`AGameMode`は今も残ってるけど`AGameModeBase`のサブクラスになっているらしい
- [[unrealEngine#概念#ゲームモード]]に概要を記載
- セットアップ方法は[Unreal Engine でゲームモードをセットアップする | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/setting-up-a-game-mode-in-unreal-engine?application_version=5.2)

---
## AHUD

- Widget(画面に表示するステータスやメニュー)を持ち、それらの遷移処理を定義するクラス(リファレンスがないのでテキトウ言ってるかも)
  - [この辺](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_002/viewer/chap_01-05_cpp-ui_widget_game_pause_input)の実装を見てるとそのようなイメージで組んでる
- [[#AGameModeBase]]クラスのメンバーに`HUDClass`があるので、それに指定するとなんかうまいこと使えるようになる(そもそもゲームモードに持たせたものがどのように管理されるのかよくわかってない)

