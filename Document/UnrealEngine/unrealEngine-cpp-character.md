# Unreal C++ のキャラクター

- 自身が操作するキャラクターとして使われることが多い
- 歩行能力(コンポーネント)を持つ[[unrealEngine#ポーン |ポーン]]

---
## 関連

- [[unrealEngine-cpp]]
- [[unrealEngine-cpp-actor]]
- [[unrealEngine-cpp-component]]

---
## ACharacter

- `APawn`から継承している
- 独自で備えているコンポーネントとして以下がある(「Unreal Engine 5 で極めるゲーム開発」P181より)
  - CapsuleComponent
    - 当たり判定に使用されるカプセル型の範囲
    - 厳密さを求める場合はメッシュの当たり判定を使用することもあるが、基本的にはこの簡易な形状で当たり判定をすることで処理不可を低減する
  - ArrowComponent
    - エディタ上で正面を表すためのガイドで、ゲーム中は非表示
  - Mesh
    - 見た目
  - CharacterMovement
    - 移動処理を担うコンポーネント