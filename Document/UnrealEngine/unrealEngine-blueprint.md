# Blueprint

- UnrealEngine独自のスクラッチ的なプログラミングツール

---
## 関連

- [[unrealEngine]]

---
## Component

- ブループリントエディタでは、アクタが持っているコンポーネントが階層構造で表示されている
- ルートコンポーネントの[[unrealEngine#トランスフォーム |トランスフォーム]]を基準として、そのほかのコンポーネントのトランスフォームを相対値で指定する
  - **スポーンするときに指定するトランスフォームは、ルートコンポーネントのトランスフォーム**
  - **ルートコンポーネントはそのブループリントの基準位置となるため、ブループリントエディタでは移動ができない**(移動に意味がない)
    - [参考](https://www.reddit.com/r/unrealengine/comments/tbgojo/for_some_reason_i_cant_change_position_or/)

---
## Construction Script

- [参考](https://zenn.dev/daichi_gamedev/books/unreal-engine-tutorial-2/viewer/blueprint2-constructionscript)
- アクタが配置されたり、エディタで変数やプロパティが変更されたりしたときに実行される処理
- ゲーム中に動くものではないため、ゲーム中に実行しなくてもよい(ゲーム開始時には確定している)内容については、こちらに記述しておくと負荷を減らせる
