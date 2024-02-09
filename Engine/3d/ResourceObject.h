#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include <dxcapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
class ResourceObject
{
public:
	ResourceObject(Microsoft::WRL::ComPtr<ID3D12Resource> resource)
		:resource_(resource)
	{}
	~ResourceObject() {
		if (resource_) {
			resource_->Release();
		}
	}
	Microsoft::WRL::ComPtr<ID3D12Resource> Get() { return resource_; }
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
};

