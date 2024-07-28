# Unreal C++ のマテリアル

- C++でのマテリアルの扱い方を記載する

---
## 関連

- [[unrealEngine-cpp]]
- [[unrealEngine-cpp-component]]

---
## UMaterialInstance

- ゲーム中に変更**不可能**なマテリアル
- [[unrealEngine-cpp-component#GetMaterial |GetMaterial]]関数などで取得することができる

---
## UMaterialInstanceDynamic

- [使い方サンプル](https://qiita.com/unknown_ds/items/4173d42f8624e70d6475)
- ゲーム中に変更**可能**なマテリアル
- [[#UMaterialInstance]]をもとに作成することがほとんどだと思われる

```cpp
// 実際に使った時のコード
// まずStaticMeshから取得した静的マテリアルを元に動的マテリアルを作成する
UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create( Plane->GetMaterial(0), NULL);
// 動的マテリアルでパラメータを変更した後、元の静的マテリアルを上書きする
Material->SetVectorParameterValue(TEXT("Color"), FLinearColor::Yellow);
Plane->SetMaterial(0, Material);
// これで、ゲーム中にマテリアルの見た目(色やら質感やら)を変更することができる
```

### SetVectorParameterValue

- 新しいパラメータ(多分カラー系の変数)をセットする関数
- 使い方は上述のサンプルコードに

### K2_GetVectorParameterValue

- [リファレンス](https://docs.unrealengine.com/5.3/en-US/API/Runtime/Engine/Materials/UMaterialInstanceDynamic/K2_GetVectorParameterValue)
- 普通に`GetVectorParameterValue`を使おうとするとよくわからん引数を要求される
- [[#SetVectorParameterValue]]と対になるのはこいつっぽい