# ソース概説

## 気にしたこと

- [クラスの役割](#各クラスの役割)
  - どのクラスが何を知っていれば良いのか
- 命名
  - アセットは[こちら](https://github.com/Allar/ue5-style-guide?tab=readme-ov-file#base-asset-name)を参考にした
  - C++クラスの命名についてはBlueprintの`BP`に習い、プレフィックスに`CPP`と付けておいた
  - C++コードの中身はUnrealEngineのオリジナルのコードを参考に命名を合わせた
    - 主なところはプレフィックス。bool型は`b`、デリゲートの型は`F`など...
- 参照はBlueprintが引き受けること
  - ハードコーディングするとPathの不一致に気づきにくいこと、アセットの移動時に追跡をしてくれないことなどから

## 課題

- Unrealエディタ側であまりプロパティを触らなかったので、`UPROPERTY`などのマクロ設定が適切にできているのか精査できていない
  - コーダー以外の人がどのようにプロパティをいじりたいのか、まだよく分かっていない
- コリジョン設定が上手く働いておらず、キャラクターが止まっているときの当たり判定イベントが発火しない
  - 代替の解決手段として暫定的に[オブザーバー](../../Source/ActionGame/Public/CPPGameObserver.h)を用意した
- ランダムにステージを生成する仕様のテストが十分でないこと
  - ランダムであるために実際にプレイするだけでは確認しきれないので、何らかのテストフレームワークを用いてテストを作る必要がある

## 各クラスの役割

- 主要なところだけ掲載

### AGameModeBase

- ゲームロジックを管理
- ゲーム内に登場するアクタをすべて知っていて、それらの相互作用を定義して良い
- 全てのゲームロジックを書くと巨大なクラスになるので、適宜コンポーネントやアクタに処理を任せて整理する

#### ACPPGameModeBase

- [ソース](../../Source/ActionGame/Public/CPPGameModeBase.h)
- 本ゲームのイベントハンドリングを殆ど受け持つ
  - ステージ生成をトリガーしたり、プレイヤーのキルを行ったりする

### AActor

- ゲーム内に配置されるオブジェクト
- 自身のメンバー変数としたアクター以外は、ゲームに登場するアクターを一切知らない
- [AGameModeBase](#agamemodebase)から一部のゲームロジックを引き受ける専用アクターと、ゲームの内容を(ほぼ[^1])知らない共有アクターで分けて考えるようにした

[^1]: 実際にはゲームのためにつくられているアクターなので、完全にゲームと切り離して考えられたアクターはない

#### ACPPRandomStageSpawner

- [ソース](../../Source/ActionGame/Public/CPPRandomStageSpawner.h)
- ゲームロジックを一部受け持っている専用アクター
  - ランダムなステージを生成するゲームロジック

#### ACPPStageFloor

- [ソース](../../Source/ActionGame/Public/CPPStageFloor.h)
- 共有アクター
- ステージ内に生成されるフロア
- ここから派生させてフロアの種類を増やせる

#### ACPPlayer

- [ソース](../../Source//ActionGame/Public/CPPPlayer.h)
- 共有アクター
- プレイヤーが操作するキャラクター(`ACharactor`から派生)

### AHUD

- 各レベルで、プレイヤー(コントローラー)毎に一つ?
- ゲームのUIあるいはステータスなどの表示、遷移を担当
- [UUserWidget](#uuserwidget)を管理する

#### ACPPGameHUD

- [ソース](../../Source/ActionGame/Public/CPPGameHUD.h)
- ゲームプレイレベルのUI表示
  - ゲームオーバー画面やジャンプのチャージ進捗など

#### ACPPMainMenuHUD

- [ソース](../../Source/ActionGame/Public/CPPMainMenuHUD.h)
- タイトル画面

### UUserWidget

- UIの操作によるイベント発行、表示するパラメーターの変更方法などを提供する
- [CPPGameOverWidget](../../Source/ActionGame/Public/CPPGameOverWidget.h)など
