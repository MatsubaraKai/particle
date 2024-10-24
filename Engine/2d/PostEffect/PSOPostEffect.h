#pragma once
#pragma once
#include <d3d12.h>
#include "DirectXCommon.h"
#include "PSOProperty.h"
#include "IPostEffectState.h"
#include "Vector3.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>

/**
* @file PSOPostEffect.h
* @brief PSOPostEffect
*/
class PSOPostEffect : public PSOProperty
{
public:
    /// <summary>
      /// インスタンスを取得する
      /// </summary>
      /// <returns>PSOPostEffectのインスタンス</returns>
    static PSOPostEffect* GetInstance();

    /// <summary>
    /// デフォルトコンストラクタ
    /// </summary>
    PSOPostEffect() = default;

    /// <summary>
    /// デフォルトデストラクタ
    /// </summary>
    ~PSOPostEffect() = default;

    /// <summary>
    /// 代入演算子を禁止
    /// </summary>
    /// <param name="other">コピー元のPSOPostEffect</param>
    /// <returns>代入不可</returns>
    const PSOPostEffect& operator=(const PSOPostEffect&) = delete;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Init();

    /// <summary>
    /// エフェクトの変更をチェックする
    /// </summary>
    void EffectChangeCheck();

    /// <summary>
    /// コマンドを設定する
    /// </summary>
    /// <param name="postProcess">ポストプロセスのポインタ</param>
    void SetCommand(PostProcess* postProcess);

    /// <summary>
    /// エフェクトを設定する
    /// </summary>
    /// <param name="num">エフェクトの番号</param>
    void SetEffect(int num);

    /// <summary>
    /// PSOプロパティを取得する
    /// </summary>
    /// <returns>PSOプロパティ</returns>
    PSOProperty GetProperty() { return property; }

private:
	HRESULT hr_;
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};


	// 描画先のRTVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	PSOProperty property;
	// エフェクトを保持するメンバ変数
	std::unique_ptr<IPostEffectState> effectArr_[PostEffectMode::kNumPostEffect];

	// どのステージを呼び出すかを管理する変数
	int currentEffectNo_ = 0;
	int prevEffectNo_ = 0;

};




