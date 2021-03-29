// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XGAUSSIAN_H
#define XGAUSSIAN_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xgaussian_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_bus_BaseAddress;
} XGaussian_Config;
#endif

typedef struct {
    u64 Control_bus_BaseAddress;
    u32 IsReady;
} XGaussian;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XGaussian_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XGaussian_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XGaussian_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XGaussian_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XGaussian_Initialize(XGaussian *InstancePtr, u16 DeviceId);
XGaussian_Config* XGaussian_LookupConfig(u16 DeviceId);
int XGaussian_CfgInitialize(XGaussian *InstancePtr, XGaussian_Config *ConfigPtr);
#else
int XGaussian_Initialize(XGaussian *InstancePtr, const char* InstanceName);
int XGaussian_Release(XGaussian *InstancePtr);
#endif

void XGaussian_Start(XGaussian *InstancePtr);
u32 XGaussian_IsDone(XGaussian *InstancePtr);
u32 XGaussian_IsIdle(XGaussian *InstancePtr);
u32 XGaussian_IsReady(XGaussian *InstancePtr);
void XGaussian_EnableAutoRestart(XGaussian *InstancePtr);
void XGaussian_DisableAutoRestart(XGaussian *InstancePtr);


void XGaussian_InterruptGlobalEnable(XGaussian *InstancePtr);
void XGaussian_InterruptGlobalDisable(XGaussian *InstancePtr);
void XGaussian_InterruptEnable(XGaussian *InstancePtr, u32 Mask);
void XGaussian_InterruptDisable(XGaussian *InstancePtr, u32 Mask);
void XGaussian_InterruptClear(XGaussian *InstancePtr, u32 Mask);
u32 XGaussian_InterruptGetEnabled(XGaussian *InstancePtr);
u32 XGaussian_InterruptGetStatus(XGaussian *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
