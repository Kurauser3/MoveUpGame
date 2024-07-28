# UEで起こったトラブルと対応まとめ

- システムがエラー吐いたパターン、ゲームはできるけど思ったように動かなかったパターンなど

---
## システムエラー系

- エラー吐いてなんもできないやつ
- コンパイルエラーとか
- インテリセンスがおかしいとか

### 何もしてないのにGENERATED_BODY()が壊れた

- 突然Intellisenceが怒り始めたとき
- 多分`#include "xxxxxxx.generated.h"`の後に`#include`を追加している
- なぜなのかはまだ調べていないが、LiveCodingでコンパイルしたときに「**`.generated.h`の後にインクルードするな**」というエラーメッセージが出るため、UEのルールとして決まっていることだと思われる
  - これに当てはまらないケースは再起動で改善したという話がちらほら出てくる

![](https://i.imgur.com/f6BETr3.png)

### 動的デリゲート型を宣言しようとしたときにインテリセンスがエラーを吐く(VisualStudio)

- [実際に解決した方法](https://forums.unrealengine.com/t/declare-dynamic-delegate-threeparams-error/448468/3)
- [他の参考](https://qiita.com/loppta/items/2e77e537b4e6a98685b6)
- Intellisenceがたまに馬鹿になるので、以下のような方法で解決を試みる
  - VisualStudioの上部のツールバーから ビルド > ソリューションのリビルド を試す
    - ダメなら再生アイコン(ローカルWindowsデバッガ)の左隣にあるソリューションの構成を別のものに変更してからリビルドしてみる(直ったら元に戻す)
  - プロジェクトを右クリックし、ソリューションの再スキャン
  - `.vs`ディレクトリを削除
  - etc...(あとはネットで調べる)

---
## 実装ミス系

- なんかうまく動いてないやつ

### タイトルなどのUIからゲームのマップに遷移したときにマウスコントロールがおかしくなってる

- マウスを使ってUI操作させるためにInputModeを変更したままになってるかも

![](https://i.imgur.com/cEZ9DCs.png)

- UIを表示したときは`SetInputMode_Xxxx`を使ってUI操作ができるようにしてるはずなので、別の画面に遷移したときに適切なInputModeに戻さないといけない
- 同じように`SetInputMode_Xxxx`を使って戻せばいい
- [次のステージへ移行するとPCのマウス＆キー入力が無効になってしまう事象につきまして - International / 日本語 (Japanese) - Epic Developer Community Forums (unrealengine.com)](https://forums.unrealengine.com/t/pc/733329/2)
  - 同じようなことになったひとがやっぱり居る

#### 遷移したときにどいつが元に戻せばいいか問題

- [[unrealEngine-pattern#遷移したときにどいつが元に戻せばいいか問題]]

---
## 分からない系

- したいことは明白なのにUEの操作がわからないやつ
- [[unrealEngine-pattern]]に書いてるかもしれない

### デバッグしたいけど基本の文字列を画面に表示する方法が分からない

- [UE5/UE4 C++でデバッグ文字列（FString）をビューポート（Viewport）に表示する（UEngine::AddOnScreenDebugMessage） 凛(kagring)のUE5/UE4とゲーム制作と雑記ブログ (fc2.com)](http://kagring.blog.fc2.com/blog-entry-659.html)
- `GEngine->AddOnScreenDebugMessage`を使う
  - [[unrealEngine-cpp-class#GEngine#AddOnScreenDebugMessage]]
- `UKismetSystemLibrary::PrintString`でもいいけどいちいちincludeが必要なので非推奨
- [他のデバッグ方法のまとめ記事](https://dev.epicgames.com/community/learning/tutorials/dXl5/advanced-debugging-in-unreal-engine)

### アセットのパスが分からない

- C++で開発するときに何らかのアセットを呼び出す場合は、そのリソースのPathを指定する必要がある
  - 実際はハードコーディングを避けたほうが良いことが多いと思うので、[[unrealEngine-pattern#アセットの参照]]で言うように、**Blueprintで参照するアセットを決めたほうがよさそう**
- ContentBrowserのフォルダ構造とPathは関係ないっぽい
  - [ドキュメント](https://docs.unrealengine.com/4.26/en-US/Basics/AssetsAndPackages/)によると"The Asset Tree displays a list of folders in your content directories where .uasset files reside."(Contentディレクトリ内の`.uasset`ファイルが存在するフォルダの一覧が表示されている)
- Pathが分からない場合は、**ContentBrowserのアセットをマウスオーバーすると表示される**

### C++クラスを削除したい

- UEのくそなところ
- エディタでC++クラスの削除がサポートされていないので、いくつかの手順を踏む必要がある
  - 単純にソリューションやエクスプローラーから消すだけではコンテンツブラウザから消えない
- [Unreal Engine C++ Tutorial #7: Misc Removing C++ Classes - YouTube](https://www.youtube.com/watch?v=kU9EXDHPJiQ)
  - VisualStudioでソリューションから対象のファイルを取り除く
  - UEエディタを閉じておく
  - エクスプローラーで対象のファイルを削除する
  - プロジェクト直下(少なくともUE5では)の`Binaries`フォルダをフォルダ丸ごと削除
    - ここの`.dll`を読んでコンテンツブラウザに表示してるっぽい
  - VisualStudioでソリューションをビルドする
    - (リビルドじゃないとダメかも?)
  - UEエディタを開くとコンテンツブラウザから消えてるはず

#### そのほかの情報

- 上記の動画の内容に加えて
  - UEエディタのToolからRefleshVisualStudioProjectしておくといいかも
  - なんかリビルドでエラー吐くようになってたのが解消したので...
