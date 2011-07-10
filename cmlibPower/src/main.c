#include <pspkernel.h>
#include <systemctrl.h>
#include <string.h>
#include "cmlibpower.h"

/*------------------------------------------------------------*/

PSP_MODULE_INFO("cmlibPower", PSP_MODULE_KERNEL, 1, 0);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  POWER CONTROLL
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//#define SLEEP_MODE    0(cmlibpower.h)
//#define REBOOT_MODE   1(cmlibpower.h)
//#define SHUTDOWN_MODE 2(cmlibpower.h)
//---------------------------
int libPowerControll(int mode)
{
	switch(mode) {
		case SLEEP_MODE:
			scePowerRequestSuspend();
			break;

		case REBOOT_MODE:
			sceKernelDcacheWritebackAll();
			sceKernelIcacheClearAll();
			sceKernelExitVSHVSH(0);
			break;

		case SHUTDOWN_MODE:
			scePowerRequestStandby();
			break;
	}
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  EXE SOFT RESET
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define EBOOT_PATH "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN"
//---------------------
void libExeSoftReset(void)
{
	int k1 = pspSdkSetK1(0);
	
	u32 vshmain_args[0x0400/4];
	memset(vshmain_args, 0, 0x0400);
	vshmain_args[0/4] = 0x0400;
	vshmain_args[4/4] = 0x20;
	vshmain_args[0x40/4] = 1;
	vshmain_args[0x280/4] = 1;
	vshmain_args[0x284/4] = 0x40003;
	
	struct SceKernelLoadExecVSHParam param;
	
	memset(&param, 0, sizeof(param));
	param.size = sizeof(param);
	param.args = strlen(EBOOT_PATH)+1;
	param.argp = EBOOT_PATH;
	param.key = "game";
	param.vshmain_args_size = 0x0400;
	param.vshmain_args = vshmain_args;

	sctrlKernelLoadExecVSHWithApitype(0x120, EBOOT_PATH, &param); 

	pspSdkSetK1(k1);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  SET CPU INFO
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libSetCpuClock(int cpuClock)
{
	
	scePowerSetCpuClockFrequency(cpuClock);
	scePowerSetBusClockFrequency(cpuClock/2);
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET CPU INFO
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
CPU_INFO libGetCpuClock(void)
{
	CPU_INFO info;
	
	info.cpuClock = scePowerGetCpuClockFrequency();
	info.busClock = scePowerGetBusClockFrequency();
	
	return info;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET CHARGE STATUS
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libGetChargeStatus(void)
{
	
	return scePowerGetBatteryChargingStatus();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET BATTERY LIFE PARCENT
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libGetBatteryLifePer(void)
{
	
	return scePowerGetBatteryLifePercent();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET BATTERY LIFE TIME
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libGetBatteryLifeTime(void)
{
	
	return scePowerGetBatteryLifeTime();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET BATTERY TEMP
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libGetBatteryTemp(void)
{
	
	return scePowerGetBatteryTemp();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET BATTERY VOLT
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float libGetBatteryVolt(void)
{
	float result = (float) scePowerGetBatteryVolt() / 1000.0;
	return result;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  IS POWER ONLINE
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libIsPowerOnline(void)
{
	
	return scePowerIsPowerOnline();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  IS BATTERY EXIST
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libIsBatteryExist(void)
{
	
	return scePowerIsBatteryExist();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  IS LOW BATTERY
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libIsLowBattery(void)
{
	
	return scePowerIsLowBattery();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  IS BATTERY CHARGING
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int libIsBatteryCharging(void)
{
	
	return scePowerIsBatteryCharging();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  GET POWER INFO
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
POWER_INFO libGetPowerInfo(void)
{
	POWER_INFO info;
	
	info.chargeStatus	= libGetChargeStatus();
	info.lifePer		= libGetBatteryLifePer();
	info.lifeTime		= libGetBatteryLifeTime();

	CPU_INFO cupInfo	= libGetCpuClock();
	info.cpuClock		= 0;
	info.busClock		= 0;
	info.cpuClock		= cupInfo.cpuClock;
	info.busClock		= cupInfo.busClock;

	info.temp			= libGetBatteryTemp();
	info.volt			= libGetBatteryVolt();
	info.acStatus		= libIsPowerOnline();
	info.isExist		= libIsBatteryExist();
	info.isLow			= libIsLowBattery();
	info.isCharging		= libIsBatteryCharging();
	
	return info;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  ENTRY POINT
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int module_start(SceSize args, void *argp)
{
	return 0;
}

int module_stop(SceSize args, void *argp)
{
	return 0;
}
