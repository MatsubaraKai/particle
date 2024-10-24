#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include <dxcapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

/**
* @file ResourceObject.h
* @brief リソースオブジェクトを管理するクラス
*/
class ResourceObject
{
public:
    /// <summary>
    /// コンストラクタ。ID3D12ResourceのComPtrを受け取ってリソースを管理する。
    /// </summary>
    /// <param name="resource">管理するID3D12ResourceのComPtr</param>
    ResourceObject(Microsoft::WRL::ComPtr<ID3D12Resource> resource)
        :resource_(resource)
    {}

    /// <summary>
    /// デストラクタ。リソースを解放する。
    /// </summary>
    ~ResourceObject() {
        if (resource_) {
            resource_->Release();
        }
    }

    /// <summary>
    /// リソースを取得する。
    /// </summary>
    /// <returns>管理しているID3D12ResourceのComPtr</returns>
    Microsoft::WRL::ComPtr<ID3D12Resource> Get() { return resource_; }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
};
