#include "action.h"
#include "jetson.h" // ���ͷ�ļ������Է���coord_array

#define distance_with_sehuan 150

static uint8_t put_to_map_state = 0;
static uint8_t is_send_ok = 0;

void Action_Catch_Obj_To_ZhuanPan(uint8_t position)
{
  Crawl_Open();
  HuaGui_ZaiWu(1000, 230);
  Delay_ms(2000);

  Crawl_Close();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);

  // ����positionѡ����Ӧ�Ķ�λ����
  switch (position)
  {
  case 1:
    Location_First();
    break;
  case 2:
    Location_Second();
    break;
  case 3:
    Location_Third();
    break;
  default:
    break;
  }
  Delay_ms(3000);

  HuaGui_UP(1000, 230);
  Backward();
  Delay_ms(3000);

  HuaGui_ZhuanPan(1000, 230);
  Delay_ms(2000);

  Crawl_Open();
  Delay_ms(2000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);

  Forward();
  Delay_ms(3000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);
  running_flag++;
}

void Action_Put_Obj_To_Map(uint8_t position)
{
  Backward();
  // ����positionѡ����Ӧ�Ķ�λ����
  switch (position)
  {
  case 1:
    Location_First();
    break;
  case 2:
    Location_Second();
    break;
  case 3:
    Location_Third();
    break;
  default:
    break;
  }
  Delay_ms(5000);

  HuaGui_ZhuanPan(1000, 230);
  Delay_ms(1000);

  Crawl_Close();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(600);

  Forward();
  Delay_ms(3000);

  HuaGui_Fang_To_Map(1000, 230);
  Delay_ms(5000);

  Crawl_Open();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);
}

void Action_Put_Obj_To_Map_MaDuo(uint8_t position)
{
  Backward();
  // ����positionѡ����Ӧ�Ķ�λ����
  switch (position)
  {
  case 1:
    Location_First();
    break;
  case 2:
    Location_Second();
    break;
  case 3:
    Location_Third();
    break;
  default:
    break;
  }
  Delay_ms(5000);

  HuaGui_ZhuanPan(1000, 230);
  Delay_ms(1000);

  Crawl_Close();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(600);

  Forward();
  Delay_ms(3000);

  HuaGui_Fang_To_Map_2(1000, 230);
  Delay_ms(5000);

  Crawl_Open();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);
}

void Action_Get_Obj_From_Map(uint8_t position)
{
  // ����positionѡ����Ӧ�Ķ�λ����
  switch (position)
  {
  case 1:
    Location_First();
    break;
  case 2:
    Location_Second();
    break;
  case 3:
    Location_Third();
    break;
  default:
    break;
  }
  HuaGui_Get_From_Map(1000, 230);
  Delay_ms(4000);

  Crawl_Close();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(2000);

  Backward();
  Delay_ms(3000);

  HuaGui_ZhuanPan(1000, 230);
  Delay_ms(1000);

  Crawl_Open();
  Delay_ms(1000);

  HuaGui_UP(1000, 230);
  Delay_ms(1000);

  Forward();
  Delay_ms(3000);
}

uint8_t Action_Put_Obj_To_Map_321(void)
{
  // ֻ�е�Action_Counter_EnableΪ0ʱ��ִ��״̬�л�
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0: // �ƶ���3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 200;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 1: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;

    case 2: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 3: // �ƶ���2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 200;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 4: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;
    case 5: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 6: // �ƶ���1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 200;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 7: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;
    case 8: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;
    case 9: // �ص�3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 250;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 10: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;
    case 11: // �ص�2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 12: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;
    case 13: // �ص�1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 14: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;
    case 15: // �������˳�
      // Jetson_Send(0x05);
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // ����0��ʾ������δ���
  return 0;
}

uint8_t Action_Put_Obj_To_Map_312(void)
{
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      //printf("t8.txt=\"sd1\"\xff\xff\xff");
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 2:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 3:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 4:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 5:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 6:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 7:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 8:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;

    case 9:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 10:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;

    case 11:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 12:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;

    case 13:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 14:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;

    case 15:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  return 0;
}


uint8_t Action_Put_Obj_To_Map_123(void)
{
  // ֻ�е�Action_Counter_EnableΪ0ʱ��ִ��״̬�л�
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0: // �ƶ���3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;

    case 2: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 3: // �ƶ���2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 4: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;
    case 5: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 6: // �ƶ���1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 7: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;
    case 8: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;

    case 9: // �ص�3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 250;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;
    case 10: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;
    case 11: // �ص�2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;
    case 12: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;
    case 13: // �ص�1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;
    case 14: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;
    case 15: // �������˳�
      // Jetson_Send(0x05);
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // ����0��ʾ������δ���
  return 0;
}

uint8_t Action_Put_Obj_To_Map_132(void)
{
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      printf("t8.txt=\"sd1\"\xff\xff\xff");
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 1:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;

    case 2:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 3:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;

    case 5:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 6:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 7:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;

    case 8:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;

    case 9:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 10:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;

    case 11:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 12:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;

    case 13:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 14:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;

    case 15:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  return 0;
}

uint8_t Action_Put_Obj_To_Map_213(void)
{
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;
    case 1:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 2:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 3:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 5:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 6:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 7:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;

    case 8:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 9:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;

    case 10:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 11:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;

    case 12:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 13:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;

    case 14:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  return 0;
}

uint8_t Action_Put_Obj_To_Map_231(void)
{
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        printf("t8.txt=\"sd2\"\xff\xff\xff");
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;
    case 1:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 2:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 3:
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 4:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 5:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 6:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {
        Counter_Times = 100;
        Counter_Enable = 1;
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);
        is_send_ok = 0;
        put_to_map_state++;
      }
      break;

    case 7:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;

    case 8:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 9:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;

    case 10:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 11:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;

    case 12:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 100, 150);
      put_to_map_state++;
      break;

    case 13:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;

    case 14:
      printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  return 0;
}

uint8_t Action_Put_Obj_To_Map_321_Tempstore(void)
{
  // ֻ�е�Action_Counter_EnableΪ0ʱ��ִ��״̬�л�
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0: // �ƶ���3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 1: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;

    case 2: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 3: // �ƶ���2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 4: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;
    case 5: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 6: // �ƶ���1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 7: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;
    case 8: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;
    case 9: // �������˳�
      // Jetson_Send(0x05);
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // ����0��ʾ������δ���
  return 0;
}

uint8_t Action_Put_Obj_To_Map_123_Tempstore(void)
{
  // ֻ�е�Action_Counter_EnableΪ0ʱ��ִ��״̬�л�
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0: // �ƶ���3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 1: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;

    case 2: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(1);
      put_to_map_state++;
      break;

    case 3: // �ƶ���2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 4: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 200, 150);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;
    case 5: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(2);
      put_to_map_state++;
      break;

    case 6: // �ƶ���1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 7: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;
    case 8: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map(3);
      put_to_map_state++;
      break;
    case 9: // �������˳�
      // Jetson_Send(0x05);
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // ����0��ʾ������δ���
  return 0;
}

uint8_t Action_Put_Obj_To_Map_321_Tempstore_Again(void)
{
  // ֻ�е�Action_Counter_EnableΪ0ʱ��ִ��״̬�л�
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0: // �ƶ���3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 1: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;

    case 2: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map_MaDuo(1);
      put_to_map_state++;
      break;

    case 3: // �ƶ���2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 4: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;
    case 5: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map_MaDuo(2);
      put_to_map_state++;
      break;

    case 6: // �ƶ���1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 7: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;
    case 8: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map_MaDuo(3);
      put_to_map_state++;
      break;

    case 9: // �ص�3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 250;
      Counter_Enable = 1;
      Car_Go_Target(0, 2 * distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 10: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;
    case 11: // �ص�2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 12: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;
    case 13: // �ص�1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 14: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;
    case 15: // �������˳�
      // Jetson_Send(0x05);
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // ����0��ʾ������δ���
  return 0;
}

uint8_t Action_Put_Obj_To_Map_123_Tempstore_Again(void)
{
  // ֻ�е�Action_Counter_EnableΪ0ʱ��ִ��״̬�л�
  if (Counter_Enable == 0)
  {
    switch (put_to_map_state)
    {
    case 0: // �ƶ���3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, -distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 1: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;

    case 2: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map_MaDuo(1);
      put_to_map_state++;
      break;

    case 3: // �ƶ���2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;

    case 4: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }
      break;
    case 5: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map_MaDuo(2);
      put_to_map_state++;
      break;

    case 6: // �ƶ���1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      // Jetson_Send(0x05);
      put_to_map_state++;
      break;

    case 7: // ΢��
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      if (!is_send_ok)
      {
        Jetson_Send(0x05);
        is_send_ok = 1;
      }

      // ����coord_array��ֵ����΢��
      if (coord_array[0] != 0 || coord_array[1] != 0)
      {

        Counter_Times = 100;
        Counter_Enable = 1;
        // X����΢��
        Car_Go_Target(coord_array[0], coord_array[1], 50, 50);

        is_send_ok = 0;
        coord_array[0] = 0;
        coord_array[1] = 0;
        put_to_map_state++;
      }

      break;
    case 8: // ����
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Put_Obj_To_Map_MaDuo(3);
      put_to_map_state++;
      break;

    case 9: // �ص�3
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 250;
      Counter_Enable = 1;
      Car_Go_Target(0, -2 * distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 10: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(1);
      put_to_map_state++;
      break;
    case 11: // �ص�2
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 12: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(2);
      put_to_map_state++;
      break;
    case 13: // �ص�1
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Counter_Times = 100;
      Counter_Enable = 1;
      Car_Go_Target(0, distance_with_sehuan, 50, 50);
      put_to_map_state++;
      break;
    case 14: // ��ȡ
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      Action_Get_Obj_From_Map(3);
      put_to_map_state++;
      break;
    case 15: // �������˳�
      // Jetson_Send(0x05);
      //printf("t9.txt=\"%d\"\xff\xff\xff", put_to_map_state);
      put_to_map_state = 0;
      return 1;
    }
  }
  // ����0��ʾ������δ���
  return 0;
}