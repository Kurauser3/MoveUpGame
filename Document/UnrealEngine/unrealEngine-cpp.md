# UnrealEngineでのC++

- C++でUE開発をする時のいろは
- Unreal C++ と言われているらしいので、検索するときなどはそのように入力する
- 互換性のためにUE独自のライブラリが採用されているらしく、**標準ライブラリは使えない**

---
## 関連

- [[unrealEngine]]
- [[unrealEngine-cpp-lib]]
- [[unrealEngine-cpp-actor]]
- [[unrealEngine-cpp-component]]
- [[unrealEngine-cpp-material]]

---
## VisualStudioとの統合

- [[unrealEngine#VisualStudioとの統合]]

---
## リファレンス

- [フレームワークの大まかな使い方](https://docs.unrealengine.com/5.2/en-US/gameplay-framework-in-unreal-engine/)
- [Engine](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/)
  - ゲームプレイのフレームワークが実装されている

---
## マクロ

- UEではえぐい数のマクロが定義されていて、コードの中に割と登場するため、使うやつは説明を載せとく
- [UEリフレクションシステム](https://docs.unrealengine.com/5.0/ja/reflection-system-in-unreal-engine/)とかいうのを実現するための仕組みらしい([[programming#リフレクション |リフレクションとは]])
  - [ブログでの説明](https://www.unrealengine.com/ja/blog/unreal-property-system-reflection)
    - C++は標準ではリフレクションをサポートしていないので、UnrealEngineが無理やりリフレクションの仕組みを作り、その結果がマクロによるアノテーション
      - アノテーションをつけられたオブジェクトが、リフレクションシステムから見えるようになっている
  - これにより、**ガベージコレクション**ができたり、**ブループリントと連携**したりすることができる
- [メタデータ指定子](https://docs.unrealengine.com/5.0/ja/metadata-specifiers-in-unreal-engine/)と呼ばれるものをマクロに付与することで、対象のエディタ上での挙動がより細かに設定できる
  - UClasses、 UFunctions、UProperties、 UEnums、および UInterfaces を宣言するときに使用する

### TEXT

- [公式ドキュメント](https://docs.unrealengine.com/4.27/ja/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/)
  - 一番下のエンコードのとこに書いてる
- C++の文字列リテラルをそのまま使うとANSIエンコードされ、サポートされる文字がかなり制限される
- TEXTマクロを使うとなんか知らんけどその辺が改善されるらしい

### UCLASS

### USTRUCT

### UENUM

- [列挙型の値にエディタ上の表示名をつける](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_001/viewer/chap_02_cpp-enumeration#c%2B%2B%E3%81%A7%E4%BD%9C%E6%88%90%E3%81%97%E3%81%9Fenumeration%EF%BC%88%E5%88%97%E6%8C%99%E5%9E%8B%EF%BC%89%E3%81%AEdisplayname%E3%82%92%E8%A8%AD%E5%AE%9A%E3%81%99%E3%82%8B)

### UPROPERTY

- [公式ドキュメント](https://docs.unrealengine.com/5.0/ja/unreal-engine-uproperties/)
- UEエディタ(Blueprint)で、C++クラスのプロパティを変更できるようにするかどうかなどの設定ができる
- プロパティを宣言する直前にこのマクロを書くことでBlueprintが認識できる
- 実際にやってることはBlueprintに認識させるだけではないらしく、[[#UObject]]継承のクラスのポインタにはつけておかないと[まずいこと](https://qiita.com/okqi/items/02327f965578ee1c379f)が起こる
- `UPROPERTY(プロパティ識別子, meta=(メタデータ指定子))`

#### プロパティ識別子

- [よく使うやつだけまとめてくれた記事](https://qiita.com/bigengelt/items/b17545fffe7b8d69e5e8)
- [公式](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/unreal-engine-uproperties#%E3%83%97%E3%83%AD%E3%83%91%E3%83%86%E3%82%A3%E6%8C%87%E5%AE%9A%E5%AD%90)
- 自分でよく使うやつだけリスト化しておく

| プロパティ識別子               | 意味                                                            |
| ---------------------- | ------------------------------------------------------------- |
| EditAnywhere           | エディタで編集可能                                                     |
| VisibleAnywhere        | エディタにプロパティは表示されるが、設定は変更できない                                   |
| Category="カテゴリ名"       | Blueprintで編集する場合のカテゴリ(グループ)を指定する                              |
| BlueprintReadOnly      | Blueprintで読み取れるが編集できない                                        |
| BlueprintReadWrite     | Blueprintで読み書き可能                                              |
| BlueprintAssignable    | マルチキャストデリゲートにつける。ブループリントで関数を割り当てられるようにする                      |
| BlueprintAuthorityOnly | マルチキャストデリゲートにつける。`BlueprintAuthorityOnly`タグが付いたイベントだけ割り当てができる |
| BlueprintCallable      | マルチキャストデリゲートにつける。ブループリントで呼び出せるようになる                           |

### UFUNCTION

- [公式ドキュメント](https://docs.unrealengine.com/4.27/ja/ProgrammingAndScripting/GameplayArchitecture/Functions/)
- 関数宣言の上の行に入れることで、関数をブループリントでも使えるようにする
  - この関数を`UFunctions`というらしい
- `UFunctions`はデリゲートにバインドすることができる
- デリゲート関数を宣言する場合は[[#UDELEGATE]]のほうを使用する

### UDELEGATE

- [公式ドキュメント](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Delegates/)
- デリゲート型をリフレクションシステムに追加したいときに使うっぽい
  - [[#DECLARE_DELEGATE]]の上の行に追加して使う
- メタデータは[[#UFUNCTION]]と同じように指定できるらしい

```cpp
// sample
UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FInstigatedAnyDamageSignature, float, Damage, const UDamageType*, DamageType, AActor*, DamagedActor, AActor*, DamageCauser);
```

### DECLARE_DELEGATE

- [公式ドキュメント](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/Delegates/)
  - ダイナミックデリゲート、マルチキャストデリゲートなど、種類によってマクロが変わるみたいなのでドキュメント見て適当なものを使う
- C#の`delegate`のようなことをC++で実現する
  - [参考動画](https://www.youtube.com/watch?v=2DYimOndAz4)
  - デリゲートは関数への参照を表す(?)型
  - デリゲートのインスタンスはオブジェクトなので、他の関数に引数として渡すことができる(普通の関数にはできない)
    - JavaScriptとかだと普通に関数を関数に渡すということができるが、いくつかの言語ではそれがデリゲートでしかサポートされていない。C#もその一つ
  - デリゲートは複数の関数を連結して(一つのデリゲートが複数の関数への参照を持って)一度に呼び出すことができる
- [[#動的デリゲート型を宣言しようとしたときにインテリセンスがエラーを吐く(VisualStudio) |なんかエラーになったとき]]

---
## 型

- 割と数があるので[[unrealEngine-cpp-type]]で記述することにした

---
## クラス

- [[unrealEngine-cpp-class]]でまとめる

---
## デバッグ

### パッケージ化後

- [UE4 パッケージ済み実行ファイルをVisualStudioでデバッグする方法](https://historia.co.jp/archives/4496/)
- Project Settings > Packagingの"Include Debug Files"をON
- パッケージ化を行う
  - 実行ファイルと同じフォルダに`.pdb`ファイルが作成される
- VisualStudioでビルド構成を変更する
  - "Editor"と付いた構成でビルドするとだめらしい(ほんとか?)ので、"Development"などに変更する
  - プロジェクトプロパティでデバッグ > コマンドにパッケージ化されたときの`.exe`を指定する

  ![[Pasted image 20240722034635.png]]

- VisualStudioからデバッグを開始するとデバッグ実行ができる
- 試してないけど[これ](https://colory-games.net/site/ue5-debug_packaged_project/)もできるっぽい(というかこっちのほうが新しい記事なので今後こっちでまとめる)

---
## ビルド

### モジュール

- 用意されているモジュールの種類は[[unrealEngine-cpp-module]]
- [Unreal Engine におけるモジュール構造とUBTの挙動について - strv.dev](https://strv.dev/blog/unrealengine--the-directory-structure-of-modules-in-unrealengine/)
- UEでC++を使うときに意識する必要がある概念
- UEではモジュールと呼ばれる単位で機能が分割されている
  - [[nodejs]]でインストールして使う`node_modules`と同じようなイメージ
- モジュールの仕組みは、**Unreal Build Tool (UBT)** と呼ばれるC#製のツールで実装されている
- **UBTはモジュール同士の依存関係を解決し、ビルド時のリンク範囲を決定する**

#### メリット

- [Unreal Engine のモジュール | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/unreal-engine-modules?application_version=5.2)
- モジュールに分けることで機能をカプセル化してコードを隠すことができる
- モジュールごとに異なるコンパイルユニットとして認識され、変更があったモジュールだけコンパイルすることが可能になり、ビルド時間が短縮される
- プロジェクトで使用しないモジュールはコンパイルから除外される
  - プラットフォーム毎に使用するモジュールを変更してコンパイルすることもできる

#### モジュールのディレクトリ構成

- モジュールは`Source`フォルダ内に配置されている

```
Source
├── ModuleA
│   ├── ModuleA.Build.cs
│   └── [ソースを置く場所]
└── ModuleB
    ├── ModuleB.Build.cs
    └── [ソースを置く場所]
```

- 階層に関してはよくわからないが、`.Build.cs`と同じフォルダにあるソースが一つのまとまりとして認識される?

#### .Build.cs

- C#で書かれるビルドの設定ファイルみたいなもの
- [[nodejs-npm]]でいうところの`package.json`みたいな感じ?
- 例えば以下のような感じ

```cs
using UnrealBuildTool;

public class ExampleModule: ModuleRules
{
	public ExampleModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine"
		});
	}
}
```

- この`ExampleModule`が`PublicDependencyModuleNames`に追加されている`Core`, `CoreUObject`, `Engine`の3モジュールに依存する(**≒ヘッダファイルにアクセスできる**)という意味の内容になっている
  - C++を**ビルドしたときにリンク関係のエラーが出たら`.Build.cs`で依存関係を記述していない可能性が高い**
  - [[#ヘッダファイルを公開する側の設定 |後述の設定]]ができてないとヘッダファイルにアクセスできないので注意

##### ヘッダファイルを公開する側の設定

- モジュールが**別のモジュールにヘッダファイルを公開するためには、`.Build.cs`でインクルードパスを指定する必要がある**
- ただし**予めデフォルトインクルードパスというのが指定されている**ため、カスタムする必要がなければ特に作業は要らない
- デフォルトインクルードパスは以下(参考サイトからコピペ)

| デフォルトインクルードパス | 効果                                                                                                                                                        |
| ------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Classes       | `Classes` ディレクトリを参照してきたモジュールにインクルードパスとして公開する。`Classes` 内のヘッダは依存元モジュールから参照可能になる。                                                                           |
| Public        | `Classes` ディレクトリと同様。ただし、`bNestedPublicIncludePaths`オプションが`true`になっている場合においては、`Public`ディレクトリ内部のディレクトリが再帰的に公開リストに追加される。                                     |
| Internal      | 参照してきた[[#モジュールのスコープ]]が、自分属するスコープと同じか、いずれかの親スコープに含まれている(つまり、自分と同じか、より内部のモジュールである)場合にのみ、`Internal`ディレクトリをインクルードパスとして公開する。                                   |
| Private       | `Private`は、このディレクトリを含むモジュール自身をコンパイルする際、モジュール内部からインクルード可能なインクルードパスとしてプライベートなコンパイル環境に追加される。  <br>他のモジュールに依存されたとしても、`Private`がインクルードパスとして他のモジュールに公開されることはない。 |

- カスタム設定が必要な時は以下のように記述する

```cs
using UnrealBuildTool;

public class ExampleModule: ModuleRules
{
	public ExampleModule(ReadOnlyTargetRules Target) : base(Target)
	{
		// 略
		PublicIncludePaths.AddRange(new string[] {
			/* 参照してきたモジュールに公開したいディレクトリを指定 */
		});
		InternalncludePaths.AddRange(new string[] {
			/* 参照してきたモジュールが自分と同じスコープか、
			より内部（親方向）のモジュールであれば公開したいディレクトリを指定 */
		});
		PrivateIncludePaths.AddRange(new string[] {
			/* このモジュール自身のビルドでインクルードパスとして利用したい内部のディレクトリを指定 */
		});
		// 略
	}
}
```

##### .Build.csに記述できるプロパティ一覧

- [Unreal Engine のモジュールのプロパティ | Unreal Engine 5.4 ドキュメンテーション | Epic Developer Community (epicgames.com)](https://dev.epicgames.com/documentation/ja-jp/unreal-engine/module-properties-in-unreal-engine?application_version=5.2)
- 内容が変わってるかもしれないので使用しているUEバージョンのドキュメントを参照

#### モジュールのスコープ

- 依存関係の制約に利用される概念([[#ヘッダファイルを公開する側の設定 |上述]]の`Internal`インクルードパスの制約で使用されている)
- 以下の6つのスコープがあるらしく、`Engine`がすべてのスコープの大元になっている

```
Engine
↑
Engine Plugins
↑
Engine Programs
↑
Marketplace
↑
Project
↑
Plugin
```

- 以下の表は参考にしたサイトのコピペ

| スコープ            | なにそれ                                                                                                                                                                           |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Engine          | エンジンが直接保持しているモジュール。 `Engine/Source` 以下の、 `Runtime`, `Developer`, `Editor`, `ThirdParty` などに配置されたモジュールはこのスコープに属する。                                                              |
| Engine Plugins  | エンジンがビルトインしているプラグインが保持するモジュール。 `Engine/Plugins` 以下にあるプラグインが持つモジュールはこのスコープに属する。                                                                                                 |
| Engine Programs | エンジンが保持しているスタンドアロン系のツールが保持するモジュール。 `Engine/Source/Programs` 以下にあるモジュールはこのスコープに属する。 `UnrealHeaderTool` などはこれ。                                                                   |
| Marketplace     | マーケットプレイスから入手され、ランチャーによって追加されたプラグインが保持するモジュール。 `Engine/Plugins/Marketplace` 以下にあるプラグインが持つモジュールはこのスコープに属する。                                                                     |
| Project         | プロジェクト側で直接保持するモジュール。 `[ProjectName]/Source` 以下にあるプロジェクトのモジュールと、 `[ProjectName]/Plugins` 、 `.uproject` ファイルで `AddtionalPluginDirectories`に指定されたディレクトリにあるプラグインのモジュールはこのスコープに属する。 |
| Plugin          | 外部プラグイン(Foreign plugin)が保持するモジュール。例えば UBT に `-Plugin` オプションで特定の `.uplugin` を指定して渡したようなプラグイン) が保持するモジュールはこのスコープに属する。                                                            |

### インクルードパス

- [コンパイルオプション](https://qiita.com/suzuki_takashi/items/c2f0301e7db7f6761c45#cpp%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB-%E3%82%B3%E3%83%B3%E3%83%91%E3%82%A4%E3%83%AB%E3%82%AA%E3%83%97%E3%82%B7%E3%83%A7%E3%83%B3)
  - `*.cpp.obj.response`の中で`/I [Path]`で指定されている
    - [インクルードパスの設定方法](https://programming-place.net/ppp/contents/visualstudio/include_path.html)

---
## パッケージ化

- [パッケージ化のためのProject Settings｜Unreal Engine 5で動かす C++ & Blueprint (zenn.dev)](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_002/viewer/chap_01-06_cpp-package_project_settings)
- [Windows（64bit）でパッケージ化する｜Unreal Engine 5で動かす C++ & Blueprint (zenn.dev)](https://zenn.dev/posita33/books/ue5_starter_cpp_and_bp_002/viewer/chap_01-06_cpp-package_win_64bit#%E5%AE%9F%E8%A1%8C%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%82%92%E3%83%97%E3%83%AC%E3%82%A4%E3%81%97%E3%81%A6%E7%A2%BA%E8%AA%8D%E3%81%99%E3%82%8B)

### ゲームの開始レベル(マップ)

- ProjectSetting > Project > Maps&Modes > GameDefaultMapで指定したレベルがゲーム起動時のマップになる
- タイトル画面とか

### ゲームに登場するレベルの設定

- **明示的にどのレベルを使用するかセットしないといけない**らしい

![[Pasted image 20240722091251.png]]

---
## トラブルシューティング

- [[unrealEngine-toroubleShooting]]