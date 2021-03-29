// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xgaussian.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XGaussian_CfgInitialize(XGaussian *InstancePtr, XGaussian_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_bus_BaseAddress = ConfigPtr->Control_bus_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XGaussian_Start(XGaussian *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL) & 0x80;
    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL, Data | 0x01);
}

u32 XGaussian_IsDone(XGaussian *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XGaussian_IsIdle(XGaussian *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XGaussian_IsReady(XGaussian *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XGaussian_EnableAutoRestart(XGaussian *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL, 0x80);
}

void XGaussian_DisableAutoRestart(XGaussian *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_AP_CTRL, 0);
}

void XGaussian_InterruptGlobalEnable(XGaussian *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_GIE, 1);
}

void XGaussian_InterruptGlobalDisable(XGaussian *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_GIE, 0);
}

void XGaussian_InterruptEnable(XGaussian *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_IER);
    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_IER, Register | Mask);
}

void XGaussian_InterruptDisable(XGaussian *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_IER);
    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_IER, Register & (~Mask));
}

void XGaussian_InterruptClear(XGaussian *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XGaussian_WriteReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_ISR, Mask);
}

u32 XGaussian_InterruptGetEnabled(XGaussian *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_IER);
}

u32 XGaussian_InterruptGetStatus(XGaussian *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XGaussian_ReadReg(InstancePtr->Control_bus_BaseAddress, XGAUSSIAN_CONTROL_BUS_ADDR_ISR);
}

