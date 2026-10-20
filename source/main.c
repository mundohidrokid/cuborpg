#include <3ds.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>

#include "vshader_shbin.h"

#define CLEAR_COLOR 0x202840FF

#define DISPLAY_TRANSFER_FLAGS \
    (GX_TRANSFER_FLIP_VERT(0) | \
     GX_TRANSFER_OUT_TILED(0) | \
     GX_TRANSFER_RAW_COPY(0) | \
     GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
     GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
     GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

typedef struct
{
    float x;
    float y;
    float z;
} Vertex;

static const Vertex cubeVertices[] =
{
    // Frente
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},

    {-0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},

    // Trás
    { 0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},

    { 0.5f, -0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},

    // Esquerda
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},

    {-0.5f, -0.5f, -0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},

    // Direita
    { 0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},

    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f,  0.5f},

    // Cima
    {-0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},

    {-0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},

    // Baixo
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},

    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},
};

static DVLB_s* vshader;
static shaderProgram_s program;
static C3D_RenderTarget* target;
static int uLoc_projection;
static C3D_Mtx projection;

static void sceneInit(void)
{
    vshader = DVLB_ParseFile(
        (u32*)vshader_shbin,
        vshader_shbin_size
    );

    shaderProgramInit(&program);
    shaderProgramSetVsh(
        &program,
        &vshader->DVLE[0]
    );

    C3D_BindProgram(&program);

    uLoc_projection =
        shaderInstanceGetUniformLocation(
            program.vertexShader,
            "projection"
        );

    C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);

    AttrInfo_AddLoader(
        attrInfo,
        0,
        GPU_FLOAT,
        3
    );

    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);

    BufInfo_Add(
        bufInfo,
        cubeVertices,
        sizeof(Vertex),
        1,
        0x210
    );

    Mtx_PerspTilt(
        &projection,
        C3D_AngleFromDegrees(60.0f),
        400.0f / 240.0f,
        0.1f,
        100.0f
    );

    C3D_FVUnifMtx4x4(
        GPU_VERTEX_SHADER,
        uLoc_projection,
        &projection
    );
}

static void sceneRender(void)
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    C3D_RenderTargetClear(
        target,
        C3D_CLEAR_ALL,
        CLEAR_COLOR,
        0
    );

    C3D_FrameDrawOn(target);

    C3D_FVUnifMtx4x4(
        GPU_VERTEX_SHADER,
        uLoc_projection,
        &projection
    );

    C3D_DrawArrays(
        GPU_TRIANGLES,
        0,
        sizeof(cubeVertices) / sizeof(Vertex)
    );

    C3D_FrameEnd(0);
}

int main(void)
{
    gfxInitDefault();

    consoleInit(GFX_BOTTOM, NULL);

    // Inicializa o Citro3D e a GPU.
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

    target = C3D_RenderTargetCreate(
        240,
        400,
        GPU_RB_RGBA8,
        GPU_RB_DEPTH24_STENCIL8
    );

    C3D_RenderTargetSetOutput(
        target,
        GFX_TOP,
        GFX_LEFT,
        DISPLAY_TRANSFER_FLAGS
    );

    sceneInit();

    printf("CUBO RPG\n");
    printf("================\n");
    printf("BUILD 0.1.0\n\n");
    printf("HP: 100/100\n\n");
    printf("Cubo 3D inicializado!\n");
    printf("START = sair\n");

    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
            break;

        sceneRender();
    }

    C3D_Fini();
    gfxExit();

    return 0;
}
