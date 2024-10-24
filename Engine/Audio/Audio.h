#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

/**
* @file Audio.cpp
* @brief オーディオ管理クラス
*/
#pragma comment(lib,"xaudio2.lib")

/// <summary>
/// 音声データを表す構造体
/// </summary>
struct SoundData {
    /// <summary>
    /// 波形フォーマット
    /// </summary>
    WAVEFORMATEX wfek;
    /// <summary>
    /// バッファの先頭アドレス
    /// </summary>
    BYTE* pBuffer;
    /// <summary>
    /// バッファのサイズ
    /// </summary>
    unsigned int bufferSize;
};

/// <summary>
/// オーディオの管理を行うクラス
/// </summary>
class Audio
{
public:
    /// <summary>
    /// シングルトンのインスタンスを取得する
    /// </summary>
    /// <returns>Audioクラスのインスタンス</returns>
    static Audio* GetInstance();

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Audio() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Audio() = default;

    /// <summary>
    /// コピー代入演算子を削除（コピー不可）
    /// </summary>
    /// <param name="other">コピー元のオブジェクト</param>
    /// <returns>コピー不可</returns>
    const Audio& operator=(const Audio&) = delete;

    /// <summary>
    /// オーディオシステムの初期化を行う
    /// </summary>
    void Initialize();

    /// <summary>
    /// オーディオシステムの更新処理を行う
    /// </summary>
    void Update();

    /// <summary>
    /// オーディオシステムのリソースを解放する
    /// </summary>
    void Release();

    /// <summary>
    /// チャンクヘッダを表す構造体
    /// </summary>
    struct ChunkHeader {
        /// <summary>
        /// チャンク毎のID
        /// </summary>
        char id[4];
        /// <summary>
        /// チャンクサイズ
        /// </summary>
        int32_t size;
    };

    /// <summary>
    /// RIFFヘッダチャンクを表す構造体
    /// </summary>
    struct RiffHeader {
        /// <summary>
        /// "RIFF"チャンク
        /// </summary>
        ChunkHeader chunk;
        /// <summary>
        /// "WAVE"タイプ
        /// </summary>
        char type[4];
    };

    /// <summary>
    /// FMTチャンクを表す構造体
    /// </summary>
    struct FormatChunk
    {
        /// <summary>
        /// "fmt "チャンク
        /// </summary>
        ChunkHeader chunk;
        /// <summary>
        /// 波形フォーマット
        /// </summary>
        WAVEFORMATEX fmt;
    };

    /// <summary>
    /// WAVファイルを読み込む
    /// </summary>
    /// <param name="filename">読み込むファイル名</param>
    /// <returns>サウンドデータのハンドル</returns>
    static uint32_t SoundLoadWave(const char* filename);

    /// <summary>
    /// サウンドデータを解放する
    /// </summary>
    /// <param name="audioHandle">解放するサウンドデータのハンドル</param>
    static void SoundUnload(uint32_t audioHandle);

    /// <summary>
    /// 音声を再生する
    /// </summary>
    /// <param name="xAudio2">IXAudio2のインスタンス</param>
    /// <param name="audioHandle">再生するサウンドデータのハンドル</param>
    /// <param name="loopFlag">ループ再生するかどうかのフラグ</param>
    /// <param name="volume">音量（0.0f～1.0f）</param>
    static void SoundPlayWave(IXAudio2* xAudio2, uint32_t audioHandle, bool loopFlag, float volume);

    /// <summary>
    /// 音声の再生を停止する
    /// </summary>
    /// <param name="xAudio2">IXAudio2のインスタンス</param>
    /// <param name="audioHandle">停止するサウンドデータのハンドル</param>
    static void SoundStopWave(IXAudio2* xAudio2, uint32_t audioHandle);

    /// <summary>
    /// 音声をループ再生する
    /// </summary>
    /// <param name="xAudio2">IXAudio2のインスタンス</param>
    /// <param name="soundData">再生するサウンドデータ</param>
    static void SoundLoopWave(IXAudio2* xAudio2, const SoundData& soundData);

    /// <summary>
    /// 音量を設定する
    /// </summary>
    /// <param name="audioHandle">設定するサウンドデータのハンドル</param>
    /// <param name="volume">音量（0.0f～1.0f）</param>
    static void SetVolume(uint32_t audioHandle, float volume);

    /// <summary>
    /// IXAudio2のインスタンスを取得する
    /// </summary>
    /// <returns>IXAudio2のComPtr</returns>
    static Microsoft::WRL::ComPtr<IXAudio2> GetIXAudio() { return xAudio2_; };

private:
    /// <summary>
    /// サウンドデータのハンドル
    /// </summary>
    static uint32_t soundHandle;

    /// <summary>
    /// サウンドデータの最大数
    /// </summary>
    static const uint32_t soundDataMaxSize = 100;

    // namespace省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    /// <summary>
    /// IXAudio2のインスタンス
    /// </summary>
    static ComPtr<IXAudio2> xAudio2_;

    /// <summary>
    /// マスターボイス
    /// </summary>
    IXAudio2MasteringVoice* masterVoice_;

    /// <summary>
    /// ソースボイスの配列
    /// </summary>
    static IXAudio2SourceVoice* pSourceVoice[soundDataMaxSize];

    /// <summary>
    /// サウンドデータの配列
    /// </summary>
    static SoundData soundData[soundDataMaxSize];
};
