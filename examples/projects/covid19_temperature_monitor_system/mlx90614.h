#ifndef MLX90614_H_
#define MLX90614_H_

#define MLX90614_WRITE_ADDR     0xB4
#define MLX90614_READ_ADDR      0xB5
#define MLX90614_AMBIENT_TEMP   0x06
#define MLX90614_TARGET_TEMP    0x07

#define HIGH 1
#define LOW 0

typedef struct mlx90614TempSensor{
	unsigned char sclPin;
	unsigned char  sdaPin;
}mlx90614;

double mlx90614ReadAmbientTempC(mlx90614 tempSensor);
double mlx90614ReadTargetTempC(mlx90614 tempSensor);
void mlx90614Start(mlx90614 tempSensor);
void mlx90614Stop(mlx90614 tempSensor);
void mlx90614Restart(mlx90614 tempSensor);
void delay_10u(void);
void delay_250u(void);
void delay_5u(void);

#endif