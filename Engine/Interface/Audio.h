#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

#pragma comment(lib,"xaudio2.lib")
// 音声データ
struct SoundData {
	// 波形フォーマット
	WAVEFORMATEX wfek;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;

};
class Audio
{
public:
	static Audio* GetInstance();

	Audio() = default;
	~Audio() = default;
	const Audio& operator=(const Audio&) = delete;

	void Initialize();
	void Update();
	void Release();

	// チャンクヘッダ
	struct ChunkHeader {
		char id[4]; // チャンク毎のID
		int32_t size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk; // "fmt"
		WAVEFORMATEX fmt; // 波形フォーマット
	};

	

	static uint32_t SoundLoadWave(const char* filename);

	static void SoundUnload(uint32_t audioHandle);

	// 音声再生
	static void SoundPlayWave(IXAudio2* xAudio2, uint32_t audioHandle, bool loopFlag);

	// 音声再生
	static void SoundStopWave(IXAudio2* xAudio2, uint32_t audioHandle);
	static void SoundLoopWave(IXAudio2* xAudio2, const SoundData& soundData);

	static Microsoft::WRL::ComPtr<IXAudio2> GetIXAudio() { return xAudio2_; };

private:
	static uint32_t soundHandle;

	const static uint32_t soundDataMaxSize = 100;
	// namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	static ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

	static IXAudio2SourceVoice* pSourceVoice[soundDataMaxSize];
	static SoundData soundData[soundDataMaxSize];

};

