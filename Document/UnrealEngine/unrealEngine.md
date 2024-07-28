# UnrealEngine

- ゲームエンジン
- 使用するプログラミング言語は[[cpp |C++]]
- C++ができなくてもGUIベースのエディタがあり、プログラミングしなくてもゲームを作れる

---
## 関連

- [[unrealEngine-cpp]]
- [[unrealEngine-blueprint]]

---
## VisualStudioとの統合

### UEのどこから起動するのか

- UEの開発画面の「ツール」から「Visual Studio を開く」

### デバッグに必要なエディタシンボルをインストールする

- [[debug#シンボルファイル]]でシンボルとは何か解説している
- C++で開発する場合は、シンボルがないとデバッグに不便なのでインストールしておく
- [参考](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_01_vs2022_setup#%E3%83%87%E3%83%90%E3%83%83%E3%82%B0%E3%81%AB%E5%BF%85%E8%A6%81%E3%81%AA%E3%82%A8%E3%83%87%E3%82%A3%E3%82%BF%E3%82%B7%E3%83%B3%E3%83%9C%E3%83%AB%E3%81%8C%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E3%81%95%E3%82%8C%E3%81%A6%E3%81%84%E3%82%8B%E3%81%8B%E7%A2%BA%E8%AA%8D%E3%81%99%E3%82%8B)
- UEランチャー ＞ ライブラリ ＞ Engine起動ボタン横のドロップダウン ＞ オプション ＞ デバッグに必要なエディタシンボルにチェックをつけてインストール
　
### ブループリント情報を見れるようにする

- [Visual Studio Integration Tool について](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_01_vs2022_integration_tool)
- 多分インストール、有効化してない状態だと、VSでUEプロジェクトを開いたときメッセージがでる
  - 「ブループリント見れません」とか「プラグインもってないよ」的な内容
- とりあえず[UEのマーケットプレイス](https://docs.unrealengine.com/using-the-unrealvs-extension-for-unreal-engine-cplusplus-projects/)に飛ぶ
- サインインしてインストールが完了したら、(ブラウザで見ている場合は)インストールボタンがラするする

---
## 学習サイト

- [公式ドキュメント](https://docs.unrealengine.com/5.1/ja/understanding-the-basics-of-unreal-engine/)
- [UE5から始める C++ & Blueprint](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001)

---
## 概念

- [公式ドキュメント](https://docs.unrealengine.com/5.1/ja/unreal-engine-terminology/)
- エディタやツールの名前は[[#ツールとエディタ |こちら]]で書いていく

### ゲームプレイフレームワーク

- [Unreal Engine のゲームプレイ フレームワーク | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/gameplay-framework-in-unreal-engine?application_version=5.2)
- プロジェクトの構成要素であるクラスやコンポーネントのことで、以下のような要素が提供されている
  - [[#アクタ]]
    - [[#ゲームモード]]
    - [[#ゲームステート]]
    - [[#カメラ]]
    - [[#ポーン]]
    - コントローラー
      - [[#プレイヤーコントローラー]]
      - [[#AIコントローラー]]
    - [[#HUD]](ヘッドアップディスプレイ)
  - [[#ゲームプレイタイマー]]

#### フレームワーククラスの関係

![](https://i.imgur.com/QVOKcXE.png)

### ワールド

- すべての[[#レベル]]のコンテナ

### レベル

- [公式](https://docs.unrealengine.com/5.1/ja/levels-in-unreal-engine/)
- ゲームのプレイ領域のこと
- ジオメトリ、ポーン、アクタなどの、プレイヤーが見たり触れたりできるもののすべてが含まれる
- レベルは、個別に`.umap`ファイルに保存されるため、**マップ**と言われることもある

### オブジェクト

- UnrealEngineの最も基本的なクラス
- ほぼすべてのものが、オブジェクトから機能を継承している
- C++では、`UObject`がすべてのオブジェクトの基本クラスになっている

### アクタ

- [公式](https://docs.unrealengine.com/5.1/ja/actors-and-geometry-in-unreal-engine/)
- [リファレンス](https://docs.unrealengine.com/5.2/ja/unreal-engine-actors-reference/)
- アクタは、カメラ、プレイヤー開始位置など、[[#レベル]]に配置できる任意の[[#オブジェクト]]のこと
  - 動くものっぽい名前だが、ただのキューブとか背景とかもアクタ
    - 動かない系は[[#スタティックメッシュアクタ]]と言われる
- C++では、`AActor`がすべてのアクタの基本クラスになっている

#### スタティックメッシュアクタ

- [公式ドキュメント](https://docs.unrealengine.com/5.2/ja/static-mesh-actors-in-unreal-engine/?utm_source=editor&utm_medium=docs&utm_campaign=rich_tooltips)
- [[#レベル]]内で、1つの3Dメッシュを表示する[[#アクタ]]
- [[#アクタ]]がスタティック(動かせない)である
  - 厳密には、アクタをゲーム内で動かすこと自体は可能だが、メッシュのジオメトリが変化しない(折れたり歪んだりなどのように変形しないという意味だと思われる)
- 今勉強してる時点での認識としては、[[#スタティックメッシュ]]という[[#コンポーネント]]をアタッチされたアクタのことだと思われ

#### ポーン

- ゲーム内キャラクターのような、アバターとして機能する[[#アクタ]]のサブクラス
- プレイヤーまたはNPCとして制御できる
- 人間かAIによって制御されている状態を、**所有された**と表現する

##### キャラクター

- [[#ポーン]]のうち、プレイヤーが使用することを意図したもの
- [[#ポーン]]のサブクラス

#### プレイヤーコントローラー

- プレイヤーの入力を受け取り、ゲーム内インタラクションに変換する[[#アクタ]]
- [[#ポーン]]([[#キャラクター]])を実体として所有することが多い
- C++では`APlayerController`クラス

#### AIコントローラー

- NPCを表現するために使用する
- デフォルト([[#プレイヤーコントローラー]]が所有していない状態)では、[[#ポーン]]と[[#キャラクター]]はベースAIコントローラーをセットされる
- C++では`AIController`クラス

#### ゲームモード

- [Unreal Engine の Game Mode と Game State | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/game-mode-and-game-state-in-unreal-engine?application_version=5.2)
- ゲームのルールを管理するクラス`AGameModeBase`とそのサブクラス
- [[#アクタ]]の一つだが、オンライン型のゲームの場合は**サーバーだけが実体を持ち、クライアント側には生成されない**など特殊な仕様を持つ
- 主に以下のようなルールを持つ
  - プレイヤーや観戦者(スペクテーター)の最大人数
  - ゲームへの参加方法、スポーン位置の選択
  - 一時停止の処理方法
  - [[#レベル]]間の推移
- **ゲームのベースとなるクラスを指定する役割**もあり、エディタではWorldSettingsの中で編集できる

![](https://i.imgur.com/jBQFoo3.png)

#### プレイヤーステート

- ゲームのプレイヤーの状態を表す
- 具体的には以下
  - 名前
  - カレントレベル
  - ヘルス値
  - スコア
  - キャプチャー ザ フラッグ ゲームで現在旗を持っているかどうか。
- ノンプレイヤーのAIにはプレイヤーステートがない(プレイヤーをシミュレートするボットにはある)
- C++では`PlayerState`クラス

#### HUD

- [Unreal Engine のユーザー インターフェースと Hud | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/user-interfaces-and-huds-in-unreal-engine?application_version=5.2)
- ヘッドアップディスプレイの略
- 画面に表示されるがゲーム内で触れられる物体としては扱わない情報
  - 例えばHPゲージ、スコア、残り時間の表示など
- 基本的にユーザーが操作できない表示のことをHUDといい、ユーザーがマウスクリックなどで操作できるアイコンやボタンなどは**UI**(ユーザーインターフェース)というらしい
  - 意味合いとして呼び分けているだけで、UnrealEngineではどちらもCanvasというオブジェクトを使用して描画できる

### ゲームプレイタイマー

- [Unreal Engine でのゲームプレイ タイマー | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/gameplay-timers-in-unreal-engine?application_version=5.2)
- アクションを遅らせたり、一定期間持続させたりする際に使用する
- `FTimerManager`型の構造体で管理されており、このデータを`World`とGameInstanceオブジェクトが持っている
  - タイマーが必要な時は、基本的に`World`を取得してそこからタイマーの値を取得する流れになる(`GetWorld()->GetTimerManager()`)

### コンポーネント

- [[#アクタ]]にアタッチする機能
- 例えば
  - Spot Light : アクタが発光する
  - Rotating Movement : アクタが回転する
  - Audio : アクタがサウンドを再生する
- **アクタがどのようにふるまうか**を定義するもの
  - このふるまいには外見も含まれるっぽい
    - [[#スタティックメッシュ]]など

#### スタティックメッシュコンポーネント

- 外見を決めるコンポーネントで、[[#メッシュ]]のジオメトリが変化しない(折れたり歪んだりなどのように変形しないという意味だと思われる)もの
- この[[#コンポーネント]]を持っている[[#アクタ]]を[[#スタティックメッシュアクタ]]と呼んでるっぽい

#### スケルタルメッシュコンポーネント

- 「Unreal Engine 5 で極めるゲーム開発」 P126
- 複数の**関節(ボーン)を持ち、形が変化**する[[#メッシュ]]
- [[#キャラクター]]や[[#ポーン]]などのような、操作可能なオブジェクトに適用される
- [[#スケルトン]]と[[#物理アセット]]が関連付けられているのが基本
  - 紐づけられていない状態だと、3Dモデルの形状の情報しか持ってない

### メッシュ

- 「Unreal Engine 5 で極めるゲーム開発」 P19
- 網の目の意味
- 3Dモデルは三角形の面の集まりであり、この三角形の辺が網のように見えることからメッシュという言い方をする

### スケルトン

- 「Unreal Engine 5 で極めるゲーム開発」 P127
- [[#スケルタルメッシュ]]と対になる、骨の状態を表すアセット
- スケルタルメッシュが見た目(形状)の情報を持っており、スケルトンは関節の情報を持っている

### トランスフォーム

- 「Unreal Engine 5 で極めるゲーム開発」 P37
- 座標(Vector)と向き(Rotation)と大きさ(Scale)の3要素を総称したもの

### ピボット

- [参考](https://historia.co.jp/archives/13368/#:~:text=%E4%BD%8D%E7%BD%AE%E5%A4%89%E6%9B%B4%E3%83%94%E3%83%9C%E3%83%83%E3%83%88%E3%83%9D%E3%82%A4%E3%83%B3%E3%83%88%E3%81%AF%E3%83%9B%E3%82%A4%E3%83%BC%E3%83%AB%E3%82%AF%E3%83%AA%E3%83%83%E3%82%AF%E3%81%A7%E3%83%9B%E3%83%BC%E3%83%AB%E3%83%89%E3%81%99%E3%82%8B%E3%81%A8%E5%8B%95%E3%81%8B%E3%81%9B%E3%81%BE%E3%81%99%E3%80%82)
- [[#トランスフォーム]]の中心になる点
  - 回転や大きさがここ基準で変更される

### コリジョン

- [公式ドキュメント](https://docs.unrealengine.com/5.3/ja/collision-in-unreal-engine/)
- 物体同士がぶつかること(ぶつかったときの挙動の意味合いもありそう)

### 物理アセット

- 「Unreal Engine 5 で極めるゲーム開発」 P128
- 骨に追従する[[#コリジョン]]
- [[#スケルタルメッシュ]]のように複数の関節を持ち、変形できるアセットには[[#スタティックメッシュ]]と同じように決め打ちのコリジョン形状を指定することができないので、特別な方法でコリジョン設定を行う
  - コリジョンを関節で分割して手足の動きに連動させる方法がとられており、このような骨に追従するコリジョンデータを、物理アセットという

---
## ツールとエディタ

- [公式ドキュメント](https://docs.unrealengine.com/5.1/ja/tools-and-editors-in-unreal-engine/)
- ツールとエディタの定義は下記の通り
  - ツール：特定のタスクを実行するために使用するもの
  - エディタ：ツールの集まり

### レベルエディタ

- デフォルト設定において、プロジェクトを開いたときにはじめて開く画面のこと
- ゲームのプレイスペースを定義する
- アクタとジオメトリ、ブループリント、Niagaraなどの様々なタイプを追加して構築する

![](https://i.imgur.com/tD1tyP5.png)

### スタティックメッシュエディタ

- [スタティックメッシュ](https://docs.unrealengine.com/5.1/ja/static-meshes/)（Blenderなどのモデリングソフトで作られた3Dモデル）のプロパティを設定したり、操作したりするエディタ
- [LOD](https://docs.unrealengine.com/5.1/ja/creating-and-using-lods-in-unreal-engine/)（スタティックメッシュからプレイヤーのカメラ位置が離れた時の描画の粗さ）を設定することもできる

### マテリアルエディタ

- マテリアル（メッシュに適用して外観を制御するアセット）を作成するエディタ

### ペルソナエディタ

- 「UnrealEngine5で極めるゲーム開発」 P128
- [[#スケルタルメッシュ]]のようなアニメーション関連の作業をするためのエディタ

### ブループリントエディタ

- C++でコードを記述することなく、アクタの制御やイベントのスクリプト作成ができるエディタ
- マテリアルの変更、UnrealEngineの機能実装まで可能

### レベルビューポート

- [[#レベルエディタ]]で実際にゲームのステージが表示されているところ

![](https://i.imgur.com/PqrHnIM.png)

### ギズモ

- アクタを選択したときに出てくる、[[#トランスフォーム]]を操作するためのUIのこと
- [[#レベルビューポート]]の右上に出てくるアイコンを切り替えることで操作したい項目(トランスフォームのうちのどれか)を選択することができる

![](https://i.imgur.com/SPnuvft.png)

![](https://i.imgur.com/wkuA8U8.png)

---
## エディタの使い方

- (UE Editor とか Unreal Editor ていうらしい)
- VisualStudioは[[visualStudio |別記事]]

### レベルエディタ

- レベルビューポート内の視点移動
  - 右クリック+WSAD(前後左右)+QE(下降上昇)+マウス操作(目線の移動)
  - 右クリック+マウスホイールで移動速度変更
  - マウスホイールのみの操作で引き、寄り

### コンテンツブラウザ

- コンテンツアセットを編集するためのエリア
- メニューバーのウィンドウメニューから開く
  - もしくはメインツールバーの+のマークがついてるアイコンから開く

#### リファレンスのリンク

- [ソースパネル](https://docs.unrealengine.com/5.1/ja/sources-panel-reference-in-unreal-engine/)
- [ブラウザの設定](https://docs.unrealengine.com/5.1/ja/content-browser-settings-in-unreal-engine/)

#### 共同開発する場合の設定

- [developersフォルダー](https://docs.unrealengine.com/5.1/ja/developers-folder-in-unreal-engine/)を使用する
  - コンテンツの他の部分を破壊せず、アセットを複製して作業できるスペース
  - 開発者ごとにフォルダーが作られる
  - コンテンツブラウザの右上にある設定から、「デベロッパーコンテンツを表示」
- developersフォルダーを[[#クックビルド]]から除外する
  - Unreal Editor メインメニューの編集から、プロジェクト設定を開き、「Directories to never cook」と検索したら出てくるので、developersフォルダを除外する設定にしておく

##### 命名規則

- 特にこだわりがなければ[UE5-style-guide](https://github.com/Allar/ue5-style-guide?tab=readme-ov-file#base-asset-name)準拠

---
## プロジェクトのディレクトリ構造

- [公式](https://docs.unrealengine.com/5.1/ja/unreal-engine-directory-structure/)

### ゲームプロジェクトのディレクトリ構成

- ドキュメントから完コピ

| ディレクトリ                    | 説明                                                                                                                                                                                                                              |
| ------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Binaries**              | コンパイル中に作成された実行ファイルまたは他のファイルが含まれます。                                                                                                                                                                                              |
| **Config**                | ゲームのデフォルト プロジェクト設定です。                                                                                                                                                                                                           |
| **Content**               | アセット パッケージとマップを含んだエンジンやゲームのコンテンツを格納します。                                                                                                                                                                                         |
| **External dependencies** | エンジンのパブリック ヘッダファイルを表示します (Visual Studio でのみ見ることができます)。                                                                                                                                                                          |
| **Intermediate**          | Visual Studio プロジェクト ファイルなど UnrealBuildTool が生成したファイルを含みます。これらのファイルは削除および再ビルドが可能です。                                                                                                                                             |
| **Saved**                 | 設定ファイルやログ ファイルなどエンジンで生成されるファイルを格納します。これらのファイルは削除および再ビルドが可能です。                                                                                                                                                                   |
| **Source**                | ゲーム モジュールのオブジェクト クラス ファイルを格納します。<br><br>- Classes : ゲーム オブジェクト クラスの定義を格納します (「`.h`」ファイル)。<br><br>- Private : プライベートなゲーム オブジェクト クラスの実装ファイルを格納します (「`.cpp`」ファイル)<br><br>- Public : パブリックなゲーム オブジェクト クラスの実装ファイルを格納します (「`.h`」ファイル) |

- ディレクトリの他に、ゲームプロジェクトに関連付けられた`.uproject`ファイルがあり、UEはこれを使うことでプロジェクトを開いて作業ができる
  - [言及されているとこ](https://docs.unrealengine.com/5.1/ja/working-with-projects-and-templates-in-unreal-engine/)

---
## ビルドとパッケージ化

### クックビルド

- [公式](https://docs.unrealengine.com/5.1/ja/build-operations-cooking-packaging-deploying-and-running-projects-in-unreal-engine/)
- クックは、UEで使用していたアセットを、デプロイ対象のプラットフォームで読み取れるように変換する処理のこと

### ライブコーディング

- [参考](https://ox.crdg.jp/tech/2023/12/01/172/)
- Live Coding
- UEエディタを開いた状態でC++クラスの変更をビルドし、変更を反映させる機能
- C++で開発するとき、処理を変更する度にIDEでビルドしなくてもいいように、デフォルトで有効になっている
- エディタ右下のアイコンを押すか、VisualStudioで"ctrl+alt+F11"

![](https://i.imgur.com/iBVLRsn.png)

#### 注意点

- クラスを追加してLiveCodingすると、エディタを再起動したときにクラスが消えてる
- これは、実際に書いたコードがなくなったわけではない
- LiveCodingは、UEエディタの**セッション中のみ有効なパッチファイルを更新することで変更を反映している**ため、エディタを閉じるとパッチの情報が無効になり、作ったクラスがなかったことにされる
  - プロジェクトを読み込んだときに読み込まれるのは`.dll`ファイルだが、LiveCodingするとこれの容量が全く変化していないことが分かる
- 書いたコード自体は残っているため、VisualStudioを開いて"ctrl+shift+b"でソリューションをビルドすればよい([参考](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_02_cpp-print_string#visualstudio%E3%81%A7build%E3%82%92%E8%A1%8C%E3%81%86))
  - ただし、LiveCodingが有効だとビルドできなくなっているため、LiveCodingアイコン横の...から Enable Live Coding をオフにするか、UEエディタを一度閉じる必要がある