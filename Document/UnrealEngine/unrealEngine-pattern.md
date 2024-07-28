# UEの実装パターン

- できることが多い中で、どんな実装方法が嬉しいのか
- 実装のベストプラクティス的な情報を集めていきたい

---
## アセットの参照

### 基本的な実装パターン

- C++クラスで他のアセットを参照するとなると、アセットのパスをハードコーディングする必要がある
  - ただの文字列リテラルで指定するので、実際にそのパスが存在しなくても静的解析ではわからない
  - アセットのパスが変更されたときに追跡しないので変更が難しくなる可能性がある
- 代替案として、**C++クラスではアセットを参照するためのステートだけ持っておき、実際のアセットを指定するのはC++クラスを継承したBlueprintが担う**

### アセット参照の種類を使い分ける

- [第007回UE4のアセットの参照方法について、そのロードの違い | CC2の楽屋裏](https://www.cc2.co.jp/blog/?p=22302)
- **ハードリファレンス**と**ソフトリファレンス**に分類される

#### ハードリファレンス

- 直接参照
- **ハードリファレンスを持っているアセットがロードされたときに、参照先のアセットもロードされる**
- ロードのし忘れがなくなるが、実際には要らないものまでロードしてしまうかもしれない
- **確実に参照元と一緒に使うアセット**はこの参照で持っておくといいかも
- C++においてはソフトリファレンス以外の方法で参照を持つと、とりあえずハードリファレンスになるっぽい?
  - [[unrealEngine-cpp#TSubclassOf |TSubclassOf]]とか[[unrealEngine-cpp#TObjectPtr |TObjectPtr]]とか

#### ソフトリファレンス

- 関節参照
- ロードのし忘れやタイミングのミスがあるかもしれないが、必要なものを必要な時にロードできる
- **場合によっては使わないアセット**などはこちらの参照がいいかも
- C++においては`TSoftObjectPtr`あるいは`TSoftClassPtr`テンプレートで持っているステートがソフトリファレンスになる

---
## スポーン

### スポーンする位置をハードコーディングしたくない

- 何かしらのアクタがスポーンする座標をC++やBlueprint内で直接入力したくない
- できればレベルエディタ上で「スポーンするポイント」を表現する何かが欲しい
- C++やBlueprintでは、**「スポーンするポイントをワールドから探してそこにスポーンする」という処理にしたい**
- こういう場合は、**TargetPointアクタ**が便利
  - [公式ドキュメント](https://docs.unrealengine.com/4.27/ja/Basics/Actors/TargetPoint/)
  - ゲーム中は見えないようになっていて、ただゲームの裏側で使う座標を持ってるだけのアクタ
  - スポーンだけではなく、位置指定が必要な処理全般に有用

### スポーンする前にデフォルト値を変更したい

- [フォーラムの同じお悩み](https://forums.unrealengine.com/t/spawning-an-actor-with-parameters/329151/14)
- `SpawnActor()`に渡せるパラメータでアクタの持っているメンバ変数のデフォルトを変更できない(Templeteインスタンスを作って渡せば可能?)ので、別の方法が必要
- `SpawnActor()`でスポーンした後に変更する方法はある
  - 対象のアクタに`Initialize(...)`のようなセットアップ関数を持たせておき、スポーンした後にそれを呼び出す

```cpp
// SpawnActor後に初期値を設定
AYourActor* SpawnedActor = World->SpawnActor<AYourActor>(AYourActor::StaticClass(), SpawnTransform);
if (SpawnedActor)
{
    SpawnedActor->Initialize(InitialValue);
}
```

- 上記の方法だとステージに実体が現れてからその状態を変更するので、一瞬かもしれないが不正な状態のアクタがゲームに存在することになる
- また、調べてみたところ`BeginPlay()`がスポーンされた時点で終わってるので、**`BeginPlay()`の初期化に使用するメンバ変数を、スポーン後に変更したところで意味がない**
- 解決策として[こちら](https://forums.unrealengine.com/t/spawn-actor-with-dynamic-data/356847)の方法が紹介されている
- `UWorld`のメンバ関数に`SpawnActorDeferred()`が用意されており、これは**スポーンを任意のタイミングまで遅らせる**ことができる関数になっている
- 以下のように使用して問題を解決できる

```cpp
UWorld* World = GetWorld();
const FTransform SpawnLocAndRotation;
// スポーンをReady状態にしたアクタを取得する
AMyActor* MyActor = World->SpawnActorDeferred<AMyActor>
  (AMyActor::StaticClass(), SpawnLocAndRotation);
// スポーンの前にセットアップ関数を呼び出す
MyActor->Initialize( ... );
// アクタをスポーンする
MyActor->FinishSpawning(SpawnLocAndRotation);
```

- なおコンストラクタは上記の方法を使用しても`Initialize()`より先に実行される(インスタンスができてるので当たり前かもしれないが)

![](https://i.imgur.com/PmNJiyW.png)

---
## サイズや座標の取得

### アクタのサイズを取得する

- [How to measure static mesh height or width in units?](https://forums.unrealengine.com/t/how-to-measure-static-mesh-height-or-width-in-units/281055/6)
- [API](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/GameFramework/AActor/GetActorBounds?application_version=5.1)
- APIでは`Bounds`(境界)と表現されている
- `AActor::GetActorBounds`でアクタを完全に囲むボックスの`Extent`(ボックスの中心から端までの距離)が取得できる
- ボックスの中心座標`Origin`も返してくれるので、**アクタの端の座標**計算などもできる

```cpp
void GetActorBounds
(
    bool bOnlyCollidingComponents,
    FVector & Origin,
    FVector & BoxExtent,
    bool bIncludeFromChildActors
)
```

---
## プレイヤーの実装

### 入力を受け取ってアクションを起こすまで

#### Enhanced Input

- [Enhanced Input in Unreal Engine | Unreal Engine 5.4 Documentation | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine)
- プラグインの一つらしい
- 古い入力システムを使用したことがないのでよくわかっていないが、従来の方法よりも高度な機能が追加されているらしい
  - [フォーラム](https://forums.unrealengine.com/t/enhanced-input-or-normal-input/695016/3)には古い入力が推奨されないというような内容が書いてある
- TODO : 詳しい内容は別のファイルにまとめる

### プレイヤーのキル

- プレイヤーを殺す方法は主に2種類
  - `Destroy()`メソッドで消す
    - この場合はプレイヤーキャラクターが消滅
    - そのキャラクターが持ってる状態や関数にアクセスできなくなる?
  - `AController::UnPossess()`でプレイヤーのコントローラから制御を切り離す
    - プレイヤーの死体を残したいときや、何かしらのアニメーションの後に消滅してほしいときに使用
    - コントローラは`APawn::GetController()`でプレイヤーから入手可能

---
## メニュー系

- [How to Make a Simple Game Over Menu in Unreal Engine 5 (youtube.com)](https://www.youtube.com/watch?v=ZpQmL_TzSPo)
- ウィジェット(WidgetBlueprint)というのを使用するっぽい
- 公式のチュートリアルもある
  - [Unreal Engine の UMG の使用を開始する | Unreal Engine 5.2 ドキュメント | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/getting-started-with-umg-for-unreal-engine?application_version=5.2)

### 基本的なC++での実装

- [タイトル画面の入力処理を実装する｜Unreal Engine 5で動かす C++ & Blueprint (zenn.dev)](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_002/viewer/chap_01-05_cpp-ui_widget_main_menu_input)
- **UserWidgetクラス**を継承したC++クラスにインタラクションの処理を実装する
- Widgetに配置するボタンなどがコンポーネントとしてC++に公開されているので、それらをステートとして持つ(**C++はコンポーネントが指すUI部品の実体を知らなくていい。**>>実際にはどのボタンなのか...とか)
- C++クラスでインタラクションを実装したら、そのクラスを継承するWidgetBPを作り、実際のボタンなどを配置してUIを作る
  - この時。**C+＋クラスで実装したコンポーネントの変数名とWidgetBPで配置したUI部品の名前を合わせることで、初めて実体と紐づく**

### Widget(UI)とゲームマップの画面遷移

- レベルからレベルへの遷移と、レベル内での遷移があるけど大体同じ感じ
- 遷移の責任を持つべきクラス(基本はHUDクラス?)で、Widgetを表示する処理やWidgetのボタンを押した後の遷移処理を行う
- 画面遷移部分のサンプルは以下([参考](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_002/viewer/chap_01-05_cpp-ui_widget_game_pause_input))

```cpp
void AInGameHUD::DispPause(const bool IsPause)
{
	// PlayerControllerを取得する
	APlayerController* PlayerController = GetOwningPlayerController();

	if (IsPause)
	{
		// Pauseメニューを表示する
		PauseWidget->SetVisibility(ESlateVisibility::Visible);
		// UIモードに設定する
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseWidget, EMouseLockMode::DoNotLock, false);
		// GameをPause状態にする
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		// Mouseカーソルを表示する
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		// Pause状態を解除する
		// GameのPause状態を解除する
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		// Mouseカーソルを非表示にする
		PlayerController->SetShowMouseCursor(false);
		// GameのInput状態に戻す
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController, false);
		// Pauseメニューを折りたたみ状態にする
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
```

- Escキーが押されるなどのトリガーで上記関数を呼び出して画面遷移する

#### UIとゲーム間で遷移するときにどのレベルが入力モードを変えるのか

- 遷移前のやつが適切なInputModeに変えてから遷移するのか、それとも遷移先で変更するのか
- 現時点での見解では遷移先の責任
- **UIレベルがゲームレベル側のInputModeを知っている必要はない**と思うので、ゲームマップ側で責任をもってInputModeを設定して遊べるようにしてあげる
  - **逆に言うとゲーム側もUIレベルのInputModeは知らない**のでUIレベル側で適切に設定してから処理を開始する
- レベル間の遷移じゃなくても、例えばゲーム中のポーズ画面への遷移なども同じ考えでいいと思う
