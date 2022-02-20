#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#include <stdio.h>

#include <random>

#include "cool_base.h"

#ifdef far
#undef far
#endif 
#ifdef near
#undef near
#endif 
#ifdef min
#undef min
#endif 
#ifdef max
#undef max
#endif 

struct Simulation {
    float  DT;
    float  MoveSpeed;
    float  SensorOffset;
    float  TurnAngle;

    float  RandomAutoTurnFactor;
    float  DecayRate;
    float  DiffuseRate;
    float  TrailWeight;

    float  SensorAngle;
    // float wasted
    // float wasted
    // float wasted

    int   Particles;
    int   W;
    int   H;
    int   DiffuseFilterHW;

    int   SensorSize;
    int   BorderRepulsionMinRange;
    int   BorderRepulsionStrength;
    // int wasted
};

internal
void
Configure(Simulation& sim)
{
    #include "hlsl_config_06.h"

    sim.Particles = ((sim.Particles + 31) / 32) * 32;
}

LRESULT WINAPI Win32_Window_Tick(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY)
        PostQuitMessage(1);
    return DefWindowProc(window, msg, wParam, lParam);
}

internal
ID3D11Texture2D*
MakeTexture(
    ID3D11Device* dx,
    Simulation& sim,
    int usage,
    int bind,
    int cpuAccess)
{
    ID3D11Texture2D* texture = {};

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = sim.W;
    desc.Height = sim.H;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = (D3D11_USAGE)usage;
    desc.BindFlags = bind;
    desc.CPUAccessFlags = cpuAccess;
    desc.MiscFlags = 0;

    HRESULT r = dx->CreateTexture2D(&desc, 0, &texture);
    if (FAILED(r)) {
        ExitProcess(1);
    }
    return texture;
}

template<class T>
internal
ID3D11Buffer*
MakeBuffer(
    ID3D11Device* dx,
    int count,
    int usage,
    int bind,
    int cpuAccess,
    int flags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
{
    ID3D11Buffer* buffer = {};

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = count * sizeof(T);
    desc.StructureByteStride = sizeof(T);
    desc.Usage = (D3D11_USAGE)usage;
    desc.BindFlags = bind;
    desc.CPUAccessFlags = cpuAccess;
    desc.MiscFlags = flags;

    HRESULT r = dx->CreateBuffer(&desc, 0, &buffer);
    if (FAILED(r)) {
        ExitProcess(2);
    }
    return buffer;
}

internal
void
MakeImage(Simulation const& sim, Image& image)
{
    i32 totalPixels = sim.W * sim.H;
    i32 totalSize = totalPixels * sizeof(i32);
    image.px = (i32*)VirtualAlloc(0, totalSize, MEM_COMMIT, PAGE_READWRITE);
}

internal
void
MakeImage(Simulation const& sim, ImageF& image)
{
    i32 totalPixels = sim.W * sim.H;
    i32 totalSize = totalPixels * sizeof(float);
    image.px = (float*)VirtualAlloc(0, totalSize, MEM_COMMIT, PAGE_READWRITE);
}

internal
void
TextureToImage(ID3D11DeviceContext* dxContext, Simulation& sim, ID3D11Texture2D* in, i32* out)
{
    D3D11_MAPPED_SUBRESOURCE mapped;
    dxContext->Map(in, 0, D3D11_MAP_READ, 0, &mapped);
    for (int y = 0; y < sim.H; ++y)
        for (int x = 0; x < sim.W; ++x) {
            char* textureBytes = (char*)mapped.pData;
            textureBytes += x * sizeof(float) + y * mapped.RowPitch;
            float* texturePixel = (float*)textureBytes;
            i32 gray = (i32)(MinF32(1, *texturePixel) * 255.f);
            i32 rgb = (gray << 16) | (gray << 8) | gray;
            out[x + y * sim.W] = rgb;
        }
    dxContext->Unmap(in, 0);
}

internal
void
TextureToImageF(ID3D11DeviceContext* dxContext, Simulation& sim, ID3D11Texture2D* in, float* out)
{
    D3D11_MAPPED_SUBRESOURCE mappedInTrailMap;
    dxContext->Map(in, 0, D3D11_MAP_READ, 0, &mappedInTrailMap);
    for (int y = 0; y < sim.H; ++y)
        for (int x = 0; x < sim.W; ++x) {
            char* textureBytes = (char*)mappedInTrailMap.pData;
            textureBytes += x * sizeof(float) + y * mappedInTrailMap.RowPitch;
            float* texturePixel = (float*)textureBytes;

            out[x + y * sim.W] = *texturePixel;
        }
    dxContext->Unmap(in, 0);
}

internal
ID3D11ComputeShader*
MakeShader(ID3D11Device* device, wchar_t const* fileName, char const* entryPoint)
{
    ID3D11ComputeShader* shader = {};
    UINT dxComputeShaderFlags = 0;
    // dxComputeShaderFlags |= D3DCOMPILE_DEBUG;
    dxComputeShaderFlags |= D3DCOMPILE_ENABLE_STRICTNESS;
    // dxComputeShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
    dxComputeShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
    // dxComputeShaderFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
    dxComputeShaderFlags |= D3DCOMPILE_ALL_RESOURCES_BOUND;

    ID3DBlob* dxComputeShaderCode = {};
    ID3DBlob* dxComputeShaderMessages = {};
    if (HRESULT r = D3DCompileFromFile(
            fileName,
            0,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint,
            "cs_5_0",
            dxComputeShaderFlags,
            0,
            &dxComputeShaderCode,
            &dxComputeShaderMessages);
        FAILED(r)) {
        char* buf = (char*)dxComputeShaderMessages->GetBufferPointer();
        OutputDebugStringA(buf);
        ExitProcess(4);
    }

    device->CreateComputeShader(
        dxComputeShaderCode->GetBufferPointer(),
        dxComputeShaderCode->GetBufferSize(),
        0,
        &shader);
    dxComputeShaderCode->Release();
    return shader;
}

internal
ID3D11ShaderResourceView*
MakeBufferSRV(ID3D11Device* device, ID3D11Buffer* buffer, int elements)
{
    ID3D11ShaderResourceView* bufferSRV = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};

    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement = 0;
    desc.Buffer.NumElements = elements;
    if (HRESULT r = device->CreateShaderResourceView(buffer, &desc, &bufferSRV);
        FAILED(r)) {
        ExitProcess(15);
    }

    return bufferSRV;
}

internal
ID3D11ShaderResourceView*
MakeTextureSRV(ID3D11Device* device, ID3D11Texture2D* buffer)
{
    ID3D11ShaderResourceView* bufferSRV = {};
    D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};

    desc.Format = DXGI_FORMAT_R32_FLOAT;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipLevels = 1;
    desc.Texture2D.MostDetailedMip = 0;
    if (HRESULT r = device->CreateShaderResourceView(buffer, &desc, &bufferSRV);
        FAILED(r)) {
        ExitProcess(15);
    }

    return bufferSRV;
}

internal
ID3D11UnorderedAccessView*
MakeBufferUAV(ID3D11Device* device, ID3D11Buffer* buffer, int elements)
{
    ID3D11UnorderedAccessView* bufferUAV = {};
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};

    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    desc.Buffer.FirstElement = 0;
    desc.Buffer.NumElements = elements;
    if (HRESULT r = device->CreateUnorderedAccessView(buffer, &desc, &bufferUAV);
        FAILED(r)) {
        ExitProcess(14);
    }

    return bufferUAV;
}

internal
ID3D11UnorderedAccessView*
MakeTextureUAV(ID3D11Device* device, ID3D11Texture2D* texture)
{
    ID3D11UnorderedAccessView* bufferUAV = {};
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};

    desc.Format = DXGI_FORMAT_R32_FLOAT;
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = 0;
    if (HRESULT r = device->CreateUnorderedAccessView(texture, &desc, &bufferUAV);
        FAILED(r)) {
        ExitProcess(15);
    }

    return bufferUAV;
}

internal
Vec2 
RandomOrientation(std::mt19937& rng)
{
    std::uniform_real_distribution<float> dirDist(0, PI*2);

    float rads = dirDist(rng);
    return Vec2{ cosf(rads), sinf(rads) };
}


int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    std::mt19937 rng;
    
    Simulation sim;
    Configure(sim);

    LARGE_INTEGER liFreq;
    QueryPerformanceFrequency(&liFreq);
    u64 freq = liFreq.QuadPart;

    WNDCLASS windowClass = {};
    windowClass.hInstance = instance;
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpszClassName = "CoolTM_DX11";
    windowClass.lpfnWndProc = &Win32_Window_Tick;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&windowClass);

    int WindowWidth = 1280;
    int WindowHeight = 720;

    int mouseX = 0;
    int mouseY = 0;

    HWND window = CreateWindowExA(
        0,
        windowClass.lpszClassName,
        "CoolTM - DX11",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth,
        WindowHeight,
        0,
        0,
        instance,
        0);

    {
        RECT r;
        GetWindowRect(window, &r);
        r.right = r.left + WindowWidth;
        r.bottom = r.top + WindowHeight;

        AdjustWindowRect(&r, GetWindowLong(window, GWL_STYLE), false);
        SetWindowPos(
            window,
            0,
            r.left,
            r.top,
            r.right - r.left,
            r.bottom - r.top,
            SWP_NOZORDER | SWP_NOACTIVATE);

        UpdateWindow(window);
    }

    ShowWindow(window, SW_SHOWNORMAL);


    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WindowWidth;
    sd.BufferDesc.Height = WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    D3D_FEATURE_LEVEL dxFeatureLevelReq = D3D_FEATURE_LEVEL_11_1;

    IDXGISwapChain* dxSwapChain;
    ID3D11Device* dxDevice;
    ID3D11DeviceContext* dxContext;

    UINT d3dFlags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG;

    HRESULT r = D3D11CreateDeviceAndSwapChain(
        0,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        d3dFlags,
        &dxFeatureLevelReq,
        1,
        D3D11_SDK_VERSION,
        &sd,
        &dxSwapChain,
        &dxDevice,
        0,
        &dxContext);

    if (FAILED(r)) {
        ExitProcess(3);
    }

    ID3D11ComputeShader* dxSimulateMonitor = MakeShader(dxDevice, L"simulate.hlsl", "Simulate_Monitor");
    // ID3D11ComputeShader* dxSimulateSense   = MakeShader(dxDevice, L"simulate.hlsl", "Simulate_Sense");
    // ID3D11ComputeShader* dxSimulateDiffuse = MakeShader(dxDevice, L"simulate.hlsl", "Simulate_Diffuse");


    ID3D11Texture2D* dxKernelInput = MakeTexture(
        dxDevice,
        sim,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
        0);

    ID3D11Texture2D* dxKernelOutput = MakeTexture(
        dxDevice,
        sim,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
        D3D11_CPU_ACCESS_READ);

    ID3D11Texture2D* dxTemporary = MakeTexture(
        dxDevice,
        sim,
        D3D11_USAGE_STAGING,
        0,
        D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ);


    ID3D11Buffer* dxBufferParticles = MakeBuffer<Particle>(
        dxDevice,
        sim.Particles,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
        D3D11_CPU_ACCESS_WRITE
    );
    ID3D11Buffer* dxBufferParticlesTemporary = MakeBuffer<Particle>(
        dxDevice,
        sim.Particles,
        D3D11_USAGE_STAGING,
        0,
        D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ
    );

    ID3D11Buffer* dxBufferSimulation = MakeBuffer<Simulation>(
        dxDevice,
        1,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
        D3D11_CPU_ACCESS_WRITE
    );

    // struct Ints
    // {
    //     UINT x, y, z;
    // };
    // ID3D11Buffer* dxBufferDispatch = MakeBuffer<Ints>(
    //     dxDevice,
    //     1,
    //     D3D11_USAGE_DEFAULT,
    //     D3D11_BIND_,
    //     D3D11_CPU_ACCESS_WRITE,
    //     D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS
    // );

    Image   draw; MakeImage(sim,  draw);
    ImageF trail; MakeImage(sim, trail);

    D3D11_MAPPED_SUBRESOURCE mapped;
    if (r = dxContext->Map(dxBufferParticles, 0, D3D11_MAP_WRITE, 0, &mapped); !FAILED(r) ) {
        for (int i = 0; i < sim.Particles; ++i){
            Particle* p = ((Particle*)mapped.pData) + i;

            p->pos.x = sim.W / 2;
            p->pos.y = sim.H / 2;
            p->dir = RandomOrientation(rng);
        }
        dxContext->Unmap(dxBufferParticles, 0);
    } else {
        ExitProcess(5);
    }

    if (r = dxContext->Map(dxBufferSimulation, 0, D3D11_MAP_WRITE, 0, &mapped); !FAILED(r) ) {
        memcpy(mapped.pData, &sim, sizeof(Simulation));
        dxContext->Unmap(dxBufferSimulation, 0);
    } else {
        ExitProcess(5);
    }

    // if (r = dxContext->Map(dxBufferDispatch, 0, D3D11_MAP_WRITE, 0, &mapped); !FAILED(r) ) {
    //     Ints* args = (Ints*)(mapped.pData);
    //     args->x = sim.Particles / 1024;
    //     args->y = 1;
    //     args->z = 1;
    //     dxContext->Unmap(dxBufferDispatch, 0);
    // } else {
    //     ExitProcess(5);
    // }

    HDC dc = GetDC(window);

    BITMAPINFOHEADER bmiHeader = {};
    bmiHeader.biSize = sizeof(BITMAPINFO);
    bmiHeader.biWidth = sim.W;
    bmiHeader.biHeight = -sim.H;
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = BI_RGB;
    bmiHeader.biSizeImage = 0;
    bmiHeader.biXPelsPerMeter = 0;
    bmiHeader.biYPelsPerMeter = 0;
    bmiHeader.biClrUsed = 0;
    bmiHeader.biClrImportant = 0;

    constexpr auto Now = [] {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return li.QuadPart;
    };

    freq /= 1000;
    i64 start = Now() / freq;
    i64 elapsed = 0;

    bool left = false;
    bool middle = false;
    bool right = false;

    ID3D11ShaderResourceView*  roViews[2] = {};
    ID3D11UnorderedAccessView* rwViews[2] = {};

    ID3D11ShaderResourceView*  dxSRVSim         = MakeBufferSRV (dxDevice, dxBufferSimulation, 1);
    ID3D11ShaderResourceView*  dxSRVInTrailMap  = MakeTextureSRV(dxDevice, dxKernelInput);
    roViews[0] = dxSRVSim;
    roViews[1] = dxSRVInTrailMap;

    ID3D11UnorderedAccessView* dxUAVParticles   = MakeBufferUAV (dxDevice, dxBufferParticles, sim.Particles);
    ID3D11UnorderedAccessView* dxUAVOutTrailMap = MakeTextureUAV(dxDevice, dxKernelOutput);
    rwViews[0] = dxUAVParticles;
    rwViews[1] = dxUAVOutTrailMap;

    dxContext->CSSetShader(dxSimulateMonitor, NULL, 0);
    roViews[0] = dxSRVSim;
    roViews[1] = dxSRVInTrailMap;
    rwViews[0] = dxUAVParticles;
    rwViews[1] = dxUAVOutTrailMap;
    dxContext->CSSetShaderResources     (0, 2, roViews);
    dxContext->CSSetUnorderedAccessViews(0, 2, rwViews, NULL);

    float dt = sim.DT;
    while (true) {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT || msg.message == WM_DESTROY ||
                msg.message == WM_CLOSE)
                return 0;

            if (msg.message == WM_LBUTTONDOWN)
                left = true;
            if (msg.message == WM_LBUTTONUP)
                left = false;
            if (msg.message == WM_RBUTTONDOWN)
                right = true;
            if (msg.message == WM_RBUTTONUP)
                right = false;
            if (msg.message == WM_MBUTTONDOWN)
                middle = true;
            if (msg.message == WM_MBUTTONUP)
                middle = false;
            if (msg.message == WM_MOUSEMOVE || msg.message == WM_MOUSEHOVER) {
                mouseX = (int)(short)LOWORD(msg.lParam);
                mouseY = (int)(short)HIWORD(msg.lParam);
            }
        }
        if (!left) {
            dxContext->CSSetShader(dxSimulateMonitor, NULL, 0);
            dxContext->Dispatch(sim.Particles / 320, 1, 1);

            // dxContext->CSSetShader(dxSimulateSense, NULL, 0);
            // dxContext->DispatchIndirect(dxBufferDispatch, 0);

            // dxContext->Dispatch((sim.Particles) / (64 * 64), 64, 1);

            // dxContext->CSSetShader(dxSimulateSense,   NULL, 0);
            // dxContext->Dispatch(1, 1, 1);
            // dxContext->CSSetShader(dxSimulateDiffuse, NULL, 0);
            // dxContext->Dispatch(1, 1, 1);
            // dxContext->CopyResource(dxKernelInput, dxKernelOutput);
        } else {
            Sleep(10);
        }

        i64 now = Now() / freq;
        i64 took = now - start;
        elapsed += took;
        start = now;

        if (middle) {
            // TODO: re-implement this
        }
        if (elapsed > 16) {
            elapsed = elapsed % 16;

            dxContext->CopyResource(dxTemporary, dxKernelOutput);
            if (r = dxContext->Map(dxTemporary, 0, D3D11_MAP_READ, 0, &mapped); !FAILED(r) ) {
                for (int y = 0; y < sim.H; ++y)
                    for (int x = 0; x < sim.W; ++x) {
                        char* textureBytes = (char*)mapped.pData;
                        textureBytes += x * sizeof(float) + y * mapped.RowPitch;
                        float* texturePixel = (float*)textureBytes;

                        int i = x + y * sim.W;
                        float grayFPx = MinF32(*texturePixel, 1.0f);
                        int grayPx = int(grayFPx * 255);
                        int rgbPx = grayPx | (grayPx << 8) | (grayPx << 16);
                        draw.px[i] = rgbPx;
                    }
                dxContext->Unmap(dxTemporary, 0);
            } else {
                ExitProcess(6);
            }

            if (left) {
                // NOTE: Sample the Trail map.
                int i = mouseX + mouseY * sim.W;
                char buf[10] = {};
                float value = trail.px[i];
                snprintf(buf, sizeof(buf), "%02.2f\n", value);
                OutputDebugStringA(buf);
            }

            SetDIBitsToDevice(
                dc,
                0,
                0,
                WindowWidth,
                WindowHeight,
                0,
                0,
                0,
                sim.H,
                draw.px,
                (BITMAPINFO*)&bmiHeader,
                DIB_RGB_COLORS);
    
            char buf [128];
            sprintf(buf, "%04dms\n", (int)took);
            OutputDebugStringA(buf);
        }
    }
    return 0;
}