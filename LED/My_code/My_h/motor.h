# ifndef __MOTOR__H__
# define __MOTOR__H__
void Init_Motor(void);
void Motor_On(void);
void Motor_Off(void);
void Motor_PWM_SET(uint8_t speed);
void Motor_Task(void *pvParameters);
# endif
