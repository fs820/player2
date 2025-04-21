//------------------------------------
//
//����Đ�����[movie.cpp]
//Author:NULL
//
//------------------------------------

#include <mfapi.h>
#include <mfidl.h>
#include <mfplay.h>
#include <mfobjects.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <evr.h>
#include <assert.h>
#include <Windows.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "evr.lib")

// �����Ɖf���̃��f�B�A�^�C�v���`
const GUID MFMediaType_Audio = { 0x73647561, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
const GUID MFMediaType_Video = { 0x73646976, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };

// �G���[�`�F�b�N�p�}�N��
#define CHECK_HR(hr) if (FAILED(hr)) { return hr; }

//---------------------------------------------------------
// �g�|���W�̍쐬�w���p�[�֐�
//---------------------------------------------------------
HRESULT AddStreamToTopology(IMFTopology* pTopology, IMFMediaSource* pSource, IMFPresentationDescriptor* pPD, DWORD iStream, HWND hwnd)
{
    IMFStreamDescriptor* pSD = NULL;
    IMFActivate* pRendererActivate = NULL;
    BOOL fSelected = FALSE;
    HRESULT hr = S_OK;

    // �X�g���[���f�B�X�N���v�^���擾
    hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD);
    CHECK_HR(hr);

    if (fSelected)
    {
        // �X�g���[���^�C�v�𔻕�
        GUID majorType;
        IMFMediaTypeHandler* pHandler = NULL;
        hr = pSD->GetMediaTypeHandler(&pHandler);
        CHECK_HR(hr);

        hr = pHandler->GetMajorType(&majorType);
        pHandler->Release();
        CHECK_HR(hr);

        // �f���X�g���[���Ȃ�EVR���g���A�����X�g���[���Ȃ�SAR���g��
        if (majorType == MFMediaType_Video)
        {
            // EVR�̃A�N�e�B�x�[�^���쐬
            hr = MFCreateVideoRendererActivate(hwnd, &pRendererActivate);
            CHECK_HR(hr);
        }
        else if (majorType == MFMediaType_Audio)
        {
            // SAR�̃A�N�e�B�x�[�^���쐬
            hr = MFCreateAudioRendererActivate(&pRendererActivate);
            CHECK_HR(hr);
        }

        // �g�|���W�Ƀ\�[�X�m�[�h��ǉ�
        IMFTopologyNode* pSourceNode = NULL;
        hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pSourceNode);
        CHECK_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
        CHECK_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
        CHECK_HR(hr);

        hr = pSourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
        CHECK_HR(hr);

        hr = pTopology->AddNode(pSourceNode);
        CHECK_HR(hr);

        // �g�|���W�Ƀ����_���[�m�[�h��ǉ�
        IMFTopologyNode* pOutputNode = NULL;
        hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pOutputNode);
        CHECK_HR(hr);

        hr = pOutputNode->SetObject(pRendererActivate);
        CHECK_HR(hr);

        hr = pTopology->AddNode(pOutputNode);
        CHECK_HR(hr);

        // �\�[�X�m�[�h�ƃ����_���[�m�[�h��ڑ�
        hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
        CHECK_HR(hr);

        pSourceNode->Release();
        pOutputNode->Release();
    }

    pSD->Release();
    if (pRendererActivate) pRendererActivate->Release();
    return hr;
}

//---------------------------------------------------------
// �g�|���W�̍쐬�֐�
//---------------------------------------------------------
HRESULT CreatePlaybackTopology(IMFMediaSource* pSource, HWND hwnd, IMFTopology** ppTopology)
{
    IMFTopology* pTopology = NULL;
    IMFPresentationDescriptor* pPD = NULL;
    HRESULT hr = S_OK;

    // �g�|���W���쐬
    hr = MFCreateTopology(&pTopology);
    CHECK_HR(hr);

    // �v���[���e�[�V�����f�B�X�N���v�^�̎擾
    hr = pSource->CreatePresentationDescriptor(&pPD);
    CHECK_HR(hr);

    // �e�X�g���[���ɑ΂��ăm�[�h���쐬���A�g�|���W�ɒǉ�
    DWORD cStreams = 0;
    hr = pPD->GetStreamDescriptorCount(&cStreams);
    CHECK_HR(hr);

    for (DWORD i = 0; i < cStreams; i++)
    {
        hr = AddStreamToTopology(pTopology, pSource, pPD, i, hwnd);
        CHECK_HR(hr);
    }

    *ppTopology = pTopology;
    pPD->Release();
    return hr;
}

//---------------------------------------------------------
// ���[�r�[�Đ��֐�
//---------------------------------------------------------
HRESULT PlayVideo(HWND hwnd, const wchar_t* videoFilePath)
{
    IMFMediaSession* pSession = NULL;
    IMFMediaSource* pSource = NULL;
    IMFTopology* pTopology = NULL;
    IMFSourceResolver* pResolver = NULL;
    IMFMediaEvent* pEvent = NULL; // ������ pEvent ���`
    MediaEventType meType; // ������ meType ���`
    HRESULT hr = S_OK;

    // Media Foundation �̏�����
    hr = MFStartup(MF_VERSION);
    CHECK_HR(hr);

    // �\�[�X���]���o�[�̍쐬
    hr = MFCreateSourceResolver(&pResolver);
    CHECK_HR(hr);

    // ���f�B�A�\�[�X�̍쐬
    MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;
    IUnknown* pSourceUnk = NULL;
    hr = pResolver->CreateObjectFromURL
    (
        videoFilePath,                    // �Đ����铮��t�@�C���̃p�X
        MF_RESOLUTION_MEDIASOURCE,        // ���f�B�A�\�[�X���擾
        NULL,
        &ObjectType,
        &pSourceUnk
    );
    CHECK_HR(hr);

    // �\�[�X�̎擾
    hr = pSourceUnk->QueryInterface(IID_PPV_ARGS(&pSource));
    pSourceUnk->Release();
    CHECK_HR(hr);

    // ���f�B�A�Z�b�V�����̍쐬
    hr = MFCreateMediaSession(nullptr, &pSession);
    if (FAILED(hr) || pSession == nullptr)
    {
        return hr;
    }

    // �g�|���W�̍쐬
    hr = CreatePlaybackTopology(pSource, hwnd, &pTopology);
    CHECK_HR(hr);

    // �g�|���W���Z�b�V�����ɐݒ�
    hr = pSession->SetTopology(0, pTopology);
    if (FAILED(hr) || pTopology == nullptr)
    {
        return hr;
    }

    if (pSession == nullptr)
    {
        return hr;
    }

    // �Đ��� 0 �b����J�n����ꍇ
    PROPVARIANT varStart;
    PropVariantInit(&varStart);
    varStart.vt = VT_EMPTY; // 0 �b����̍Đ����w��

    hr = pSession->Start(nullptr, &varStart);  // ��2������ varStart ��n��
    PropVariantClear(&varStart);

    if (FAILED(hr)) 
    {
        return hr;
    }

    // �C�x���g���[�v�ōĐ���Ԃ��Ď�
    while (true)
    {
        hr = pSession->GetEvent(0, &pEvent);
        CHECK_HR(hr);

        hr = pEvent->GetType(&meType);  // ������ meType ���g�p
        CHECK_HR(hr);

        if (meType == MESessionEnded) 
        {
            break;  // �Đ����I�������烋�[�v�𔲂���
        }

        pEvent->Release();
    }

    // �Z�b�V�����̏I��
    hr = pSession->Close();
    CHECK_HR(hr);

    // ���f�B�A�Z�b�V�����̏I���m�F
    hr = pSession->GetEvent(0, &pEvent);
    CHECK_HR(hr);

    hr = pEvent->GetType(&meType);  // �����ł� meType ���g�p
    CHECK_HR(hr);

    if (meType == MESessionClosed) 
    {
        // �Z�b�V�������I������
    }

    pEvent->Release();

    // ���\�[�X���
    if (pSource) pSource->Shutdown();
    if (pSession) pSession->Shutdown();
    if (pResolver) pResolver->Release();
    if (pTopology) pTopology->Release();
    if (pSession) pSession->Release();
    if (pSource) pSource->Release();

    // Media Foundation �̏I��
    MFShutdown();
    return hr;
}