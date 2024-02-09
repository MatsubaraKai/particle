#pragma once
//シーン名を列挙型(Enum)で定義
enum SCENE { TITLE,SELECT,STSGE1,STAGE2,CLEAR };

// シーン内での処理を行う基底クラス
class IScene{
protected:
	// シーン番号を管理する変数
	static int sceneNo;
public:
	// 継承先で実装される関数
	/// 抽象クラスなので純粋仮想関数とする
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Release() = 0;
	virtual int GameClose() = 0;

	// 仮想デストラクタを用意しないと警告される
	virtual ~IScene();

	// シーン番号のゲッター
	static int GetSceneNo();
	static void SetSceneNo(int sceneno);

	
};

