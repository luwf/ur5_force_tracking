#ifndef _data_H
#define _data_H


#pragma pack(1)
#pragma pack(push)
#pragma pack(1)

/*接收传感器的数据结构*/

typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char byte;

typedef struct response_struct {
	uint32 rdt_sequence;
	uint32 ft_sequence;
	uint32 status;
	int32 FTData[6];    // "Fx", "Fy", "Fz", "Tx", "Ty", "Tz" 
} RESPONSE;

typedef struct{
	char Flg;
	double Force_Torque[6];//传感器力与力矩
}SENDUDPDATA;

/*接收UR的数据结构*/
typedef struct UrRobotState
{
	int Message_Size;
	double Time;
	double q_target[6];
	double qd_target[6];
	double qdd_target[6];
	double I_target[6];
	double M_target[6];
	double q_actual[6];
	double qd_actual[6];
	double I_actual[6];
	double I_control[6];
	double Tool_vector_actual[6];
	double TCP_speed_actual[6];
	double TCP_force[6];
	double Tool_vector_target[6];
	double TCP_speed_target[6];
	double Digital_input_bits;
	double Motor_temperatures[6];
	double Controller_Timer;
	double Test_value;
	double Robot_Mode;
	double Joint_Modes[6];
	double Safety_Mode;
	double unknown1[6];
	double Tool_Accelerometer_values[3];
	double unknown2[6];
	double Speed_scaling;
	double Linear_momentum_norm;
	double unknown3;
	double unknown4;
	double V_main;
	double V_robot;
	double I_robot;
	double V_actual[6];
	double Digital_outputs;
	double Program_state;
}UrRobotState;


//转换8字节整形
typedef unsigned long long UINT64;
#define ntohll(x) ( ((UINT64)x & 0xff00000000000000LL)>>56 |\
	((UINT64)x & 0x00ff000000000000LL)>>40 |\
	((UINT64)x & 0x0000ff0000000000LL)>>24 |\
	((UINT64)x & 0x000000ff00000000LL)>>8 | \
	((UINT64)x & 0x00000000ff000000LL)<<8 | \
	((UINT64)x & 0x0000000000ff0000LL)<<24 |\
	((UINT64)x & 0x000000000000ff00LL)<<40 | \
	((UINT64)x & 0x00000000000000ffLL)<<56 )


/* 为了使问题清晰，先定义个函数 */
double SwapDoubleEndian(double* pdVal)
{
	UINT64 llVal = ntohll(*((UINT64*)pdVal));
	return *((double*)&llVal);
}

#pragma pack(pop)
#endif