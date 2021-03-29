// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xgaussian.h"

extern XGaussian_Config XGaussian_ConfigTable[];

XGaussian_Config *XGaussian_LookupConfig(u16 DeviceId) {
	XGaussian_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XGAUSSIAN_NUM_INSTANCES; Index++) {
		if (XGaussian_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XGaussian_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XGaussian_Initialize(XGaussian *InstancePtr, u16 DeviceId) {
	XGaussian_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XGaussian_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XGaussian_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

